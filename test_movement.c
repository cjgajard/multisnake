#include "minunit.h"
#include "movement.h"

int g_width = 10;
int g_height = 10;
int g_maxpos = 64;

static char *test_movement()
{
    mu_assert("move(4, right) is not 5", movement(4, RIGHT) == 5);
    mu_assert("move(7, right) is not 0", movement(7, RIGHT) == 0);
    mu_assert("move(14, right) is not 15", movement(14, RIGHT) == 15);
    mu_assert("move(23, right) is not 16", movement(23, RIGHT) == 16);
    mu_assert("move(4, left) is not 3", movement(4, LEFT) == 3);
    mu_assert("move(0, left) is not 7", movement(0, LEFT) == 7);
    mu_assert("move(14, left) is not 13", movement(14, LEFT) == 13);
    mu_assert("move(24, left) is not 31", movement(24, LEFT) == 31);
    mu_assert("move(8, up) is not 0", movement(8, UP) == 0);
    mu_assert("move(0, up) is not 56", movement(0, UP) == 56);
    return 0;
}

const char *all_tests() {
    mu_run_test(test_movement);
    return 0;
}
