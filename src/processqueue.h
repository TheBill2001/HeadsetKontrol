#ifndef PROCESSQUEUE_H
#define PROCESSQUEUE_H

#include <QProcess>
#include <QQueue>

class ProcessQueue : public QObject
{
    Q_OBJECT
public:
    ProcessQueue(QObject *parent = nullptr);
    ~ProcessQueue();

    void addProcess(const QString &processPath, const QStringList &arguments);
    bool isRunning() const;
    int runningProcess();

public Q_SLOTS:
    void start();
    void stop();
    void clear();

Q_SIGNALS:
    void outputReady(const QString &output, const QStringList &arguments);
    void runningChanged(bool running);
    void finished();

private:
    QQueue<QProcess *> m_queue;
    bool m_running;

    void setRunning(bool running);
};

#endif
