#pragma once

#include <QObject>
#include <QTimer>
#include <vector>
#include <memory>
#include <QString>

#include "SafeQueue.h"
#include "Task.h"
#include "WorkerThread.h"

namespace gs {
namespace calc {

class Application : public QObject {
    Q_OBJECT
public:
    explicit Application(size_t threadCount = THREAD_COUNT, QObject* parent = nullptr);
    ~Application() override;

    // Adding task from QML
    Q_INVOKABLE void calculate(const QString & expression,
                               int engineTypeVal, //TODO: how to keep invokable but remove int-vs-int risk
                               int delay_s);

signals:
    void requestAdded(const QString& msg);   // Green
    void resultSuccess(const QString& msg);  // Blue
    void resultError(const QString& msg);    // Red
    void parseError(const QString& msg);     // Red

    void resultReady();

private slots:
    void checkResults();

private:
    static constexpr size_t THREAD_COUNT = 4;

    SafeQueue<Task> m_requests;
    SafeQueue<Task> m_results;

    std::vector<std::unique_ptr<WorkerThread>> m_workers;
};

} // namespace calc
} // namespace gs
