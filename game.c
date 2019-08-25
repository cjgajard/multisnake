#include <stdlib.h>
#include <time.h>
#include "game.h"

#define POISON_PROBABILITY 0.25
bool g_gameover;
bool g_poison;
struct vector g_food;
int g_score;
int g_selected;
int g_width, g_height, g_maxpos;
int g_snakelist_count;
struct snake *g_snakelist[SNAKELISTLEN];

static bool chance (double probability);
static bool check_collision (void);
static bool check_border (void);
static bool is_selection (void);
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
	struct vector initial = {g_width / 2 - 1, g_height / 2};
	g_snakelist[0] = snake_Create(initial, RIGHT);
	g_snakelist_count = 1;
}

void game_InputMove (enum direction d)
{
	if (!is_selection())
		return;
	enum direction now = g_snakelist[g_selected]->head->direction;
	enum directive next;
	switch (now - d) {
	case -1:
	case 3:
		next = STARBOARD;
		break;
	case 1:
	case -3:
		next = PORT;
		break;
	default:
		next = KEEP;
		break;
	}
	snake_Turn(g_snakelist[g_selected], next);
}

void game_InputRotate (int dir)
{
	int x = dir > 0 ? 1 : -1;
	int next = mod(g_selected + x, g_snakelist_count);
	if (next >= 0 && next < sizeof(g_snakelist))
		g_selected = next;
	else
		g_selected = -1;
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
	if (!is_selection())
		return;
	snake_Turn(g_snakelist[g_selected], d);
}

void game_Update ()
{
	for (int i = 0; i < g_snakelist_count; i++) {
		struct snake *s = g_snakelist[i];
		if (!s)
			continue;
		if (vector_Eq(s->head->position, g_food)) {
			if (g_poison)
				game_OnPoison(s);
			else
				snake_OnFood(s);
			game_OnEat();
		}
		snake_Update(s);
	}
	g_gameover = check_collision() || check_border();
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

bool check_border ()
{
	for (int i = 0; i < g_snakelist_count; i++) {
		struct snake *s = g_snakelist[i];
		if (!s)
			continue;
		if (snake_InBorder(s))
			return true;
	}
	return false;
}

bool is_selection ()
{
	if (g_selected < 0 || g_selected >= g_snakelist_count)
		return false;
	if (!g_snakelist[g_selected])
		return false;
	return true;
}

void game_OnEat ()
{
	g_score += 1;
	game_UpdateFood();
}

void game_UpdateFood ()
{
	g_food.x = rand() % (g_width - 2) + 1;
	g_food.y = rand() % (g_height - 2) + 1;
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
