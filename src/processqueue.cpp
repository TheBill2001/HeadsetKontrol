#include <QDebug>

#include "processqueue.h"

using namespace Qt::Literals::StringLiterals;

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
    for (const auto &child : std::as_const(children))
        if (child->state() != QProcess::NotRunning)
            ++count;
    return count;
}

void ProcessQueue::start()
{
    while (!m_queue.isEmpty() && runningProcess() < 1) {
        QProcess *p_process = m_queue.dequeue();
        m_outputData.clear();

        connect(p_process, &QProcess::readyReadStandardOutput, this, [=]() {
            m_outputData += p_process->readAllStandardOutput();
        });

        connect(p_process, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus != QProcess::QProcess::NormalExit) {
                auto error = QString::fromUtf8(p_process->readAllStandardError());
                if (!error.isEmpty()) {
                    error = u"headsetcontrol exit with code %1: %2."_s.arg(QString::number(exitCode), error);
                } else {
                    error = u"headsetcontrol exit with code %1."_s.arg(QString::number(exitCode));
                }
                qWarning().noquote() << error;
                Q_EMIT errorOccurred(error);
                Q_EMIT outputReady({});
            } else {
                Q_EMIT outputReady(m_outputData);
            }

            p_process->deleteLater();
        });

        connect(p_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
            Q_UNUSED(error)
            auto errorStr = u"headsetcontrol process error: %1."_s.arg(p_process->errorString());
            qWarning().noquote() << errorStr;
            Q_EMIT errorOccurred(errorStr);
            Q_EMIT outputReady({});
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
    for (const auto &child : std::as_const(children)) {
        child->terminate();
    }
    setRunning(false);
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
