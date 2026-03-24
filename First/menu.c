#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "lineal_form.h"
#include "polymorph.h"
#include <errno.h>
#include <limits.h>
#include <float.h>

int Int_input(int min, int max, char* message)
{
    setlocale(LC_ALL, "Rus");
    int value;
    char string[200];
    while (1)
    {
        if (fgets(string, sizeof(string), stdin) == NULL)
        {
            continue;
        }
        char* pointer;
        value = (int) strtol(string, &pointer, 10); 
        if (errno == ERANGE)
        {
            printf("Ошибка: Введено неккоректное значение. Введите ещё раз: \n");
            continue;
        }
        if (*pointer != '\0' && *pointer != '\n') 
        {
            printf("%s", message);
            continue;
        }
        if (min <= value && value <= max)
        {
            return value;
        }
        printf("%s", message);
    }
}

int Double_input(double min, double max, char* message)
{
    setlocale(LC_ALL, "Rus");
    double value;
    char string[200];
    while (1)
    {
        if (fgets(string, sizeof(string), stdin) == NULL)
        {
            continue;
        }
        char* pointer;
        value = (double) strtod(string, &pointer); 
        if (errno == ERANGE)
        {
            printf("Ошибка: Введено неккоректное значение. Введите ещё раз: \n");
            continue;
        }
        if (*pointer != '\0' && *pointer != '\n') 
        {
            printf("%s", message);
            continue;
        }
        if (min <= value && value <= max)
        {
            return value;
        }
        printf("%s", message);
    }
}

int menu()
{
    int err_code;
    int type_choice, size, operation;
    int int_scalar, int_x;
    double double_scalar, double_x;
    
    printf("=== ЛИНЕЙНАЯ ФОРМА ===\n");
    
    // Выбор типа
    printf("Выберите тип данных:\n");
    printf("1. int\n");
    printf("2. double\n");
    printf("Ваш выбор: ");
    type_choice = Int_input(1, 2, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
    
    // Выбор размера
    printf("Введите количество коэффициентов от 1 до 2147483647: ");
    size = Int_input(1, INT_MAX, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
    
    // Создание первой формы
    Linear_form* form1;
    if (type_choice == 1) 
    {
        int* coeffs = malloc(size * sizeof(int));
        printf("Введите %d целых чисел: ", size);
        for (int i = 0; i < size; i++) 
        {
            coeffs[i] = Int_input(INT_MIN, INT_MAX, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
        }
        form1 = create(coeffs, size, get_int_type_info(), &err_code);
        free(coeffs);
    } 
    else 
    {
        double* coeffs = malloc(size * sizeof(double));
        printf("Введите %d чисел: ", size);
        for (int i = 0; i < size; i++) 
        {
            coeffs[i] = Double_input(DBL_MIN, 1e6, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
        }
        form1 = create(coeffs, size, get_double_type_info(), &err_code);
        free(coeffs);
    }
    
    // Создание второй формы
    Linear_form* form2;
    if (type_choice == 1) 
    {
        int* coeffs = malloc(size * sizeof(int));
        printf("Введите %d целых чисел для второй формы: ", size);
        for (int i = 0; i < size; i++)
        {
            coeffs[i] = Int_input(INT_MIN, INT_MAX, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
        }
        form2 = create(coeffs, size, get_int_type_info(), &err_code);
        free(coeffs);
    } 
    else 
    {
        double* coeffs = malloc(size * sizeof(double));
        printf("Введите %d чисел для второй формы: ", size);
        for (int i = 0; i < size; i++) 
        {
            coeffs[i] = Double_input(DBL_MIN, 1e6, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
        }
        form2 = create(coeffs, size, get_double_type_info(), &err_code);
        free(coeffs);
    }
    
    // Меню операций
    while (operation != 0) 
    {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Сложить формы\n");
        printf("2. Вычесть формы\n");
        printf("3. Умножить на скаляр\n");
        printf("4. Вычислить значение\n");
        printf("5. Показать формы\n");
        printf("0. Выход\n");
        printf("Выберите операцию: ");
        operation = Int_input(0, 6, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
        
        switch (operation) 
        {
            case 1: 
            {
                if (type_choice == 1) 
                {
                    Linear_form* result = plus(form1, form2, &err_code);
                    printf("Результат: ");
                    Print(result);
                    destroy(result);
                } 
                else 
                {
                    Linear_form* result = plus(form1, form2, &err_code);
                    printf("Результат: ");
                    Print(result);
                    destroy(result);
                }
                break;
            }
            
            case 2: 
            {
                if (type_choice == 1) 
                {
                    Linear_form* result = minus(form1, form2, &err_code);
                    printf("Результат: ");
                    Print(result);
                    destroy(result);
                } 
                else 
                {
                    Linear_form* result = minus(form1, form2, &err_code);
                    printf("Результат: ");
                    Print(result);
                    destroy(result);
                }
                break;
            }
            
            case 3: 
            {
                printf("Умножить какую форму? (1 или 2): ");
                int form_choice;
                form_choice = Int_input(1, 2, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
                
                Linear_form* target = (form_choice == 1) ? form1 : form2;

                
                if (type_choice == 1) 
                {
                    printf("Введите скаляр: ");
                    int_scalar = Int_input(INT_MIN, INT_MAX, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
                    Linear_form* result = mult(target, &int_scalar, &err_code);
                    printf("Результат: ");
                    Print(result);
                    destroy(result);
                } 
                else 
                {
                    printf("Введите скаляр: ");
                    double_scalar = Double_input(DBL_MIN, 1e6, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
                    Linear_form* result = mult(target, &double_scalar, &err_code);
                    printf("Результат: ");
                    Print(result);
                    destroy(result);
                }
                break;
            }
            
            case 4: 
            {
                printf("Вычислить какую форму? (1 или 2): ");
                int form_choice;
                form_choice = Int_input(1, 2, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
                
                Linear_form* target = (form_choice == 1) ? form1 : form2;
                
                if (type_choice == 1) 
                {
                    printf("Введите x: ");
                    int_x = Int_input(INT_MIN, INT_MAX, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
                    int result = Calculate_int(target, int_x, &err_code);
                    printf("Значение = %d\n", result);
                } 
                else 
                {
                    printf("Введите x: ");
                    double_x = Double_input(DBL_MIN, 1e6, "Ошибка: Введено неккоректное значение. Введите ещё раз:\n");
                    double result = Calculate_double(target, double_x, &err_code);
                    printf("Значение = %.2f\n", result);
                }
                break;
            }
            
            case 5:
            {
                printf("Форма 1: ");
                Print(form1);
                printf("Форма 2: ");
                Print(form2);
                break;
            }
        }  
    } 
    
    destroy(form1);
    destroy(form2);
    
    return 0;
}