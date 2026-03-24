#include <stdio.h>
#include "tests.h"

int main() 
{
    test_int_plus();
    test_int_minus();
    test_double_plus();
    test_double_minus();
    test_double_mult();
    test_int_mult();
    test_create_int();
    test_destroy();
    test_create_double();
    test_calculate_int();
    test_calculate_double();
    test_of_Additivity();
    test_of_Uniformity();
    test_of_Null();

    
    print_test_stats();

    return 0;
}
