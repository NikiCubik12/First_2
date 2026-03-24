#include <stdio.h>
#include <stdlib.h>
#include "polymorph.h"

int int_input(void *out) 
{
    if (scanf("%d", (int*)out) > 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

void int_print(const void *in) 
{
    printf("%d", *(const int*)in); 
}

int double_input(void *out) 
{
    if (scanf("%lf", (double*)out) > 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

void double_print(const void *in) 
{
    printf("%.2f", *(const double*)in);
}

TypeInfo* get_int_type_info() 
{
    static TypeInfo int_info; 
    int_info.size = sizeof(int);
    int_info.input = int_input;
    int_info.print = int_print;
    return &int_info;
}

TypeInfo* get_double_type_info() 
{
    static TypeInfo double_info;
    double_info.size = sizeof(double);
    double_info.input = double_input;
    double_info.print = double_print;
    return &double_info;
}