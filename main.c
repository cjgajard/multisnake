#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "movement.h"
#include "snake.h"

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

int SCREEN_WIDTH, SCREEN_HEIGHT;
int g_width, g_height, g_maxpos, g_square;
SDL_Window *g_window;
SDL_Renderer *g_renderer;
SDL_Surface *g_screen_surface;

/*
 * Execution
 */
int main(int argc, const char* args[])
{
	// initialize variable globals
	SCREEN_WIDTH = 640;
	SCREEN_HEIGHT = 480;
	g_width = 40; // 80 64 40 32 20 16
	g_square = SCREEN_WIDTH / g_width;
	g_height = SCREEN_HEIGHT / g_square;
	g_maxpos = (g_width - 2) * (g_height - 2);

	// define variables
	int exitcode = 0; // $?
	bool quit = false; // main loop flag
	SDL_Event e; // event handler

	// initialize
	srand(time(NULL));

	if (!__init()) {
		exitcode = 1;
		goto quit_sdl;
	}

	int timelapse = 0;

	int Q_dir = 0;
	int A_dir = 0;
	int Z_dir = 0;

	struct snake *head = NULL;
	struct snake *snake = NULL;

	// new_snake_chain(directive, direction, position);
	for (int i = 0, l = 4; i < l; i++) {
		snake = malloc(sizeof(struct snake));
		snake->directive = (i == l - 1) ? &Q_dir : NULL;
		snake->direction = LEFT;
		snake->next_dir = LEFT;
		snake->position = g_maxpos / 2 + l - i;
		snake->next = head;
		head = snake;
	}

	struct snake *head2 = NULL;
	for (int i = 0, l = 2; i < l; i++) {
		snake = malloc(sizeof(struct snake));
		snake->directive = (i == l - 1) ? &A_dir : NULL;
		snake->direction = RIGHT;
		snake->next_dir = RIGHT;
		snake->position = g_maxpos / 2 - l + i - g_width;
		snake->next = head2;
		head2 = snake;
	}

	struct snake *head3 = NULL;
	for (int i = 0, l = 10; i < l; i++) {
		snake = malloc(sizeof(struct snake));
		snake->directive = (i == l - 1) ? &Z_dir : NULL;
		snake->direction = DOWN;
		snake->next_dir = DOWN;
		snake->position = i * (g_width - 2);
		snake->next = head3;
		head3 = snake;
	}

	struct snake *snakelist[3] = {head, head2, head3};

	// main loop
	while (!quit) {
		// handle events on event queue
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_q:
						Q_dir = PORT;
						break;
					case SDLK_w:
						Q_dir = STARBOARD;
						break;
					case SDLK_a:
						A_dir = PORT;
						break;
					case SDLK_s:
						A_dir = STARBOARD;
						break;
					case SDLK_z:
						Z_dir = PORT;
						break;
					case SDLK_x:
						Z_dir = STARBOARD;
						break;
				}
			} else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
					case SDLK_q:
					case SDLK_w:
						Q_dir = KEEP;
						break;
					case SDLK_a:
					case SDLK_s:
						A_dir = KEEP;
						break;
					case SDLK_z:
					case SDLK_x:
						Z_dir = KEEP;
						break;
				}
			}
		}

		// print grid
		renderMap();

		// if (timelapse++ % 10 == 0){ food = rand() % g_maxpos;
		for (int i = 0, l = LENGTH(snakelist); i < l; i++) {
			struct snake *indirect = snakelist[i];

		move_snake(indirect);
		while (indirect) {
			/*printf(
				"(%d) snake %p: position %d\n",
				timelapse, indirect, indirect->position);*/

			SDL_Rect rect;
			rect = get_rect(indirect->position);
			SDL_SetRenderDrawColor(
				g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderFillRect(g_renderer, &rect);

			indirect = indirect->next;
		}
		}


		timelapse += 1;

		SDL_RenderPresent(g_renderer);
		SDL_Delay(128);
	}

	// Destroy window
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;
quit_sdl:
	// Close SDL
	SDL_Quit();
	return exitcode;
}
