#include "WorkerThread.h"
#include "include/shared/SharedMath.h"
#include <stdexcept>


using DoItFunc = double (*)(int, double, double);

namespace gs {
namespace calc {

WorkerThread::WorkerThread(SafeQueue<Task>& requests,
                           SafeQueue<Task>& results,
                           QObject* parent)
    : QThread(parent)
    , m_requests(requests)
    , m_results(results)
{}

void WorkerThread::run() {
    while (true) {
        Task task;
        m_requests.WaitAndPop(task);

        if (task.IsShutdown()) {
            break;
        }

        const QString threadId = QString("0x%1").arg(reinterpret_cast<quintptr>(QThread::currentThreadId()), 0, 16);
        task.Solve(std::move(threadId));
        m_results.Push(task);

        emit resultReady();
    }
}


} // namespace calc
} // namespace gs

