// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hklogstorage.hpp"

#include <QTemporaryFile>
#include <QTest>

using namespace Qt::StringLiterals;

class LogStorageTest : public QObject
{
    Q_OBJECT

    QTemporaryFile m_file;

    QList<HKLogEntry> m_entries{
        HKLogEntry{
            u"Entry 1"_s,
            u"Message 1"_s,
        },
        HKLogEntry{
            u"Entry 2"_s,
            u"Message 2"_s,
        },
        HKLogEntry{
            u"Entry 3"_s,
            u"Message 3"_s,
            u"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque tellus justo, imperdiet non viverra at, faucibus at nisl. Nullam lobortis vel lorem vel cursus."_s},
        HKLogEntry{u"Entry 4"_s, u"Message 4"_s, QDateTime::fromSecsSinceEpoch(1)},
        HKLogEntry{u"Entry 5"_s, u"Message 5"_s, QDateTime::fromSecsSinceEpoch(0)},
    };
    QList<HKLogEntry> m_sortedEntries = m_entries;
    QList<HKLogEntry> m_filteredEntries{m_entries[1], m_entries[2]};

public:
    explicit LogStorageTest(QObject *parent = nullptr)
        : QObject{parent}
    {
        std::ranges::sort(m_sortedEntries);
    }

private Q_SLOTS:
    void initTestCase()
    {
        QVERIFY2(m_file.open(), "Failed to open temporary file.");
        qInfo() << "Temporary file:" << m_file.fileName();
    }

    void writeLog()
    {
        HKLogStorage storage{&m_file};
        for (const auto &entry : std::as_const(m_entries)) {
            storage.addEntry(entry);
        }

        QCOMPARE_EQ(storage.entries(), m_sortedEntries);
    }

    void readLog()
    {
        HKLogStorage storage{&m_file};
        QCOMPARE_EQ(storage.entries(), m_sortedEntries);
    }

    void limitLog()
    {
        HKLogStorage storage{&m_file};
        storage.setAgeLimit(1);
        storage.setLimit(2);
        QCOMPARE_EQ(storage.entries(), m_filteredEntries);
    }
};

QTEST_MAIN(LogStorageTest)
#include "log-storage.moc"
