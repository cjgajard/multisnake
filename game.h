#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include "snake.h"
#define SNAKELISTLEN 10

/*
 * Screen dimensions
 */
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

/*
 * Size in pixels of each square.
 */
extern int g_square;

extern struct snake *g_snakelist[SNAKELISTLEN];

extern int g_selected;

extern int g_food;

extern bool g_gameover;

extern int g_score;

/*
 * Initialize game variables.
 * - width: Number of horizontal squares in the screen. Recomended 80, 64, 40,
 *   32, 20 or 16.
 */
void game_Init(int width);

void game_InputMove(enum direction d);

void game_InputSelect(int id);

void game_InputTurn(enum directive d);

void game_Eat();
#endif
