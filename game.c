#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "movement.h"

#define POISON_PROBABILITY 0.25
bool g_gameover;
bool g_poison;
int g_food;
int g_score;
int g_selected;
int g_width, g_height, g_maxpos;
int g_snakelist_count;
struct snake *g_snakelist[SNAKELISTLEN];

static bool chance (double probability);
static bool check_collision (void);
static void game_UpdateFood (void);
static void game_OnEat (void);
static void game_OnPoison (struct snake *s);

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
			if (g_poison)
				game_OnPoison(s);
			else
				snake_OnFood(s);
			game_OnEat();
		}
		snake_Update(s);
	}
	g_gameover = check_collision();
}

/* private */

bool chance (double probability)
{
	if (!probability)
		return false;
	return rand() <= (int)(probability * RAND_MAX);
}

bool check_collision ()
{
	for (int i = 0; i < g_snakelist_count; i++) {
		for (int j = 0; j < g_snakelist_count; j++) {
			if (snake_Eats(g_snakelist[i], g_snakelist[j]))
				return true;
		}
	}
	return false;
}

void game_OnEat ()
{
	g_score += 1;
	game_UpdateFood();
}

void game_UpdateFood ()
{
	g_food = rand() % (g_maxpos + 1);
	g_poison = chance(POISON_PROBABILITY);
}

void game_OnPoison (struct snake *s)
{
	if (g_snakelist_count >= SNAKELISTLEN)
		return;
	struct snake *next = snake_OnPoison(s);
	if (!next)
		return;
	g_snakelist[g_snakelist_count++] = next;
}
