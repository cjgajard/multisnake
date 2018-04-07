#include "minunit.h"

int foo = 7;
int bar = 5;

static char * test_foo() {
	mu_assert("foo != 7", foo == 7);
	return 0;
}

static char * test_bar() {
	mu_assert("bar != 5", bar == 5);
	return 0;
}

const char * all_tests() {
	mu_run_test(test_foo);
	mu_run_test(test_bar);
	return 0;
}
