#ifndef SNAKE_H
#define SNAKE_H
#include <stdbool.h>
#include "movement.h"

/*
 * Stores a fraction of a snake string.
 */
struct snake_tail {
	// struct snake_tail *previous;
	struct snake_tail *next;
	enum direction direction;
	int position;
};

struct snake {
	struct snake_tail *head;
	enum directive directive;
	int grow;
};

struct snake *snake_New(int n, enum direction d);
void snake_Append(struct snake *s, int x);
void snake_Destroy(struct snake *s);
void snake_Update(struct snake *s);
bool snake_Eat(struct snake *s, int food);
bool snake_Ouroboros(struct snake *s);
#endif
