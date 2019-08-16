#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "movement.h"
#include "snake.h"
#include "renderer.h"

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

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
		if (renderer_Event() == EVENTQUIT) {
			quitflag = true;
			break;
		}

		renderer_RenderGrid();
		for (int i = 0, l = LENGTH(g_snakelist); i < l; i++) {
			struct snake *snake = g_snakelist[i];
			snake_Update(snake);
			renderer_RenderSnake(i, snake);
		}
		renderer_Render();
	}

	renderer_Close();
	return exitcode;
}
