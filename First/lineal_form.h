#ifndef LINEAL
#define LINEAL
#include "polymorph.h"

typedef struct
{
    void* coeffs; // указатель ~ динамическому массиву       
    int size;            
    TypeInfo* type_info; 
} Linear_form;

Linear_form* create(void* coeffs, int n, TypeInfo* type_info);
void destroy(Linear_form* lf);
void Print(Linear_form* a);
Linear_form* plus(Linear_form* a, Linear_form* b);
Linear_form* minus(Linear_form* a, Linear_form* b);
Linear_form* mult(Linear_form* a, void* c);
int Calculate_int(Linear_form* a, int x);
double Calculate_double(Linear_form* a, double x);


#endif