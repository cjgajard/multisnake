#include <stdio.h>
#include <stdlib.h>
#include "movement.h"
#include "snake.h"

/*
 * Updates the position of a snake point
 */
void move_tail(struct snake_tail *t, enum direction n);

/*
 * Creates a new snake_tail
 */
struct snake_tail *snake_tail_New()
{
	struct snake_tail *t = malloc(sizeof(struct snake_tail));
	t->next = NULL;
	t->direction = RIGHT; // TODO
	t->position = 0; // TODO
	return t;
}

/* public */

struct snake *snake_New()
{
	struct snake *s = malloc(sizeof(struct snake));
	s->directive = KEEP;
	s->head = snake_tail_New();
	return s;
}

void snake_Append(struct snake *s)
{
	struct snake_tail **indirect = /*(struct snake_tail *)*/&s->head;
	while (*indirect)
		indirect = &(*indirect)->next;
	*indirect = snake_tail_New();
}

void snake_Turn(struct snake *s, enum directive d)
{
	s->directive = d;
}

void snake_Update(struct snake *s)
{
	struct snake_tail *h = (struct snake_tail *)s->head;
	enum direction d = turn(h->direction, s->directive);
	s->directive = KEEP;
	move_tail(h, d);
}

/* private */

void move_tail(struct snake_tail *t, enum direction n)
{
	if (!t)
		return;
	t->position = movement(t->position, t->direction);
	move_tail(t->next, t->direction);
	t->direction = n;
}
