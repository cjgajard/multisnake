#include <stdio.h>
#include <stdlib.h>
#include "movement.h"
#include "snake.h"

/*
 * Updates the position of a snake point
 */
void move_tail(struct snake_tail *t, enum direction n, int grow);

/*
 * Creates a new snake_tail
 */
struct snake_tail *snake_tail_New(int x, int d)
{
	struct snake_tail *t = malloc(sizeof(struct snake_tail));
	t->next = NULL;
	t->direction = d;
	t->position = x;
	return t;
}

/* public */

struct snake *snake_New()
{
	struct snake *s = malloc(sizeof(struct snake));
	s->directive = KEEP;
	s->head = snake_tail_New(0, RIGHT);
	return s;
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
	move_tail(h, d, s->grow);
	s->grow = 0;
}

bool snake_Ouroboros(struct snake *s)
{
	int p = s->head->position;
	struct snake_tail *t = s->head->next;
	while (t) {
		if (t->position == p)
			return 1;
		t = t->next;
	}
	return 0;
}

bool snake_EatMap(struct snake *s)
{
	return 0;
}

bool snake_EatSnake(struct snake *s, struct snake *s2)
{
	return 0;
}

bool snake_Eat(struct snake *s, int food)
{
	s->grow = s->head->position == food;
	return (bool)s->grow;
}

/* private */

void move_tail(struct snake_tail *t, enum direction n, int grow)
{
	int p = t->position;
	t->position = movement(p, t->direction);
	if (t->next)
		move_tail(t->next, t->direction, grow);
	else if (grow)
		t->next = snake_tail_New(p, t->direction);
	t->direction = n;
}
