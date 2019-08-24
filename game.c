#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "movement.h"

bool g_gameover;
bool g_poison;
const double poison_probability = 0.333333;
int g_food;
int g_score;
int g_selected;
int g_width, g_height, g_maxpos;
int g_snakelist_count;
struct snake *g_snakelist[SNAKELISTLEN];

// static bool chance (double probability);
static void game_UpdateFood (void);
static void game_OnEat (void);

/* public */

void game_Close ()
{
	for (int i = 0; i < SNAKELISTLEN; i++) {
		if (g_snakelist[i])
			snake_Destroy(g_snakelist[i]);
	}
}

void game_Init (int width, int height)
{
	srand(time(NULL));
	g_width = width - 2;
	g_height = height - 2;
	g_maxpos = g_width * g_height;
	g_gameover = false;
	g_score = 0;
	g_selected = -1;
	game_UpdateFood();
	g_poison = false;
	for (int i = 0; i < SNAKELISTLEN; i++)
		g_snakelist[i] = NULL;
	g_snakelist[0] = snake_Create((g_maxpos + g_width) / 2 - 1, RIGHT);
	g_snakelist_count = 1;
}

void game_InputMove (enum direction d)
{
}

void game_InputSelect (int id)
{
	if (id >= 0 && id < sizeof(g_snakelist) && id != g_selected)
		g_selected = id;
	else
		g_selected = -1;
}

void game_InputTurn (enum directive d)
{
	if (g_selected < 0 || g_selected >= g_snakelist_count)
		return;
	if (!g_snakelist[g_selected])
		return;
	snake_Turn(g_snakelist[g_selected], d);
}

void game_Update ()
{
	for (int i = 0; i < g_snakelist_count; i++) {
		struct snake *s = g_snakelist[i];
		if (!s)
			continue;
		if (s->head->position == g_food) {
			if (g_poison) {
				if (g_snakelist_count < SNAKELISTLEN) {
					struct snake *next = snake_OnPoison(s);
					g_snakelist[g_snakelist_count++] = next;
				}
			}
			else {
				snake_OnFood(s);
			}
			game_OnEat();
		}
		snake_Update(s);
		if (!g_gameover)
			g_gameover = snake_Ouroboros(s);
	}
}

/* private */

/*
bool chance (double probability)
{
	if (!probability)
		return false;
	return rand() <= (int)(probability * RAND_MAX);
}
*/

void game_OnEat ()
{
	g_score += 1;
	game_UpdateFood();
}

void game_UpdateFood ()
{
	g_food = rand() % (g_maxpos + 1);
	g_poison = g_score % 4 == 0;
}
