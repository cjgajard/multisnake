#include <stdio.h>
#include "renderer.h"
#include <SDL2/SDL.h>

/*
 * The window we'll be rendering to.
 */
SDL_Window *g_window = NULL;

/*
 * The surface contained by the window.
 */
SDL_Surface *g_screen_surface = NULL;

/*
 * The default renderer.
 */
SDL_Renderer *g_renderer = NULL;

/*
 * Returns a SDL rectangle representing a square with position and dimentions.
 */
SDL_Rect get_rect(int square_id);

/*
 * Stores a 2D position.
 */
struct vector {
	int x;
	int y;
};

/*
 * Returns a vector with the distance from the origin of the top-left corner
 * of a square.
 */
struct vector topleft_position(int square_id);

/* public */

int renderer_Event()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			return EVENTQUIT;
		if (e.type == SDL_KEYDOWN) {
			if (e.key.repeat)
				continue;
			SDL_Keycode k = e.key.keysym.sym;
			switch (k) {
			case SDLK_ESCAPE:
				return EVENTQUIT;
			case SDLK_q:
			case SDLK_y:
				game_InputTurn(PORT);
				break;
			case SDLK_e:
			case SDLK_u:
				game_InputTurn(STARBOARD);
				break;
			case SDLK_1:
			case SDLK_KP_1:
				game_InputSelect(0);
				break;
			default:
#if DEBUG
				fprintf(stderr, "key=%s\n", SDL_GetKeyName(k));
#endif
			}
		}
	}

	return EVENTNONE;
}

void renderer_RenderSnake(int id, struct snake *s)
{
	struct snake_tail *current = s->head;
	while (current) {
		SDL_Rect rect = get_rect(current->position);
		if (id == g_selected) {
			SDL_SetRenderDrawColor(
					g_renderer, 0xFF, 0xFF, 0x00, 0xFF);
		}
		else {
			SDL_SetRenderDrawColor(
					g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
		SDL_RenderFillRect(g_renderer, &rect);
		current = current->next;
	}
}

/* private */

struct vector topleft_position(int square_id)
{
	int id = square_id % g_maxpos;
	int w = g_width - 2;
	int x = (id % w) * g_square + g_square;
	int y = (id / w) * g_square + g_square;
	struct vector v = {x, y};
	return v;
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
int sdl_error(const char str[])
{
	log_error(str, SDL_GetError());
	return 1;
}

/*
 * Starts up SDL and creates window
 */
int renderer_Init()
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
			SDL_WINDOW_SHOWN);

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

	return 0;
}

void renderer_RenderGrid()
{
	// clear screen
	SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(g_renderer);

	SDL_SetRenderDrawColor(g_renderer, 0x40, 0x40, 0x40, 0xFF);
	// SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

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

void renderer_Render()
{
	SDL_RenderPresent(g_renderer);
	SDL_Delay(128);
}

void renderer_Close()
{
	if (g_renderer) {
		SDL_DestroyRenderer(g_renderer);
		g_renderer = NULL;
	}
	if (g_window) {
		SDL_DestroyWindow(g_window);
		g_window = NULL;
	}
	SDL_Quit();
}

SDL_Rect get_rect(int square_id)
{
	struct vector v = topleft_position(square_id);
	SDL_Rect rekt = {v.x + 2, v.y + 1, g_square - 2, g_square - 1};
	return rekt;
}
