#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "movement.h"

int SCREEN_WIDTH, SCREEN_HEIGHT;
struct snake *g_snakelist[SNAKELISTLEN];
int g_width, g_height, g_maxpos, g_square;
int g_selected;
int g_food;
int g_score;
bool g_gameover = false;

void game_UpdateFood();

/* public */

void game_Init(int width)
{
	srand(time(NULL));

	SCREEN_WIDTH = 640;
	SCREEN_HEIGHT = 480;
	g_square = SCREEN_WIDTH / width;
	g_width = width - 2;
	g_height = SCREEN_HEIGHT / g_square - 2;
	g_maxpos = g_width * g_height;
	g_selected = -1;
	g_score = 0;
	game_UpdateFood();
	for (int i = 0; i < SNAKELISTLEN; i++)
		g_snakelist[i] = NULL;
	g_snakelist[0] = snake_New((g_maxpos + g_width) / 2 - 1, RIGHT);
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
	if (!g_snakelist[g_selected])
		return;
	g_snakelist[g_selected]->directive = d;
}

void game_Eat()
{
	g_score += 1;
	game_UpdateFood();
}

/* private */

void game_UpdateFood()
{
	g_food = rand() % (g_maxpos + 1);
}
