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
		game_Update();
		renderer_Render();
	}

	game_Close();
	renderer_Close();
	return exitcode;
}
