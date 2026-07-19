#include <QLibrary>
#include <string>

using DoItFunc = double (*)(int typeWork, double a, double b);

namespace gs {
namespace calc {

class ExternalEngineMgr {
public:
    static ExternalEngineMgr& instance()
    {
        static ExternalEngineMgr instance;
        return instance;
    }

    explicit ExternalEngineMgr(const ExternalEngineMgr&) = delete;
    ExternalEngineMgr& operator=(const ExternalEngineMgr&) = delete;

    DoItFunc GetDoItFunc() const { return m_doItFunc; }
    bool IsValid() const { return m_doItFunc != nullptr && m_errorMsg.empty(); }
    const std::string & GetErrorMsg() const { return m_errorMsg; }

private:
    ExternalEngineMgr();
    ~ExternalEngineMgr();

    QLibrary m_lib;
    DoItFunc m_doItFunc = nullptr;

    std::string m_errorMsg;
};

} // namespace calc
} // namespace gs
