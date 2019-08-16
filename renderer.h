#ifndef RENDERER_H
#define RENDERER_H
#include "game.h"
#include "snake.h"

#define EVENTQUIT -1
#define EVENTNONE 0

int renderer_Event();
int renderer_Init();
void renderer_Close();
void renderer_Render();
void renderer_RenderGrid();
void renderer_RenderSnake(int id, struct snake *s);
#endif
