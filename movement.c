#include "movement.h"

int mod (int x, int N)
{
	int r = x % N;
	return r < 0 ? r + N : r;
	// return (x % N + N) % N;
}

int movement (int position, int direction)
{
	int w = g_width;

	switch (direction) {
	case UP:
		return mod(position - w, g_maxpos);
	case RIGHT:
		return mod(position + 1, w) + (position / w) * w;
	case DOWN:
		return mod(position + w, g_maxpos);
	case LEFT:
		return mod(position - 1, w) + (position / w) * w;
	default:
		return position;
	}
}

int turn (int direction, int directive)
{
	switch (directive) {
	case PORT:
		return mod(direction - 1, 4);
	case STARBOARD:
		return mod(direction + 1, 4);
	// case KEEP:
	default:
		return direction;
	}
}
