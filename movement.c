#include "movement.h"

int mod (int n, int div)
{
	int r = n % div;
	return r < 0 ? r + div : r;
	// return (x % N + N) % N;
}

struct vector movement (enum direction d, struct vector v)
{
	struct vector v2 = {v.x, v.y};
	switch (d) {
	case UP:
		v2.y--;
		break;
	case RIGHT:
		v2.x++;
		break;
	case DOWN:
		v2.y++;
		break;
	case LEFT:
		v2.x--;
		break;
	}
	return v2;
}

int turn (enum directive i, enum direction d)
{
	switch (i) {
	case PORT:
		return mod(d - 1, 4);
	case STARBOARD:
		return mod(d + 1, 4);
	case KEEP:
	default:
		return d;
	}
}

int vector_Eq(struct vector this, struct vector other)
{
	return this.x == other.x && this.y == other.y;
}
