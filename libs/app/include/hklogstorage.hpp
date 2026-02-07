// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKLOGSTORAGE_HPP
#define HKLOGSTORAGE_HPP

#include "hkapp_export.hpp"

#include <QDateTime>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QObjectComputedProperty>
#include <QPointer>
#include <QTimer>
#include <QUuid>

class HKHeadset;
class HKHeadsetError;
class QFile;
class QQuickWindow;

class HKAPP_EXPORT HKLogEntry
{
    Q_GADGET
    Q_PROPERTY(QUuid id READ id FINAL)
    Q_PROPERTY(QString idString READ idString FINAL)
    Q_PROPERTY(QString title READ title FINAL)
    Q_PROPERTY(QString message READ message FINAL)
    Q_PROPERTY(QString details READ details FINAL)
    Q_PROPERTY(QDateTime timestamp READ timestamp FINAL)

    friend class HKLogStorage;
    friend std::weak_ordering operator<=>(const HKLogEntry &lhs, const HKLogEntry &rhs);
    friend bool operator==(const HKLogEntry &lhs, const HKLogEntry &rhs);
    friend QDataStream &operator<<(QDataStream &stream, const HKLogEntry &entry);
    friend QDataStream &operator>>(QDataStream &stream, HKLogEntry &entry);

public:
    HKLogEntry() = default;

    HKLogEntry(QUuid entryId, QString title, QString message, QString details, QDateTime timestamp = QDateTime::currentDateTime())
        : m_id{entryId}
        , m_title{std::move(title)}
        , m_message{std::move(message)}
        , m_details{std::move(details)}
        , m_timestamp{std::move(timestamp)}
    {
    }

    HKLogEntry(QUuid entryId, QString title, QString message, QDateTime timestamp = QDateTime::currentDateTime())
        : m_id{entryId}
        , m_title{std::move(title)}
        , m_message{std::move(message)}
        , m_timestamp{std::move(timestamp)}
    {
    }

    HKLogEntry(QString title, QString message, QString details, QDateTime timestamp = QDateTime::currentDateTime())
        : m_id{QUuid::createUuid()}
        , m_title{std::move(title)}
        , m_message{std::move(message)}
        , m_details{std::move(details)}
        , m_timestamp{std::move(timestamp)}
    {
    }

    HKLogEntry(QString title, QString message, QDateTime timestamp = QDateTime::currentDateTime())
        : m_id{QUuid::createUuid()}
        , m_title{std::move(title)}
        , m_message{std::move(message)}
        , m_timestamp{std::move(timestamp)}
    {
    }

    [[nodiscard]] QUuid id() const noexcept
    {
        return m_id;
    }

    [[nodiscard]] QString idString() const noexcept
    {
        return m_id.toString(QUuid::WithoutBraces);
    }

    [[nodiscard]] QString title() const noexcept
    {
        return m_title;
    }

    [[nodiscard]] QString message() const noexcept
    {
        return m_message;
    }

    [[nodiscard]] QString details() const noexcept
    {
        return m_details;
    }

    [[nodiscard]] QDateTime timestamp() const noexcept
    {
        return m_timestamp;
    }

private:
    QUuid m_id;
    QString m_title;
    QString m_message;
    QString m_details;
    QDateTime m_timestamp{QDateTime::currentDateTime()};
};

Q_DECLARE_TYPEINFO(HKLogEntry, Q_RELOCATABLE_TYPE);

[[nodiscard]] HKAPP_EXPORT inline std::weak_ordering operator<=>(const HKLogEntry &lhs, const HKLogEntry &rhs)
{
    return lhs.m_timestamp <=> rhs.m_timestamp;
};

[[nodiscard]] HKAPP_EXPORT inline bool operator==(const HKLogEntry &lhs, const HKLogEntry &rhs)
{
    return lhs.m_id == rhs.m_id && lhs.m_title == rhs.m_title && lhs.m_message == rhs.m_message && lhs.m_details == rhs.m_details
        && lhs.m_timestamp == rhs.m_timestamp;
}

[[maybe_unused]] HKAPP_EXPORT QDataStream &operator<<(QDataStream &stream, const HKLogEntry &entry);
[[maybe_unused]] HKAPP_EXPORT QDataStream &operator>>(QDataStream &stream, HKLogEntry &entry);

class HKAPP_EXPORT HKLogStorage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<HKLogEntry> entries READ entries BINDABLE bindableEntries NOTIFY entriesChanged FINAL)
    // NOLINTNEXTLINE(clazy-qproperty-without-notify)
    Q_PROPERTY(qsizetype entriesCount READ entriesCount BINDABLE bindableEntriesCount STORED false FINAL)
    Q_PROPERTY(int limit READ limit WRITE setLimit BINDABLE bindableLimit NOTIFY limitChanged FINAL)
    Q_PROPERTY(int ageLimit READ ageLimit WRITE setAgeLimit BINDABLE bindableAgeLimit NOTIFY ageLimitChanged FINAL)
public:
    explicit HKLogStorage(const QString &filePath, QObject *parent = nullptr);
    explicit HKLogStorage(QFile *file, QObject *parent = nullptr);
    ~HKLogStorage() override;

    [[nodiscard]] static HKLogStorage *instance();

    [[nodiscard]] QBindable<QList<HKLogEntry>> bindableEntries() const
    {
        return &m_entries;
    }

    [[nodiscard]] QList<HKLogEntry> entries() const
    {
        return m_entries.value();
    }

    [[nodiscard]] QBindable<qsizetype> bindableEntriesCount() const
    {
        return &m_entriesCount;
    }

    [[nodiscard]] qsizetype entriesCount() const
    {
        return m_entriesCount.value();
    }

    [[nodiscard]] QBindable<int> bindableLimit()
    {
        return &m_limit;
    }

    [[nodiscard]] int limit()
    {
        return m_limit.value();
    }

    void setLimit(int value)
    {
        m_limit = value;
    }

    [[nodiscard]] QBindable<int> bindableAgeLimit()
    {
        return &m_ageLimit;
    }

    [[nodiscard]] int ageLimit()
    {
        return m_ageLimit.value();
    }

    void setAgeLimit(int value)
    {
        m_ageLimit = value;
    }

public Q_SLOTS:
    void addEntry(HKLogEntry entry);
    void addEntries(QList<HKLogEntry> entries);
    void removeEntry(QUuid entryId);
    void removeEntries(QList<QUuid> entryIds);

Q_SIGNALS:
    void entriesChanged(const QList<HKLogEntry> &);
    void limitChanged(int);
    void ageLimitChanged(int);
    void showEntryRequested(const HKLogEntry &);

private:
    const QPointer<QFile> m_file;
    QTimer m_debounceTimer;
    QMutex m_mutex;

    Q_OBJECT_BINDABLE_PROPERTY(HKLogStorage, QList<HKLogEntry>, m_entries, &HKLogStorage::entriesChanged)

    [[nodiscard]] qsizetype calculateEntriesCount() const
    {
        return entries().size();
    }
    Q_OBJECT_COMPUTED_PROPERTY(HKLogStorage, qsizetype, m_entriesCount, &HKLogStorage::calculateEntriesCount)

    Q_OBJECT_BINDABLE_PROPERTY(HKLogStorage, int, m_limit, &HKLogStorage::limitChanged)
    Q_OBJECT_BINDABLE_PROPERTY(HKLogStorage, int, m_ageLimit, &HKLogStorage::ageLimitChanged)

    void filter(QList<HKLogEntry> entries);
    [[nodiscard]] bool fileCheck();
    void read();
    void write();
};

#endif // HKLOGSTORAGE_HPP
