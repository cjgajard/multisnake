#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "list.h"

#define POISON_PROBABILITY 0.25
bool g_gameover;
bool g_poison;
int g_score;
int g_selected;
int g_width, g_height, g_maxpos;
struct list *g_snakelist;
struct vector g_food;

static bool chance (double probability);
static bool check_collision (void);
static bool check_border (void);
static void game_UpdateFood (void);
static void game_OnEat (void);
static void game_OnPoison (struct snake *s);

/* public */

void game_Close ()
{
	struct list_item *i = g_snakelist->head;
	while (i) {
		struct list_item *n = i->next;
		struct snake *s = i->value;
		snake_Destroy(s);
		i = n;
	}
	list_Destroy(g_snakelist);
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
	g_snakelist = list_New();

	struct vector initial = {g_width / 2 - 1, g_height / 2};
	list_Append(g_snakelist, snake_Create(initial, RIGHT));
}

void game_InputMove (enum direction d)
{
	struct snake *s = list_Get(g_snakelist, g_selected);
	if (!s)
		return;
	enum direction now = s->head->direction;
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
	snake_Turn(s, next);
}

void game_InputRotate (int dir)
{
	int x = dir > 0 ? 1 : -1;
	int next = mod(g_selected + x, g_snakelist->length);
	g_selected = next;
}

void game_InputSelect (int id)
{
	g_selected = (id == g_selected) ? -1 : id;
}

void game_InputTurn (enum directive d)
{
	struct snake *s = list_Get(g_snakelist, g_selected);
	if (!s)
		return;
	snake_Turn(s, d);
}

void game_Update ()
{
	struct list_item *i = g_snakelist->head;
	while (i) {
		struct snake *s = i->value;
		if (vector_Eq(s->head->position, g_food)) {
			if (g_poison)
				game_OnPoison(s);
			else
				snake_OnFood(s);
			game_OnEat();
		}
		snake_Update(s);
		i = i->next;
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
	struct list_item *i = g_snakelist->head;
	while (i) {
		struct snake *s = i->value;
		struct list_item *j = g_snakelist->head;
		while (j) {
			struct snake *s2 = j->value;
			if (snake_Eats(s, s2))
				return true;
			j = j->next;
		}
		i = i->next;
	}
	return false;
}

bool check_border ()
{
	struct list_item *i = g_snakelist->head;
	while (i) {
		struct snake *s = i->value;
		if (snake_InBorder(s))
			return true;
		i = i->next;
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
	g_food.x = rand() % (g_width - 2) + 1;
	g_food.y = rand() % (g_height - 2) + 1;
	g_poison = chance(POISON_PROBABILITY);
}

void game_OnPoison (struct snake *s)
{
	if (g_snakelist->length >= SNAKELISTLEN)
		return;
	struct snake *next = snake_OnPoison(s);
	if (!next)
		return;
	list_Append(g_snakelist, next);
}
