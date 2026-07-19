#pragma once

#include <QString>
#include <atomic>
#include <string>
#include <vector>

namespace gs {
namespace calc {

enum class EngineType {
    Inner,
    OuterLib
};

class Task
{
    struct Action {
        Action(int op, double b) : op(op), b(b) {}

        int op;
        double b;
    };

public:
    Task() = default;
    explicit Task(QString source,
                  EngineType engineType = EngineType::Inner,
                  size_t delay_s = 1);

    bool TryParse();
    bool Solve(QString threadId);

    QString GetSource() const { return m_source; }
    double GetResult() const { return m_result; }
    bool IsSuccess() const { return m_success; }
    const std::string & GetErrorMsg() const { return m_errorMsg; }
    size_t GetId() const { return m_taskId; }
    QString GetThreadId() const { return m_threadId; }

    // Poison pill
    bool IsShutdown() const { return m_isShutdown; }
    static Task CreatePoisonPill();

private:
    size_t m_taskId {0};

    QString m_source;
    EngineType m_engineType = EngineType::Inner;
    size_t m_delay_s {0};

    double m_firstOperand {0.0};
    std::vector<Action> m_actions;

    double m_result {0.0};
    std::string m_errorMsg;
    bool m_success = true;

    QString m_threadId;

    // Poison pill
    bool m_isShutdown = false;

private:
    static std::atomic<size_t> m_idCount;
};


} // namespace calc
} // namespace gs
