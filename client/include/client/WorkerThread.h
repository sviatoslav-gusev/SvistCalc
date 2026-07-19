#pragma once

#include <QThread>
#include <QLibrary>
#include "SafeQueue.h"
#include "Task.h"

namespace gs {
namespace calc {

class WorkerThread : public QThread {
    Q_OBJECT

signals:
    void resultReady();

public:
    WorkerThread(SafeQueue<Task>& requests,
                 SafeQueue<Task>& results,
                 QObject* parent = nullptr);

protected:
    void run() override;

private:
    SafeQueue<Task>& m_requests;
    SafeQueue<Task>& m_results;
};

} // namespace calc
} // namespace gs
