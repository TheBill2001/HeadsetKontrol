// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hklogstorage.hpp"

#include "hkglobal.hpp"

#include <QApplicationStatic>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QThreadPool>

using namespace Qt::StringLiterals;

namespace
{
constexpr quint32 HKError_MAGIC = 0x4EE5C9E8;
}

QDataStream &operator<<(QDataStream &stream, const HKLogEntry &entry)
{
    stream << HKError_MAGIC << quint32(QT_VERSION_CHECK(3, 0, 0)); // NOLINT(*signed-bitwise)
    stream.setVersion(QDataStream::Qt_6_9);
    stream << entry.m_id;
    stream << entry.m_title;
    stream << entry.m_message;
    stream << entry.m_details;
    stream << entry.m_timestamp;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, HKLogEntry &entry)
{
    {
        quint32 magic{};
        stream >> magic;

        if (magic != HKError_MAGIC) {
            qCWarning(HK_LOGGING, "Invalid HKError signature!");
            stream.setStatus(QDataStream::ReadCorruptData);
            return stream;
        }
    }

    quint32 version{};
    stream >> version;

    if (version == QT_VERSION_CHECK(3, 0, 0)) { // NOLINT(*signed-bitwise)
        stream.setVersion(QDataStream::Qt_6_9);
        stream >> entry.m_id;
        stream >> entry.m_title;
        stream >> entry.m_message;
        stream >> entry.m_details;
        stream >> entry.m_timestamp;
    }

    return stream;
}

// NOLINTNEXTLINE(*avoid-non-const-global-variables)
Q_APPLICATION_STATIC(HKLogStorage, g_instance, QStandardPaths::writableLocation(QStandardPaths::StateLocation) + u"/headsetkontrol.log"_s)

HKLogStorage::HKLogStorage(const QString &filePath, QObject *parent)
    : HKLogStorage{new QFile(filePath), parent}
{
    m_file->moveToThread(thread());
    m_file->setParent(this);
}

HKLogStorage::HKLogStorage(QFile *file, QObject *parent)
    : QObject{parent}
    , m_file{file}
    , m_limit{0}
    , m_ageLimit{0}
{
    Q_ASSERT(m_file);

    m_debounceTimer.setSingleShot(true);
    m_debounceTimer.setTimerType(Qt::CoarseTimer);
    m_debounceTimer.setInterval(std::chrono::milliseconds{100});

    connect(this, &HKLogStorage::limitChanged, this, [this] {
        filter(entries());
    });

    connect(this, &HKLogStorage::ageLimitChanged, this, [this] {
        filter(entries());
    });

    connect(this, &HKLogStorage::entriesChanged, &m_debounceTimer, qOverload<>(&QTimer::start));
    connect(&m_debounceTimer, &QTimer::timeout, this, [this] {
        QThreadPool::globalInstance()->start([this] {
            write();
        });
    });

    read();
}

HKLogStorage::~HKLogStorage()
{
    write();
}

HKLogStorage *HKLogStorage::instance()
{
    return g_instance;
}

void HKLogStorage::addEntry(HKLogEntry entry)
{
    addEntries({std::move(entry)});
}

void HKLogStorage::addEntries(QList<HKLogEntry> _entries)
{
    QMetaObject::invokeMethod(this, [this, _entries = std::move(_entries)]() mutable {
        filter(this->entries() + std::move(_entries));
    });
}

void HKLogStorage::removeEntry(QUuid entryId)
{
    removeEntries({entryId});
}

void HKLogStorage::removeEntries(QList<QUuid> entryIds)
{
    QMetaObject::invokeMethod(this, [this, entryIds = std::move(entryIds)] {
        auto entries = this->entries();

        for (const auto &entryId : entryIds) {
            entries.removeIf([&entryId](const HKLogEntry &entry) {
                return entryId == entry.m_id;
            });
        }

        m_entries = std::move(entries);
    });
}

void HKLogStorage::filter(QList<HKLogEntry> entries)
{
    {
        const auto ageLimit = this->ageLimit();
        if (ageLimit > 0) {
            const auto currentTime = QDateTime::currentDateTime();
            entries.removeIf([&currentTime, ageLimit = std::chrono::hours{ageLimit}](const HKLogEntry &entry) {
                return currentTime - entry.m_timestamp > ageLimit;
            });
        }
    }

    std::ranges::sort(entries);

    {
        const auto limit = this->limit();
        if (limit > 0 && limit <= entries.size()) {
            entries.erase(entries.begin(), entries.end() - limit);
        }
    }

    m_entries = std::move(entries);
}

bool HKLogStorage::fileCheck()
{
    QMutexLocker lock(&m_mutex);

    if (!m_file) {
        Q_UNLIKELY_BRANCH;
        qCDebug(HK_LOGGING, "Log file pointer is null, nothing will be saved.");
        return false;
    } else {
        if (m_file->isOpen()) {
            if (m_file->openMode().testFlag(QFile::ReadWrite)) {
                return true;
            }
            m_file->close();
        } else {
            QFileInfo fileInfo{*m_file};
            auto dir = fileInfo.dir();
            if (!dir.exists()) {
                if (!dir.mkpath("."_L1)) {
                    qCWarning(HK_LOGGING, "Failed to create log directory, nothing will be saved.");
                    return false;
                }
            }
        }

        if (!m_file->open(QFile::ReadWrite)) {
            qCWarning(HK_LOGGING, "Failed to open log file, nothing will be saved: %s (%d).", qPrintable(m_file->errorString()), m_file->error());
            return false;
        }
    }

    return true;
}

void HKLogStorage::read()
{
    if (!fileCheck()) {
        return;
    }

    QList<HKLogEntry> entries;
    {
        QMutexLocker lock(&m_mutex);
        m_file->seek(0);

        QByteArray data = m_file->readAll();
        if (!data.isEmpty()) {
            data = qUncompress(data);
        }
        QDataStream stream(data);
        stream >> entries;
    }

    QMetaObject::invokeMethod(this, [this, entries = std::move(entries)]() mutable {
        m_entries = std::move(entries);
    });
}

void HKLogStorage::write()
{
    if (!fileCheck()) {
        return;
    }

    QMutexLocker lock{&m_mutex};

    m_file->resize(0);
    m_file->seek(0);
    {
        QByteArray data;
        QDataStream stream(&data, QDataStream::WriteOnly);

        QList<HKLogEntry> entries;
        QMetaObject::invokeMethod(
            this,
            [this, &entries] {
                entries = m_entries.value();
            },
            thread() == QThread::currentThread() ? Qt::AutoConnection : Qt::BlockingQueuedConnection);

        stream << entries;
        m_file->write(qCompress(data, 9));
    }
    m_file->flush();
}
