#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "movement.h"

bool g_gameover = false;
int g_food;
int g_score;
int g_selected;
int g_width, g_height, g_maxpos;
struct snake *g_snakelist[SNAKELISTLEN];

static void game_UpdateFood();
static void game_Eat();

/* public */

void game_Init(int width, int height)
{
	srand(time(NULL));
	g_width = width - 2;
	g_height = height - 2;
	g_maxpos = g_width * g_height;
	g_selected = -1;
	g_score = 0;
	game_UpdateFood();
	for (int i = 0; i < SNAKELISTLEN; i++)
		g_snakelist[i] = NULL;
	g_snakelist[0] = snake_Create((g_maxpos + g_width) / 2 - 1, RIGHT);
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
	if (g_selected < 0 || g_selected >= SNAKELISTLEN)
		return;
	if (!g_snakelist[g_selected])
		return;
	snake_Turn(g_snakelist[g_selected], d);
}

void game_Eat()
{
	g_score += 1;
	game_UpdateFood();
}

void game_Update()
{
	for (int i = 0; i < SNAKELISTLEN; i++) {
		struct snake *s = g_snakelist[i];
		if (!s)
			continue;
		if (s->head->position == g_food) {
			snake_OnFood(s);
			game_Eat();
		}
		snake_Update(s);
		if (!g_gameover)
			g_gameover = snake_Ouroboros(s);
	}
}

/* private */

static void game_UpdateFood()
{
	g_food = rand() % (g_maxpos + 1);
}
