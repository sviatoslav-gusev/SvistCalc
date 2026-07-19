#include "Application.h"
#include <QStringBuilder>

namespace gs {
namespace calc {

Application::Application(size_t threadCount, QObject* parent)
    : QObject(parent)
{
    // Compose threadpool with task workers
    m_workers.reserve(threadCount);
    for (size_t i = 0; i < threadCount; ++i)
    {
        auto worker = std::make_unique<WorkerThread>(m_requests, m_results);

        connect(worker.get(), &WorkerThread::resultReady,
                this,         &Application::checkResults);

        worker->start(); // Start QThread
        m_workers.emplace_back(std::move(worker));
    }
}

Application::~Application()
{
    for (size_t i = 0; i < m_workers.size(); ++i) {
        m_requests.Push(Task::CreatePoisonPill());
    }

    // Wait each thread
    for (std::unique_ptr<WorkerThread> & worker : m_workers) {
        worker->wait();
    }
}

void Application::calculate(const QString& expression, int engineTypeVal, int delay_s)
{
    const EngineType type = (engineTypeVal == 0) // TODO: how to do more explicit
                          ? EngineType::Inner
                          : EngineType::OuterLib;

    Task task(expression, type, delay_s);


    if (task.TryParse()) {
        m_requests.Push(task);

        const QString msg = QString("[Task %1] created: %2")
                            .arg(task.GetId())
                            .arg(expression);
        emit requestAdded(msg);
    }
    else {
        const QString msg = QString("[Task %1] cannot parse '%2': %3")
                            .arg(task.GetId())
                            .arg(expression)
                            .arg(QString::fromStdString(task.GetErrorMsg()));
        emit parseError(msg);
    }
}

void Application::checkResults()
{
    Task completedTask;

    while (m_results.TryPop(completedTask))
    {
        if (completedTask.IsSuccess()) {
            // Blue
            const QString msg = QString("[Task %1] Thread %2 get result: %3 = %4")
                                .arg(completedTask.GetId())
                                .arg(completedTask.GetThreadId())
                                .arg(completedTask.GetSource())
                                .arg(completedTask.GetResult());
            emit resultSuccess(msg);
        }
        else {
            // Red
            const QString msg = QString("[Task %1] Thread %2 get error: %3")
                                .arg(completedTask.GetId())
                                .arg(completedTask.GetThreadId())
                                .arg(QString::fromStdString(completedTask.GetErrorMsg()));
            emit resultError(msg);
        }
    }
}

} // namespace calc
} // namespace gs
