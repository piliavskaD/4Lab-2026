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

    report("linear(1.5)  ", linearInterpolation(x, y, 4, 1.5, &r), r);
    report("lagrange(1.5)", lagrange(x, y, 4, 1.5, &r), r);
    report("interpolate  ", interpolate(x, y, 4, 1.5, INTERP_METHOD_LAGRANGE, &r), r);
    report("error(linear)", interpolationError(x, y, 4, 1.5, INTERP_METHOD_LINEAR, 2.25, &r), r);

    // Граничні випадки
    report("nullptr x    ", lagrange(nullptr, y, 4, 1.5, &r), r);
    report("n = 1        ", lagrange(x, y, 1, 1.5, &r), r);
    report("out of range ", linearInterpolation(x, y, 4, 10.0, &r), r);
    return 0;
}