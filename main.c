#include "renderer.h"

/*
 * Execution
 */
int main(int argc, const char* args[])
{
	int exitcode = 0; // $?

	int sw = 640, sh = 480;
	int w = 32;
	int h = sh / (sw / w);

	game_Init(w, h);

	if (renderer_Init(sw, sh, sw / w))
		exitcode = 1;

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
