#include <stdio.h>
#include <stdbool.h>
// #include <stdlib.h>
// #include <time.h>
#include "game.h"
#include "movement.h"

int SCREEN_WIDTH, SCREEN_HEIGHT;
struct snake *g_snakelist[1];
int g_width, g_height, g_maxpos, g_square;
int g_selected;

void game_Init(int width)
{
	SCREEN_WIDTH = 640;
	SCREEN_HEIGHT = 480;
	g_width = width;
	g_square = SCREEN_WIDTH / g_width;
	g_height = SCREEN_HEIGHT / g_square;
	g_maxpos = (g_width - 2) * (g_height - 2);
	g_selected = -1;

	// srand(time(NULL));
	g_snakelist[0] = snake_New(/*direction, position*/);

	for (int i = 0, l = 2; i < l; i++) {
		snake_Update(g_snakelist[0]);
		snake_Append(g_snakelist[0]);
	}
}

void game_InputMove(enum direction d)
{
}

void game_InputSelect(int id)
{
	if (id >= 0 && id < sizeof(g_snakelist) && id != g_selected)
		g_selected = id;
	else
		g_selected = -1;
}

void game_InputTurn(enum directive d)
{
	if (g_selected < 0)
		return;
	g_snakelist[g_selected]->directive = d;
}
