#ifndef RENDERER_H
#define RENDERER_H
#include "game.h"
#include "snake.h"

#define EVENTQUIT -1
#define EVENTNONE 0

int renderer_Event (void);
int renderer_Init (int w, int h, int sqr);
void renderer_Close (void);
void renderer_Render (void);
#endif
