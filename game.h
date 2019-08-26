#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include "list.h"
#include "snake.h"
#define SNAKELISTLEN 10

extern bool g_gameover;
extern bool g_poison;
extern int g_score;
extern int g_selected;
extern int g_speed;
extern struct list *g_snakelist;
extern struct vector g_food;

void game_Close (void);
/*
 * Initialize game variables.
 * - width: Number of horizontal squares in the screen. Recomended 80, 64, 40,
 *   32, 20 or 16.
 */
void game_Init (int width, int height);
void game_InputMove (enum direction d);
void game_InputSelect (int id);
void game_InputTurn (enum directive d);
void game_Update (void);
#endif
