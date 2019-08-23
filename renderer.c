#include <stdio.h>
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

/*
 * Stores a 2D position.
 */
struct vector {
	int x;
	int y;
};

/*
 * Stores a text texture and its dimensions.
 */
struct msg {
	SDL_Texture *t;
	int w;
	int h;
};

static SDL_Window *window = NULL;
// static SDL_Surface *screen = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font_strong = NULL;
static const SDL_Color white = {255, 255, 255};
static struct msg *msg_gameover = NULL;

/*
 * Prints an error message to stderr
 */
static void log_error(const char str[], const char err[]);
/*
 * Logs current SDL error and returns false
 */
static int sdl_error(const char str[]);
/*
 * Returns a SDL rectangle representing a square with position and dimentions.
 */
static SDL_Rect get_rect(int square_id);
/*
 * Returns a vector with the distance from the origin of the top-left corner
 * of a square.
 */
static struct vector topleft_position(int square_id);
static void renderer_RenderGrid();
static void renderer_RenderSnake(int id);
static void renderer_RenderFood();
static void renderer_GameOver();
static struct msg *msg_New(TTF_Font *f, const char *str);
static void msg_Destroy(struct msg *t);
static void msg_Render(struct msg *t, int x, int y);

/* public */

void renderer_Close()
{
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	if (font_strong) {
		TTF_CloseFont(font_strong);
		font_strong = NULL;
	}
	if (msg_gameover)
		msg_Destroy(msg_gameover);
	SDL_Quit();
}

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
			case SDLK_2:
			case SDLK_KP_2:
				game_InputSelect(1);
				break;
			}
		}
	}

	return EVENTNONE;
}

int renderer_Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return sdl_error("SDL could not initialize");
	if (TTF_Init() < 0) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return 2;
	}

	window = SDL_CreateWindow(
			"SDL Tutorial",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (window == NULL)
		return sdl_error("Window could not be created");

	renderer = SDL_CreateRenderer(
			window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		return sdl_error("Renderer could not be created");

	// screen = SDL_GetWindowSurface(window);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	font_strong = TTF_OpenFont("assets/SourceSansPro-Regular.ttf", 24);
	msg_gameover = msg_New(font_strong, "GAME OVER");
	return 0;
}

void renderer_Render()
{
	if (g_gameover) {
		renderer_GameOver();
		goto flush;
	}
	renderer_RenderGrid();
	renderer_RenderFood();
	for (int i = 0; i < SNAKELISTLEN; i++)
		renderer_RenderSnake(i);
flush:
	SDL_RenderPresent(renderer);
	SDL_Delay(128);
}

/* private */

struct vector topleft_position(int square_id)
{
	int id = square_id % g_maxpos;
	int x = (id % g_width) * g_square + g_square;
	int y = (id / g_width) * g_square + g_square;
	struct vector v = {x, y};
	return v;
}

void log_error(const char str[], const char err[])
{
	fprintf(stderr, "ERROR: %s!\n", str);
	fprintf(stderr, "%s\n", err);
}

int sdl_error(const char str[])
{
	log_error(str, SDL_GetError());
	return 1;
}

void renderer_RenderFood()
{
	SDL_Rect rect = get_rect(g_food);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
}

void renderer_RenderGrid()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);

	int zero = g_square;
	int right = SCREEN_WIDTH - g_square;
	int bottom = SCREEN_HEIGHT - g_square;

	for (int i = 0; i <= g_width; i++) {
		int pos = g_square * (i + 1);
		SDL_RenderDrawLine(renderer, pos, zero, pos, bottom);
	}

	for (int i = 0; i <= g_height; i++) {
		int pos = g_square * (i + 1);
		SDL_RenderDrawLine(renderer, zero, pos, right, pos);
	}
}

void renderer_RenderSnake(int id)
{
	struct snake *s = g_snakelist[id];
	if (s == NULL)
		return;
	struct snake_tail *current = s->head;
	while (current) {
		SDL_Rect rect = get_rect(current->position);
		if (id == g_selected)
			SDL_SetRenderDrawColor(
					renderer, 0xFF, 0xFF, 0x00, 0xFF);
		else
			SDL_SetRenderDrawColor(
					renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &rect);
		current = current->next;
	}
}

void renderer_GameOver()
{
	int padding = 16;
	int w = msg_gameover->w + padding * 2;
	int h = msg_gameover->h + padding;
	SDL_Rect rect = {
		.x = (SCREEN_WIDTH - w) / 2,
		.y = (SCREEN_HEIGHT - h) / 2,
		.w = w + 2,
		.h = h + 2
	};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xCC);
	SDL_RenderFillRect(renderer, &rect);
	msg_Render(msg_gameover, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

SDL_Rect get_rect(int square_id)
{
	struct vector v = topleft_position(square_id);
	SDL_Rect rekt = {v.x + 2, v.y + 1, g_square - 2, g_square - 1};
	return rekt;
}

static struct msg *msg_New(TTF_Font *f, const char *str)
{
	int w, h;
	struct msg *t;
	t = malloc(sizeof(*t));

	SDL_Surface *srf = TTF_RenderText_Blended(f, str, white);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_FreeSurface(srf);
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	t->t = texture;
	t->w = w;
	t->h = h;
	return t;
}

static void msg_Destroy(struct msg *t)
{
	SDL_DestroyTexture(t->t);
	free(t);
	t = NULL;
}

static void msg_Render(struct msg *t, int x, int y)
{
	SDL_Rect r = {
		.x = x - t->w / 2,
		.y = y - t->h / 2,
		.w = t->w,
		.h = t->h
	};
	SDL_RenderCopy(renderer, t->t, NULL, &r);
}
