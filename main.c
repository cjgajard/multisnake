#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "movement.h"
#include "snake.h"
#include "renderer.h"

/*
 * Execution
 */
int main(int argc, const char* args[])
{
	int quitflag = false;
	int exitcode = 0; // $?
	game_Init(32);

	if (renderer_Init()) {
		exitcode = 1;
		quitflag = 1;
	}

	// main loop
	while (!quitflag) {
		if (renderer_Event() == EVENTQUIT)
			break;
		for (int i = 0; i < SNAKELISTLEN; i++) {
			struct snake *s = g_snakelist[i];
			if (s->head->position == g_food) {
				s->grow = 1;
				game_UpdateFood();
			}
			snake_Update(s);
		}

		renderer_Render();
	}

	renderer_Close();
	return exitcode;
}
