#include "Interpolation.h"
#include <cmath>

namespace {

    // Базова перевірка вхідних даних
    int validateCommon(const double* x, const double* y, int n,
        double xq, const double* result) {
        if (!x || !y || !result) return INTERP_ERR_NULL;
        if (n < 2 || n > INTERP_MAX_N) return INTERP_ERR_SIZE;
        if (!std::isfinite(xq)) return INTERP_ERR_NOT_FINITE;
        for (int i = 0; i < n; ++i) {
            if (!std::isfinite(x[i]) || !std::isfinite(y[i]))
                return INTERP_ERR_NOT_FINITE;
        }
        return INTERP_OK;
    }

} // namespace

int linearInterpolation(const double* x, const double* y, int n,
    double xq, double* result) {
    int rc = validateCommon(x, y, n, xq, result);
    if (rc != INTERP_OK) return rc;

    // x має строго зростати
    for (int i = 1; i < n; ++i) {
        if (x[i] == x[i - 1]) return INTERP_ERR_DUPLICATE_X;
        if (x[i] < x[i - 1])  return INTERP_ERR_NOT_SORTED;
    }
    // Екстраполяції немає
    if (xq < x[0] || xq > x[n - 1]) return INTERP_ERR_OUT_OF_RANGE;

    // Шукаємо відрізок, що містить xq
    int i = 0;
    while (i < n - 2 && xq > x[i + 1]) ++i;

    double t = (xq - x[i]) / (x[i + 1] - x[i]);
    double value = y[i] + t * (y[i + 1] - y[i]);
    if (!std::isfinite(value)) return INTERP_ERR_OVERFLOW;

    *result = value;
    return INTERP_OK;
}

int lagrange(const double* x, const double* y, int n,
    double xq, double* result) {
    int rc = validateCommon(x, y, n, xq, result);
    if (rc != INTERP_OK) return rc;

    // Перевірка дублікатів x
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (x[i] == x[j]) return INTERP_ERR_DUPLICATE_X;

    // P(xq) = sum y[i] * prod (xq - x[j]) / (x[i] - x[j])
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double term = y[i];
        for (int j = 0; j < n; ++j) {
            if (j == i) continue;
            term *= (xq - x[j]) / (x[i] - x[j]);
        }
        sum += term;
    }
    if (!std::isfinite(sum)) return INTERP_ERR_OVERFLOW;

    *result = sum;
    return INTERP_OK;
}

int interpolate(const double* x, const double* y, int n,
    double xq, int method, double* result) {
    switch (method) {
    case INTERP_METHOD_LINEAR:   return linearInterpolation(x, y, n, xq, result);
    case INTERP_METHOD_LAGRANGE: return lagrange(x, y, n, xq, result);
    default:                     return INTERP_ERR_BAD_METHOD;
    }
}

int interpolationError(const double* x, const double* y, int n,
    double xq, int method, double exactValue,
    double* result) {
    if (!result) return INTERP_ERR_NULL;
    if (!std::isfinite(exactValue)) return INTERP_ERR_NOT_FINITE;

    double approx = 0.0;
    int rc = interpolate(x, y, n, xq, method, &approx);
    if (rc != INTERP_OK) return rc;

    *result = std::fabs(exactValue - approx);
    return INTERP_OK;
}