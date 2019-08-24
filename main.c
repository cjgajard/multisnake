#include "renderer.h"

/*
 * Execution
 */
int main(int argc, const char* args[])
{
	int exitcode = 0; // $?
	game_Init(32);
	if (renderer_Init()) {
		exitcode = 1;
	}

	while (!exitcode) {
		if (renderer_Event() == EVENTQUIT)
			break;
		if (g_gameover)
			continue;

		for (int i = 0; i < SNAKELISTLEN; i++) {
			struct snake *s = g_snakelist[i];
			if (!s)
				continue;
			if (snake_Eat(s, g_food))
				game_Eat();
			snake_Update(s);
			if (!g_gameover)
				g_gameover = snake_Ouroboros(s);
		}

		renderer_Render();
	}

	renderer_Close();
	return exitcode;
}
