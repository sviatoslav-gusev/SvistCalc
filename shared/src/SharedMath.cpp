#include "SharedMath.h"
#include <cmath>
#include <stdexcept>

double gs::calc::shared::Calculate(int typeWork, double a, double b) // noexcept(false)
{
    auto ValidateOrThrow = [](double val, const char* label) {
        if (std::isnan(val))
            throw std::logic_error(std::string(label) + " is Not a Number (NaN)");
        if (std::isinf(val))
            throw std::logic_error(std::string(label) + " overflow (Infinity)");
    };

    ValidateOrThrow(a, "Operand A");
    ValidateOrThrow(b, "Operand B");

    constexpr double EPS = 1e-9;
    double result = 0.0;

    switch (typeWork) {
    case '+': { result = a + b; break; }
    case '-': { result = a - b; break; }
    case '*': { result = a * b; break; }
    case '/': {
        if (abs(b) < EPS) {
            throw std::logic_error("Division by zero");
        }
        result = a / b;
        break; }
    default:
        throw std::logic_error("Unknown operation");
    }

    ValidateOrThrow(result, "Action result");

    return result;
}
