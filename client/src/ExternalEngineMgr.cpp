#include "ExternalEngineMgr.h"
#include <stdexcept>

namespace gs {
namespace calc {

ExternalEngineMgr::ExternalEngineMgr() {
    m_lib.setFileName("libExternalEngine");

    if (!m_lib.load()) {
        m_errorMsg = m_lib.errorString().toStdString();
        return;
    }

    m_doItFunc = reinterpret_cast<DoItFunc>(m_lib.resolve("DoIt"));
    if (!m_doItFunc) {
        m_errorMsg = "Cannot find 'DoIt' inside the lib!";
    }
}

ExternalEngineMgr::~ExternalEngineMgr() {
    if (m_lib.isLoaded()) {
        m_lib.unload();
    }
}

} // namespace calc
} // namespace gs
