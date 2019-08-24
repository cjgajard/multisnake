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
	bool grow;
	int length;
};

bool snake_Ouroboros (struct snake *this);
struct snake *snake_Create (int n, enum direction d);
struct snake *snake_New (void);
struct snake *snake_OnPoison (struct snake *this);
void snake_Destroy (struct snake *this);
void snake_OnFood (struct snake *this);
void snake_Turn (struct snake *this, enum directive d);
void snake_Update (struct snake *this);
#endif
