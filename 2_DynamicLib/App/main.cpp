#include <iostream>
#include "Interpolation.h"

// Друкує результат або код помилки
void report(const char* name, int rc, double value) {
    std::cout << name << ": ";
    if (rc == INTERP_OK) std::cout << value << "\n";
    else                 std::cout << "ERROR " << rc << "\n";
}

int main() {
    // Точки на параболі y = x^2
    double x[] = { 0.0, 1.0, 2.0, 3.0 };
    double y[] = { 0.0, 1.0, 4.0, 9.0 };
    double r = 0.0;
    int rc = 0;

    // Спочатку виклик (rc), потім друк: порядок гарантований
    rc = linearInterpolation(x, y, 4, 1.5, &r);
    report("linear(1.5)  ", rc, r);

    rc = lagrange(x, y, 4, 1.5, &r);
    report("lagrange(1.5)", rc, r);

    rc = interpolate(x, y, 4, 1.5, INTERP_METHOD_LAGRANGE, &r);
    report("interpolate  ", rc, r);

    rc = interpolationError(x, y, 4, 1.5, INTERP_METHOD_LINEAR, 2.25, &r);
    report("error(linear)", rc, r);

    // Граничні випадки
    rc = lagrange(nullptr, y, 4, 1.5, &r);
    report("nullptr x    ", rc, r);

    rc = lagrange(x, y, 1, 1.5, &r);
    report("n = 1        ", rc, r);

    rc = linearInterpolation(x, y, 4, 10.0, &r);
    report("out of range ", rc, r);

    return 0;
}