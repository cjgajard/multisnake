#ifndef RENDERER_H
#define RENDERER_H
#include "game.h"
#include "snake.h"

#define EVENTQUIT -1
#define EVENTNONE 0

int renderer_Event();
/*
 * Starts up SDL and creates window
 */
int renderer_Init();
void renderer_Close();
void renderer_Render();
#endif
