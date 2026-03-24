#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "polymorph.h"
#include "tests.h"
#include "lineal_form.h"
#include <locale.h>

int totalTests = 0;
int passedTests = 0;

#define EPS 1e-9
void check_double(double expected, double got)
{
    totalTests++;
    if (fabs(expected - got) < EPS) 
        passedTests++;
}

void check_int(int expected, int got)
{
    totalTests++;
    if (expected == got) 
        passedTests++;
}

void test_int_plus()
{
    int err_code;

    TypeInfo* type_info1 = get_int_type_info();
    TypeInfo* type_info2 = get_int_type_info();

    int _a[3] = {1,2,3};
    int _b[4] = {5,6,7};

    Linear_form* a = create (_a, 3, type_info1, &err_code);
    Linear_form* b = create (_b, 3, type_info2, &err_code);
    Linear_form* c = plus(a, b, &err_code);

    int* c_coeffs = (int*) c->coeffs;
    check_int(c_coeffs[0], 6);
    check_int(c_coeffs[1], 8);
    check_int(c_coeffs[2], 10);

    destroy (a);
    destroy (b);
    destroy (c);
}

void test_int_minus()
{
    int err_code;

    TypeInfo* type_info1 = get_int_type_info();
    TypeInfo* type_info2 = get_int_type_info();

    int _a[3] = {1,2,3};
    int _b[3] = {5,6,7};

    Linear_form* a = create (_a, 3, type_info1, &err_code);
    Linear_form* b = create (_b, 3, type_info2, &err_code);
    Linear_form* c = minus(a, b, &err_code);

    int* c_coeffs = (int*) c->coeffs;
    check_int(c_coeffs[0], -4);
    check_int(c_coeffs[1], -4);
    check_int(c_coeffs[2], -4);

    destroy (a);
    destroy (b);
    destroy (c);
}

void test_double_plus()
{
    int err_code;

    TypeInfo* type_info1 = get_double_type_info();
    TypeInfo* type_info2 = get_double_type_info();

    double _a[3] = {1.5,2.5,3.5};
    double _b[3] = {5.0,6.0,7.0};

    Linear_form* a = create (_a, 3, type_info1, &err_code);
    Linear_form* b = create (_b, 3, type_info2, &err_code);
    Linear_form* c = plus(a, b, &err_code);

    double* c_coeffs = (double*) c->coeffs;
    check_double(c_coeffs[0], 6.5);
    check_double(c_coeffs[1], 8.5);
    check_double(c_coeffs[2], 10.5);

    destroy (a);
    destroy (b);
    destroy (c);
}

void test_double_minus()
{
    int err_code;

    TypeInfo* type_info1 = get_double_type_info();
    TypeInfo* type_info2 = get_double_type_info();

    double _a[3] = {1.5,2.5,3.5};
    double _b[3] = {5.0,6.0,7.0};

    Linear_form* a = create (_a, 3, type_info1, &err_code);
    Linear_form* b = create (_b, 3, type_info2, &err_code);
    Linear_form* c = minus(a, b, &err_code);

    double* c_coeffs = (double*) c->coeffs;
    check_double(c_coeffs[0], -3.5);
    check_double(c_coeffs[1], -3.5);
    check_double(c_coeffs[2], -3.5);

    destroy (a);
    destroy (b);
    destroy (c);
}

void test_double_mult()
{
    int err_code;

    TypeInfo* type_info = get_double_type_info();
    double _a[3] = {1.,2.,3.};
    double coef = 1.5;

    Linear_form* a = create (_a, 3, type_info, &err_code);
    Linear_form* c = mult(a, &coef, &err_code);

    double* c_coeffs = (double*) c->coeffs;
    check_double(c_coeffs[0], 1.5);
    check_double(c_coeffs[1], 3.0);
    check_double(c_coeffs[2], 4.5);

    destroy (a);
    destroy (c);
}

