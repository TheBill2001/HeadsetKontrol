#include <QDebug>

#include "processqueue.h"

ProcessQueue::ProcessQueue(QObject *parent)
    : QObject{parent}
    , m_running{false}
{
}

ProcessQueue::~ProcessQueue()
{
    clear();
    stop();
}

void ProcessQueue::addProcess(const QString &processPath, const QStringList &arguments)
{
    QProcess *p_process = new QProcess(this);
    p_process->setProgram(processPath);
    p_process->setArguments(arguments);
    m_queue.enqueue(p_process);
}

bool ProcessQueue::isRunning() const
{
    return m_running;
}

int ProcessQueue::runningProcess()
{
    int count = 0;
    auto children = findChildren<QProcess *>();
    for (const auto &child : children)
        if (child->state() != QProcess::NotRunning)
            ++count;
    return count;
}

void ProcessQueue::start()
{
    while (!m_queue.isEmpty() && runningProcess() < 1) {
        QProcess *p_process = m_queue.dequeue();
        connect(p_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus != QProcess::NormalExit) {
                qCritical().nospace() << "headsetcontrol crashed with argument: " << p_process->arguments() << "; exit code: " << exitCode;
            } else {
                auto processOutput = QTextStream(p_process);
                Q_EMIT outputReady(processOutput.readAll(), p_process->arguments());
            }
            p_process->deleteLater();
        });
        connect(p_process, &QProcess::destroyed, this, [=]() {
            if (m_queue.isEmpty()) {
                stop();
            } else {
                start();
            }
        });

        setRunning(true);
        p_process->start();
    }
}

void ProcessQueue::stop()
{
    auto children = findChildren<QProcess *>();
    for (const auto &child : children) {
        child->terminate();
    }
    setRunning(false);
    Q_EMIT finished();
}

void ProcessQueue::clear()
{
    m_queue.clear();
}

void ProcessQueue::setRunning(bool running)
{
    if (running != m_running) {
        m_running = running;
        Q_EMIT runningChanged(m_running);
    }
}
