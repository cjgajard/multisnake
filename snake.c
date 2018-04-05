#include "movement.h"
#include "snake.h"

void move_snake(struct snake *snake)
{
	if (!snake)
		return;

	snake->position = movement(snake->position, snake->direction);

	if (snake->directive)
		snake->next_dir = turn(snake->next_dir, *snake->directive);

	if (snake->next)
		snake->next->next_dir = snake->direction;

	move_snake(snake->next);
	snake->direction = snake->next_dir;
}
