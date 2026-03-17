#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "polymorph.h"
#include "lineal_form.h"

Linear_form* create(void* coeffs, int n, TypeInfo* type_info) 
{
    setlocale(LC_ALL, "Rus");
    Linear_form* result = (Linear_form*) malloc(sizeof(Linear_form));

    result->size = n;
    result->type_info = type_info;
    result->coeffs = malloc(n * type_info->size);
    
    memcpy(result->coeffs, coeffs, n * type_info->size); 
    
    return result;
}

void destroy(Linear_form* lf) 
{
    if (lf != NULL) 
    {
        free(lf->coeffs);
        free(lf);
    }
}

void Print(Linear_form* a) 
{
    printf("[");
    for (int i = 0; i < a->size; i++) 
    {
        void* element = (char*)a->coeffs + i*a->type_info->size; 
        a->type_info->print(element);
        if (i < a->size - 1) 
        {
            printf(", ");
        }
    }
    printf("]\n");
}

Linear_form* plus(Linear_form* a, Linear_form* b) 
{
    if (a->type_info != b->type_info) {
        printf("Ошибка: разные типы \n");
        return NULL;
    }
    
    if (a->size != b->size) {
        printf("Ошибка: разные размеры\n");
        return NULL;
    }
    
    Linear_form* result = create(a->coeffs, a->size, a->type_info);
    
    if (a->type_info == get_int_type_info()) 
    {
        int* a_coeffs = (int*)a->coeffs;
        int* b_coeffs = (int*)b->coeffs;
        int* res_coeffs = (int*)result->coeffs;
        
        for (int i = 0; i < a->size; i++) 
        {
            res_coeffs[i] = a_coeffs[i] + b_coeffs[i];
        }
    }
    else if (a->type_info == get_double_type_info()) 
    {
        double* a_coeffs = (double*)a->coeffs;
        double* b_coeffs = (double*)b->coeffs;
        double* res_coeffs = (double*)result->coeffs;
        
        for (int i = 0; i < a->size; i++) 
        {
            res_coeffs[i] = a_coeffs[i] + b_coeffs[i];
        }
    }
    
    return result;
}

Linear_form* minus(Linear_form* a, Linear_form* b) 
{
    if (a->type_info != b->type_info) {
        printf("Ошибка: разные типы \n");
        return NULL;
    }
    
    if (a->size != b->size) {
        printf("Ошибка: разные размеры\n");
        return NULL;
    }
    
    Linear_form* result = create(a->coeffs, a->size, a->type_info);
    
    if (a->type_info == get_int_type_info()) 
    {
        int* a_coeffs = (int*)a->coeffs;
        int* b_coeffs = (int*)b->coeffs;
        int* res_coeffs = (int*)result->coeffs;
        
        for (int i = 0; i < a->size; i++) 
        {
            res_coeffs[i] = a_coeffs[i] - b_coeffs[i];
        }
    }
    else if (a->type_info == get_double_type_info()) 
    {
        double* a_coeffs = (double*)a->coeffs;
        double* b_coeffs = (double*)b->coeffs;
        double* res_coeffs = (double*)result->coeffs;
        
        for (int i = 0; i < a->size; i++) 
        {
            res_coeffs[i] = a_coeffs[i] - b_coeffs[i];
        }
    }
    
    return result;
}

Linear_form* mult(Linear_form* a, void* c) 
{
    Linear_form* result = create(a->coeffs, a->size, a->type_info);
    
    if (a->type_info == get_int_type_info()) 
    {
        int* a_coeffs = (int*)a->coeffs;
        int* res_coeffs = (int*)result->coeffs;
        int scalar = *(int*)c;
        
        for (int i = 0; i < a->size; i++) 
        {
            res_coeffs[i] = a_coeffs[i] * scalar;
        }
    }
    else if (a->type_info == get_double_type_info()) 
    {
        double* a_coeffs = (double*)a->coeffs;
        double* res_coeffs = (double*)result->coeffs;
        double scalar = *(double*)c;
        
        for (int i = 0; i < a->size; i++) 
        {
            res_coeffs[i] = a_coeffs[i] * scalar;
        }
    }
    
    return result;
}

int Calculate_int(Linear_form* a, int x) 
{
    if (a->type_info != get_int_type_info()) 
    {
        printf("Ошибка: неправильный тип для Calculate_int\n");
        return 0;
    }
    
    int* coeffs = (int*)a->coeffs;
    int result = coeffs[0]; 
    for (int i = 1; i < a->size; i++) 
    {
        result += coeffs[i] * x;
    }
    return result;
}

double Calculate_double(Linear_form* a, double x) {
    if (a->type_info != get_double_type_info()) {
        printf("Ошибка: неправильный тип для Calculate_double\n");
        return 0;
    }
    
    double* coeffs = (double*)a->coeffs;
    double result = coeffs[0];
    for (int i = 1; i < a->size; i++) 
    {
        result += coeffs[i] * x;
    }
    return result;
}