#ifndef SNAKE_H
#define SNAKE_H
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

struct snake *snake_New(void);
void snake_Append(struct snake *s, int x);
void snake_Destroy(struct snake *s);
void snake_Update(struct snake *s);
#endif
