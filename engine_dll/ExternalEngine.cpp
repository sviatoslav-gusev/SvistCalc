#include "../shared/SharedMath.h"

#if defined(_WIN32)
    #if defined(BUILD_EXTERNAL_ENGINE)
        #define EXPORT_API __declspec(dllexport)
    #else
        #define EXPORT_API __declspec(dllimport)
    #endif
#else
    #define EXPORT_API __attribute__((visibility("default")))
#endif



extern "C" {

EXPORT_API double DoIt(int TypeWork, double OperandA, double OperandB) // noexcept(false)
{
    return gs::calc::shared::Calculate(TypeWork, OperandA, OperandB);
}

} // extern "C"
