#include <stdlib.h>
#include "snake.h"

/*
 * Creates a new snake_tail
 */
struct snake_tail *snake_tail_New (struct vector x, enum direction d)
{
	struct snake_tail *t;
	t = malloc(sizeof(*t));
	t->next = NULL;
	t->direction = d;
	t->position = x;
	return t;
}

/* public */

bool snake_Eats (struct snake *this, struct snake *other)
{
	if (!this || !other)
		return false;
	struct vector x = this->head->position;
	struct snake_tail *t = other->head;
	if (this == other)
		t = t->next;
	while (t) {
		if (vector_Eq(t->position, x))
			return true;
		t = t->next;
	}
	return false;
}

/*
bool snake_OutOfBorders (struct snake *this)
{
	struct snake_tail *t = this->head;
	while (t) {
		if (t->position == x)
			return true;
		t = t->next;
	}
	return false;
}
*/

struct snake *snake_Create (struct vector x, enum direction d)
{
	struct snake *this = snake_New();
	this->head = snake_tail_New(x, d);
	this->grow = true;
	this->length += 1;
	return this;
}

struct snake *snake_New ()
{
	struct snake *this = malloc(sizeof(struct snake));
	this->directive = KEEP;
	this->grow = false;
	this->length = 0;
	return this;
}

void snake_Destroy (struct snake *this)
{
	struct snake_tail *t = this->head;
	while (t) {
		struct snake_tail *n = t->next;
		free(t);
		t = n;
	}
	free(this);
}

void snake_OnFood (struct snake *this)
{
	this->grow = true;
}

struct snake *snake_OnPoison (struct snake *this)
{
	struct snake_tail *t = this->head;
	int i = 1;
	int l = this->length / 2;

	while (t) {
		if (i >= l)
			break;
		t = t->next;
		i++;
	}

	if (!t->next)
		return NULL;
	struct snake *s = snake_New();
	s->head = t->next;
	s->length = this->length - l;
	t->next = NULL;
	this->length = l;
	return s;
}

void snake_Turn (struct snake *this, enum directive d)
{
	this->directive = d;
}

void snake_Update (struct snake *this)
{
	struct snake_tail **indirect = &this->head;
	enum direction nd = turn(this->directive, (*indirect)->direction);
	struct vector x;

	while (*indirect) {
		x = (*indirect)->position;
		enum direction d = (*indirect)->direction;
		(*indirect)->position = movement(d, x);
		(*indirect)->direction = nd;
		nd = d;
		indirect = &(*indirect)->next;
	}

	if (this->grow)
		*indirect = snake_tail_New(x, nd);

	this->length += (int)this->grow;
	this->directive = KEEP;
	this->grow = false;
}
