#ifndef SNAKE_H
#define SNAKE_H
#include <stdbool.h>
#include "movement.h"

/*
 * Stores a fraction of a snake string.
 */
struct snake_tail {
	struct snake_tail *next;
	struct vector position;
	enum direction direction;
};

struct snake {
	struct snake_tail *head;
	enum directive directive;
	bool grow;
	int length;
};

bool snake_Eats (struct snake *this, struct snake *other);
bool snake_InBorder (struct snake *this);
struct snake *snake_Create (struct vector x, enum direction d);
struct snake *snake_New (void);
struct snake *snake_OnPoison (struct snake *this);
void snake_Destroy (struct snake *this);
void snake_OnFood (struct snake *this);
void snake_Turn (struct snake *this, enum directive d);
void snake_Update (struct snake *this);
#endif
