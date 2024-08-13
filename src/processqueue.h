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
    void outputReady(const QByteArray &data);
    void runningChanged(bool running);
    void errorOccurred(const QString &error);

private:
    QQueue<QProcess *> m_queue;
    bool m_running;

    QByteArray m_outputData;

    void setRunning(bool running);
};

#endif