void test_int_mult()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {1,2,3};
    int coef = 2;

    Linear_form* a = create (_a, 3, type_info, &err_code);
    Linear_form* c = mult(a, &coef, &err_code);

    int* c_coeffs = (int*) c->coeffs;
    check_int(c_coeffs[0], 2);
    check_int(c_coeffs[1], 4);
    check_int(c_coeffs[2], 6);

    destroy (a);
    destroy (c);
}

void test_create_int()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {1,2,3};
    Linear_form* a = create (_a, 3, type_info, &err_code);

    if (a != NULL)
    {
        totalTests++;
        passedTests++;

        check_int (a->size, 3);
        check_int (a->type_info->size,  type_info->size);

        int* b = (int*) a->coeffs;
        check_int (b[0], 1);
        check_int (b[1], 2);
        check_int (b[2], 3);
    }
    else
    {
        totalTests++;
    }
    destroy (a);
}

void test_destroy()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {1,2,3};

    Linear_form* a = create(_a, 3, type_info, &err_code);

    destroy (a);
    totalTests++;

    if (a == NULL)
        passedTests++;
}


void test_create_double()
{
    int err_code;

    TypeInfo* type_info = get_double_type_info();
    double _a[3] = {1.,2.,3.};
    Linear_form* a = create (_a, 3, type_info, &err_code);

    if (a != NULL)
    {
        totalTests++;
        passedTests++;

        check_double (a->size, 3);
        check_int (a->type_info->size,  type_info->size);

        double* b = (double*) a->coeffs;
        check_double (b[0], 1.);
        check_double (b[1], 2.);
        check_double (b[2], 3.);
    }
    else
    {
        totalTests++;
    }

    destroy (a);
}

void test_calculate_int()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {1,2,3};
    Linear_form* a = create (_a, 3, type_info, &err_code);

    int x = 2;
    int result = Calculate_int (a, x, &err_code);
    check_int (result, 11);
    destroy (a);
}

void test_calculate_double()
{
    int err_code;

    TypeInfo* type_info = get_double_type_info();
    double _a[3] = {1.5,2.5,3.5};
    Linear_form* a = create (_a, 3, type_info, &err_code);

    double x = 2;
    double result = Calculate_double (a, x, &err_code);
    check_int (result, 13.5);
    destroy (a);
}

// f(x+y) = f(x) + f(y)
void test_of_Additivity()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {0, 2, 3}; 
    Linear_form* a = create(_a, 3, type_info, &err_code);

    int x = 1, y = 2;
    int calcul1 = Calculate_int(a, (x+y), &err_code);
    int calcul2 = Calculate_int(a, x, &err_code) + Calculate_int(a, y, &err_code);

    check_int(calcul1, calcul2);
    destroy(a);
}

// f(a*x) = a * f(x)
void test_of_Uniformity()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {0, 2, 3}; 
    Linear_form* a = create(_a, 3, type_info, &err_code);

    int x = 2, alpha = 3;

    Linear_form* b = mult(a, &alpha, &err_code);
    int calcul1 = Calculate_int(a, alpha*x, &err_code);
    int calcul2 = Calculate_int(b, x, &err_code);

    check_int(calcul1, calcul2);
    destroy(a);
    destroy(b);
}

// f(0) = 0
void test_of_Null()
{
    int err_code;

    TypeInfo* type_info = get_int_type_info();
    int _a[3] = {1, 2, 3}; 
    Linear_form* a = create(_a, 3, type_info, &err_code);

    int calcul = Calculate_int(a, 0, &err_code);

    check_int(calcul, 1);
    destroy(a);
}


void print_test_stats()
{
    setlocale(LC_ALL, "Rus");
    printf("\n=================================================\n");
    printf("           РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ\n");
    printf("=================================================\n");
    printf("  Всего тестов: %d\n", totalTests);
    printf("  Пройдено:     %d\n", passedTests);
    printf("  Провалено:    %d\n", totalTests - passedTests);
    printf("=================================================\n");
    
    if (passedTests == totalTests) 
        printf("   Все тесты пройдены успешно!\n");
    else 
        printf("   Возникли ошибки!\n");
    printf("=================================================\n\n");
}