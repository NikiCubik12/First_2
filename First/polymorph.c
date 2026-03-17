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
    static TypeInfo info; 
    info.size = sizeof(int);
    info.input = int_input;
    info.print = int_print;
    return &info;
}

TypeInfo* get_double_type_info() 
{
    static TypeInfo info;
    info.size = sizeof(double);
    info.input = double_input;
    info.print = double_print;
    return &info;
}