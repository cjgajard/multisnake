#include <SDL2/SDL.h>

/*
 * Screen dimensions
 */
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

/*
 * Partial functions.
 */
bool __init();
void renderMap();

/*
 * The window we'll be rendering to.
 */
extern SDL_Window *g_window;

/*
 * The surface contained by the window.
 */
extern SDL_Surface *g_screen_surface;

/*
 * The default renderer.
 */
extern SDL_Renderer *g_renderer;

/*
 * Size in pixels of each square.
 */
extern int g_square;

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

/*
 * Returns a SDL rectangle representing a square with position and dimentions.
 */
SDL_Rect get_rect(int square_id);
