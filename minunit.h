#include <stdio.h>
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); if (message) \
	return message; } while (0)

extern const char *all_tests();

int main() {
	const char *result = all_tests();
	if (result) {
		printf("\033[31m%s\033[0m\n", result);
	} else {
		printf("\033[32mALL TESTS PASSED\033[0m\n");
	}
	return result != 0;
}
