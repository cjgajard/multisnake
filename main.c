#include <time.h>
#include "renderer.h"

static struct timespec tframe;
static struct timespec tlast = {0};
static bool wait_frame ();

int main(int argc, const char* args[])
{
	int exitcode = 0; // $?

	int sw = 640, sh = 480;
	int w = 32;
	int h = sh / (sw / w);

	game_Init(w, h);
	tframe.tv_sec = 0;
	tframe.tv_nsec = 1000L * 1000L * 1000L / (long)g_speed;

	if (renderer_Init(sw, sh, sw / w))
		exitcode = 1;

	while (!exitcode) {
		if (renderer_Event() == EVENTQUIT)
			break;
		if (g_gameover)
			continue;
		if (wait_frame())
			continue;
		game_Update();
		renderer_Render();
	}

	game_Close();
	renderer_Close();
	return exitcode;
}
static bool wait_frame ()
{
	struct timespec tnow;
	clock_gettime(CLOCK_MONOTONIC, &tnow);
	struct timespec tnext = {
		tlast.tv_sec + tframe.tv_sec,
		tlast.tv_nsec + tframe.tv_nsec
	};
	if (tnow.tv_sec <= tnext.tv_sec && tnow.tv_nsec < tnext.tv_nsec)
		return true;
	tlast.tv_sec = tnow.tv_sec;
	tlast.tv_nsec = tnow.tv_nsec;
	return false;
}
