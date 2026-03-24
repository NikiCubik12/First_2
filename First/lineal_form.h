#ifndef LINEAL
#define LINEAL
#include "polymorph.h"

typedef struct
{
    void* coeffs;     
    int size;            
    TypeInfo* type_info; 
} Linear_form;

void Scan_Errors(int err_code);
Linear_form* create(void* coeffs, int n, TypeInfo* type_info, int *err_code);
void destroy(Linear_form* lf);
void Print(Linear_form* a);
Linear_form* plus(Linear_form* a, Linear_form* b, int *err_code);
Linear_form* minus(Linear_form* a, Linear_form* b, int *err_code);
Linear_form* mult(Linear_form* a, void* c, int *err_code);
int Calculate_int(Linear_form* a, int x, int *err_code);
double Calculate_double(Linear_form* a, double x, int *err_code);


#endif