#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "movement.h"

struct vector topleft_position(int square_id)
{
	int id = square_id % g_maxpos;
	int w = g_width - 2;
	int x = (id % w) * g_square + g_square;
	int y = (id / w) * g_square + g_square;
	struct vector v = {x, y};
	return v;
}

SDL_Rect get_rect(int square_id)
{
	struct vector v = topleft_position(square_id);
	SDL_Rect rekt = {v.x + 2, v.y + 1, g_square - 2, g_square - 1};
	return rekt;
}

/*
 * Prints an error message to stderr
 */
void log_error(const char str[], const char err[])
{
	fprintf(stderr, "ERROR: %s!\n", str);
	fprintf(stderr, "%s\n", err);
}

/*
 * Logs current SDL error and returns false
 */
bool sdl_error(const char str[])
{
	log_error(str, SDL_GetError());
	return false;
}

/*
 * Starts up SDL and creates window
 */
bool __init()
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return sdl_error("SDL could not initialize");

	// create window
	g_window = SDL_CreateWindow(
			"SDL Tutorial",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
			);

	if (g_window == NULL)
		return sdl_error("Window could not be created");

	// create renderer for window
	g_renderer = SDL_CreateRenderer(
			g_window, -1, SDL_RENDERER_ACCELERATED);
	if (g_renderer == NULL)
		return sdl_error("Renderer could not be created");

	// initialize renderer color
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// get window surface
	g_screen_surface = SDL_GetWindowSurface(g_window);

	return true;
}

void renderMap()
{
	// clear screen
	SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(g_renderer);

	// SDL_SetRenderDrawColor(g_renderer, 0x80, 0x80, 0x80, 0xFF);
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int zero = g_square;
	int right = SCREEN_WIDTH - g_square;
	int bottom = SCREEN_HEIGHT - g_square;

	for (int i = 0; i < g_width; i++) {
		int pos = g_square * (i + 1);
		SDL_RenderDrawLine(g_renderer, pos, zero, pos, bottom);
	}

	for (int i = 0; i < g_height; i++) {
		int pos = g_square * (i + 1);
		SDL_RenderDrawLine(g_renderer, zero, pos, right, pos);
	}
}
