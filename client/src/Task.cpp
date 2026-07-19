#include "Task.h"
#include <QThread>
#include "ExternalEngineMgr.h"
#include "include/shared/SharedMath.h"

//using DoItFunc = double (*)(int typeWork, double a, double b);

namespace gs {
namespace calc {

std::atomic<size_t> Task::m_idCount {0};

Task::Task(QString source,
           EngineType engineType,
           size_t delay_s)
    : m_taskId(++m_idCount)
    , m_source(source)
    , m_engineType(engineType)
    , m_delay_s(delay_s)
{}

bool Task::TryParse()
{
    QString s = m_source;
    s.remove(' ');
    s.replace(',', '.');

    if (s.isEmpty()) {
        m_errorMsg = "Empty task line";
        return false;
    }

    // Search 1st operator (+, -, *, /)
    // "for (int i = 1;"  to skip posible negative num
    int nextOpPos = -1;
    for (int i = 1; i < s.length(); ++i) {
        QChar c = s[i];
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            nextOpPos = i;
            break;
        }
    }

    // Extract 1st num
    const QString firstNumStr = (nextOpPos == -1)
                                ? s
                                : s.left(nextOpPos);
    bool ok = false;
    m_firstOperand = firstNumStr.toDouble(&ok);
    if (!ok) {
        m_errorMsg = "Cannot parse 1st operand: " + firstNumStr.toStdString();
        return false;
    }

    // Single num case
    if (nextOpPos == -1) {
        return true;
    }

    // Parse actions
    int pos = nextOpPos;
    while (pos < s.length())
    {
        const QChar op = s[pos];
        ++pos;

        // Scroll until next op or EOL
        int endNumPos = -1;
        for (int i = pos; i < s.length(); ++i)
        {
            const QChar c = s[i];
            if (c == '+' || c == '-' || c == '*' || c == '/') {
                endNumPos = i;
                break;
            }
        }

        // Extract number
        const QString numStr = (endNumPos == -1)
                              ? s.mid(pos)
                              : s.mid(pos, endNumPos - pos);
        const double val = numStr.toDouble(&ok);
        if (!ok) {
            m_errorMsg = "Cannot parse number " + numStr.toStdString() +
                         " after operator " + op.toLatin1();
            return false;
        }

        // int + double
        m_actions.emplace_back(op.toLatin1(), val);

        if (endNumPos == -1) {
            break;
        }
        pos = endNumPos;
    }

    return true;
}

bool Task::Solve(QString threadId) {

    //  Moved to :124
    //    if (m_delay_s > 0) {
    //        QThread::sleep(m_delay_s);
    //    }
    m_threadId = std::move(threadId);

    double currentResult = m_firstOperand;
    DoItFunc externalDoIt = nullptr;

    if (m_engineType == EngineType::OuterLib) {
        if (!ExternalEngineMgr::instance().IsValid()) {
            m_success = false;
            m_errorMsg = ExternalEngineMgr::instance().GetErrorMsg();
            return m_success;
        }
        externalDoIt = ExternalEngineMgr::instance().GetDoItFunc();
    }

    try {
        for (const Task::Action & action : m_actions)
        {
            // Moved here to have more flexible timing in queues
            if (m_delay_s > 0) {
                QThread::sleep(m_delay_s);
            }

            currentResult = (m_engineType == EngineType::OuterLib && externalDoIt)
                          ? externalDoIt(action.op, currentResult, action.b)
                          : shared::Calculate(action.op, currentResult, action.b);
        }
        m_result = currentResult;
    }
    catch (const std::logic_error& e) {
        m_success = false;
        m_errorMsg = e.what();
    }
    catch (...) {
        m_success = false;
        m_errorMsg = "Unknown calculation error!";
    }

    return m_success;
}

Task Task::CreatePoisonPill()
{
    Task t;
    t.m_isShutdown = true;
    return t;
}

} // namespace calc
} // namespace gs
