#include <iostream>
#include <cmath>
#include <limits>
#include "PolyLib.h"   // неявне зв'язування: заголовок + .lib

static int g_fail = 0;

static void check(const char* name, bool ok) {
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (!ok) ++g_fail;
}

static bool same(const double* a, const double* b, int n) {
    for (int i = 0; i < n; ++i)
        if (std::fabs(a[i] - b[i]) > 1e-12) return false;
    return true;
}

static void reset(double* out, int& len) {
    for (int i = 0; i < 10; ++i) out[i] = 777.0;
    len = -1;
}

// ===== ТЕСТИ (однакові в обох проєктах) =====
static void run_tests() {
    const double NaN = std::numeric_limits<double>::quiet_NaN();
    double out[10];
    int len;

    std::cout << "--- Нормальна робота ---\n";
    {   // 1 + 2x + 3x^2 + (4 + 5x) = 5 + 7x + 3x^2
        double a[] = { 1, 2, 3 }, b[] = { 4, 5 }, e[] = { 5, 7, 3 };
        reset(out, len);
        int rc = poly_add(a, 3, b, 2, out, 10, &len);
        check("add", rc == POLY_OK && len == 3 && same(out, e, 3));
    }
    {   // (x^2 + 1)(x + 2) = 2 + x + 2x^2 + x^3
        double a[] = { 1, 0, 1 }, b[] = { 2, 1 }, e[] = { 2, 1, 2, 1 };
        reset(out, len);
        int rc = poly_multiply(a, 3, b, 2, out, 10, &len);
        check("multiply", rc == POLY_OK && len == 4 && same(out, e, 4));
    }
    {   // P(2) = 1 + 4 + 12 = 17
        double a[] = { 1, 2, 3 }, r = 0;
        int rc = poly_evaluate(a, 3, 2.0, &r);
        check("evaluate P(2)=17", rc == POLY_OK && std::fabs(r - 17.0) < 1e-12);
    }
    {   // (1 + 2x + 3x^2)' = 2 + 6x
        double a[] = { 1, 2, 3 }, e[] = { 2, 6 };
        reset(out, len);
        int rc = poly_derivative(a, 3, out, 10, &len);
        check("derivative", rc == POLY_OK && len == 2 && same(out, e, 2));
    }
    {   // первісна {2,6} зі сталою 1 = 1 + 2x + 3x^2
        double a[] = { 2, 6 }, e[] = { 1, 2, 3 };
        reset(out, len);
        int rc = poly_integral(a, 2, 1.0, out, 10, &len);
        check("integral", rc == POLY_OK && len == 3 && same(out, e, 3));
    }
    {   // похідна константи = {0}
        double a[] = { 5 };
        reset(out, len);
        int rc = poly_derivative(a, 1, out, 1, &len);
        check("derivative of constant", rc == POLY_OK && len == 1 && out[0] == 0.0);
    }

    std::cout << "\n--- nullptr (очікуємо -1, буфер не змінюється) ---\n";
    {
        double a[] = { 1, 2, 3 }, b[] = { 4, 5 };
        reset(out, len);
        check("add a=nullptr", poly_add(nullptr, 3, b, 2, out, 10, &len) == POLY_ERR_NULL);
        check("add b=nullptr", poly_add(a, 3, nullptr, 2, out, 10, &len) == POLY_ERR_NULL);
        check("add out=nullptr", poly_add(a, 3, b, 2, nullptr, 10, &len) == POLY_ERR_NULL);
        check("add outLen=nullptr", poly_add(a, 3, b, 2, out, 10, nullptr) == POLY_ERR_NULL);
        check("multiply a=nullptr", poly_multiply(nullptr, 3, b, 2, out, 10, &len) == POLY_ERR_NULL);
        check("derivative a=nullptr", poly_derivative(nullptr, 3, out, 10, &len) == POLY_ERR_NULL);
        check("integral out=nullptr", poly_integral(a, 3, 0, nullptr, 10, &len) == POLY_ERR_NULL);
        double r = 0;
        check("evaluate a=nullptr", poly_evaluate(nullptr, 3, 1.0, &r) == POLY_ERR_NULL);
        check("evaluate result=nullptr", poly_evaluate(a, 3, 1.0, nullptr) == POLY_ERR_NULL);
        check("буфер і outLen не змінились", out[0] == 777.0 && len == -1);
    }

    std::cout << "\n--- Некоректний розмір (очікуємо -2) ---\n";
    {
        double a[] = { 1, 2, 3 }, b[] = { 4, 5 };
        reset(out, len);
        check("add na=0", poly_add(a, 0, b, 2, out, 10, &len) == POLY_ERR_SIZE);
        check("add na=-1", poly_add(a, -1, b, 2, out, 10, &len) == POLY_ERR_SIZE);
        check("add nb=0", poly_add(a, 3, b, 0, out, 10, &len) == POLY_ERR_SIZE);
        check("multiply na=0", poly_multiply(a, 0, b, 2, out, 10, &len) == POLY_ERR_SIZE);
        check("derivative n=0", poly_derivative(a, 0, out, 10, &len) == POLY_ERR_SIZE);
        check("integral n=-5", poly_integral(a, -5, 0, out, 10, &len) == POLY_ERR_SIZE);
        double r = 0;
        check("evaluate n=0", poly_evaluate(a, 0, 1.0, &r) == POLY_ERR_SIZE);
        check("буфер не змінився", out[0] == 777.0 && len == -1);
    }

    std::cout << "\n--- Малий буфер (очікуємо -3, нічого не записано) ---\n";
    {
        double a[] = { 1, 2, 3 }, b[] = { 4, 5 };
        reset(out, len);
        check("add outCap=2 (треба 3)", poly_add(a, 3, b, 2, out, 2, &len) == POLY_ERR_BUFFER);
        check("add outCap=0", poly_add(a, 3, b, 2, out, 0, &len) == POLY_ERR_BUFFER);
        check("add outCap=-1", poly_add(a, 3, b, 2, out, -1, &len) == POLY_ERR_BUFFER);
        check("multiply outCap=3 (треба 4)", poly_multiply(a, 3, b, 2, out, 3, &len) == POLY_ERR_BUFFER);
        check("derivative outCap=1 (треба 2)", poly_derivative(a, 3, out, 1, &len) == POLY_ERR_BUFFER);
        double c[] = { 5 };
        check("derivative const outCap=0", poly_derivative(c, 1, out, 0, &len) == POLY_ERR_BUFFER);
        check("integral outCap=3 (треба 4)", poly_integral(a, 3, 0, out, 3, &len) == POLY_ERR_BUFFER);
        check("буфер не змінився", out[0] == 777.0 && out[1] == 777.0 && len == -1);
    }

    std::cout << "\n--- NaN / inf / переповнення (README: поверне POLY_OK) ---\n";
    {
        double a[] = { 1, NaN }, r = 0;
        int rc = poly_evaluate(a, 2, 1.0, &r);
        check("evaluate з NaN: rc=OK, результат NaN", rc == POLY_OK && std::isnan(r));
    }
    {
        double a[] = { 1e308 }, b[] = { 1e308 };
        reset(out, len);
        int rc = poly_multiply(a, 1, b, 1, out, 10, &len);
        check("multiply 1e308*1e308: rc=OK, результат inf", rc == POLY_OK && std::isinf(out[0]));
    }
    {
        double a[] = { 0, 0, 1 }, r = 0;
        int rc = poly_evaluate(a, 3, 1e200, &r);
        check("evaluate x=1e200: rc=OK, результат inf", rc == POLY_OK && std::isinf(r));
    }

    std::cout << "\n--- Інформаційні (контракт не визначає) ---\n";
    {
        double r = 0;
        int rc = poly_evaluate(nullptr, 0, 1.0, &r);
        std::cout << "[INFO] a=nullptr і n=0 одночасно: код " << rc
            << " (-1 = перевірка nullptr першою, -2 = розміру)\n";
    }
    {   
        double a[] = { 1, 2, 3 }, b[] = { 4, 5, 6 };
        int rc = poly_add(a, 3, b, 3, a, 3, &len);
        std::cout << "[INFO] add з out==a: код " << rc << ", a = {"
            << a[0] << ", " << a[1] << ", " << a[2] << "}\n";
    }
}

int main() {
    std::setlocale(LC_ALL, "Ukrainian"); 
    run_tests();
    std::cout << "\nПровалених перевірок: " << g_fail << "\n";
    return g_fail ? 1 : 0;
}