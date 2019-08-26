#include <stdio.h>
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

/*
 * Stores a text texture and its dimensions.
 */
struct msg {
	SDL_Texture *t;
	int w;
	int h;
};

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
static TTF_Font *font_regular = NULL;
static TTF_Font *font_strong = NULL;
static const SDL_Color white = {255, 255, 255};
static const SDL_Color black = {0, 0, 0};
/*
 * Size in pixels of each square.
 */
static int g_square;
static int screen_width;
static int screen_height;
static struct msg *msg_gameover = NULL;

/*
 * Prints an error message to stderr
 */
static void log_error (const char str[], const char err[]);
/*
 * Logs current SDL error and returns false
 */
static int sdl_error (const char str[]);
/*
 * Returns a SDL rectangle representing a square with position and dimentions.
 */
static SDL_Rect get_rect (struct vector v);
static void renderer_RenderFood (void);
static void renderer_RenderGameOver (void);
static void renderer_RenderGrid (void);
static void renderer_RenderScore (void);
static void renderer_RenderSnake (int id, struct snake *s);
static struct msg *msg_New (TTF_Font *f, SDL_Color c, const char *str);
static void msg_Destroy (struct msg *t);
static void msg_Render (struct msg *t, int x, int y);

/* public */

void renderer_Close ()
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
	if (font_regular) {
		TTF_CloseFont(font_regular);
		font_regular = NULL;
	}
	if (msg_gameover) {
		msg_Destroy(msg_gameover);
		msg_gameover = NULL;
	}
	SDL_Quit();
}

int renderer_Event ()
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
			case SDLK_w:
			case SDLK_k:
			case SDLK_UP:
				game_InputMove(UP);
				break;
			case SDLK_d:
			case SDLK_l:
			case SDLK_RIGHT:
				game_InputMove(RIGHT);
				break;
			case SDLK_s:
			case SDLK_j:
			case SDLK_DOWN:
				game_InputMove(DOWN);
				break;
			case SDLK_a:
			case SDLK_h:
			case SDLK_LEFT:
				game_InputMove(LEFT);
				break;
			case SDLK_TAB:
				game_InputRotate(1);
				break;
			case SDLK_0:
			case SDLK_KP_0:
				game_InputSelect(9);
				break;
			case SDLK_1:
			case SDLK_KP_1:
				game_InputSelect(0);
				break;
			case SDLK_2:
			case SDLK_KP_2:
				game_InputSelect(1);
				break;
			case SDLK_3:
			case SDLK_KP_3:
				game_InputSelect(2);
				break;
			case SDLK_4:
			case SDLK_KP_4:
				game_InputSelect(3);
				break;
			case SDLK_5:
			case SDLK_KP_5:
				game_InputSelect(4);
				break;
			case SDLK_6:
			case SDLK_KP_6:
				game_InputSelect(5);
				break;
			case SDLK_7:
			case SDLK_KP_7:
				game_InputSelect(6);
				break;
			case SDLK_8:
			case SDLK_KP_8:
				game_InputSelect(7);
				break;
			case SDLK_9:
			case SDLK_KP_9:
				game_InputSelect(8);
				break;
			}
		}
	}

	return EVENTNONE;
}

int renderer_Init (int w, int h, int sqr)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return sdl_error("SDL could not initialize");
	if (TTF_Init() < 0) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return 2;
	}

	screen_width = w;
	screen_height = h;
	g_square = sqr;

	window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screen_width,
			screen_height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (window == NULL)
		return sdl_error("Window could not be created");

	renderer = SDL_CreateRenderer(
			window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		return sdl_error("Renderer could not be created");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// screen = SDL_GetWindowSurface(window);

	font_regular = TTF_OpenFont("assets/SourceSansPro-Regular.ttf", 14);
	font_strong = TTF_OpenFont("assets/SourceSansPro-Regular.ttf", 28);
	msg_gameover = msg_New(font_strong, white, "GAME OVER");
	return 0;
}

void renderer_Render ()
{
	renderer_RenderGrid();
	renderer_RenderFood();
	struct list_item *i = g_snakelist->head;
	while (i) {
		renderer_RenderSnake(i->id, i->value);
		i = i->next;
	}
	renderer_RenderScore();
	if (g_gameover)
		renderer_RenderGameOver();
	SDL_RenderPresent(renderer);
	SDL_Delay(128);
}

/* private */

SDL_Rect get_rect (struct vector v)
{
	SDL_Rect rekt = {
		v.x * g_square + 1,
		v.y * g_square + 1,
		g_square - 1,
		g_square - 1};
	return rekt;
}

void log_error (const char str[], const char err[])
{
	fprintf(stderr, "ERROR: %s!\n", str);
	fprintf(stderr, "%s\n", err);
}

int sdl_error (const char str[])
{
	log_error(str, SDL_GetError());
	return 1;
}

void renderer_RenderFood ()
{
	SDL_Rect rect = get_rect(g_food);
	if (g_poison)
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	else
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
}

void renderer_RenderGrid ()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);

	int zero = g_square;
	int right = screen_width - g_square;
	int bottom = screen_height - g_square;

	for (int i = 0; i <= g_width; i++) {
		int pos = g_square * (i + 1);
		SDL_RenderDrawLine(renderer, pos, zero, pos, bottom);
	}

	for (int i = 0; i <= g_height; i++) {
		int pos = g_square * (i + 1);
		SDL_RenderDrawLine(renderer, zero, pos, right, pos);
	}
}

void renderer_RenderSnake (int id, struct snake *s)
{
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

	SDL_Rect rect = get_rect(s->head->position);
	char str[4];
	sprintf(str, "%d", (id + 1) % 10);
	struct msg *m = msg_New(font_regular, black, str);
	msg_Render(m, rect.x + rect.w / 2 , rect.y + rect.h / 2);
	msg_Destroy(m);
}

static void renderer_RenderScore ()
{
	char str[16];
	sprintf(str, "SCORE %03d", g_score);
	struct msg *score = msg_New(font_regular, white, str);
	msg_Render(score, score->w / 2 + g_square, score->h / 2);
	msg_Destroy(score);
}

void renderer_RenderGameOver ()
{
	int padding = 16;
	int w = msg_gameover->w + padding * 2;
	int h = msg_gameover->h + padding;
	SDL_Rect rect = {
		.x = (screen_width - w) / 2,
		.y = (screen_height - h) / 2,
		.w = w + 2,
		.h = h + 2
	};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xCC);
	SDL_RenderFillRect(renderer, &rect);
	msg_Render(msg_gameover, screen_width / 2, screen_height / 2);
}

struct msg *msg_New (TTF_Font *f, SDL_Color c, const char *str)
{
	int w, h;
	struct msg *t;
	t = malloc(sizeof(*t));

	SDL_Surface *srf = TTF_RenderText_Blended(f, str, c);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_FreeSurface(srf);
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	t->t = texture;
	t->w = w;
	t->h = h;
	return t;
}

void msg_Destroy (struct msg *t)
{
	SDL_DestroyTexture(t->t);
	free(t);
}

void msg_Render (struct msg *t, int x, int y)
{
	SDL_Rect r = {
		.x = x - t->w / 2,
		.y = y - t->h / 2,
		.w = t->w,
		.h = t->h
	};
	SDL_RenderCopy(renderer, t->t, NULL, &r);
}
