#ifdef TESTS
#define TESTS

#include "polymorph.h"
#include "lineal_form.h"

extern int totalTests = 0;
extern int passedTests = 0;

check_double(double expected, double got);
check_int(int expected, int got);
test_int_plus();
test_int_minus();
test_double_plus();
test_double_minus();
test_int_mult();
test_double_mult();
test_create_int();
test_destroy();
test_create_double();
test_calculate_int();
test_calculate_double();

test_of_Additivity();
test_of_Uniformity();
test_of_Null();

print_test_stats();

#endif