#ifndef MOVEMENT_H
#define MOVEMENT_H
/*
 * 2D movement directions.
 */
enum direction { UP, RIGHT, DOWN, LEFT };

/*
 * 2D turning directions.
 */
enum directive { KEEP, PORT, STARBOARD };

/*
 * Number of squares in width.
 */
extern int g_width;

/*
 * Number of squares in width.
 */
extern int g_height;

/*
 * Greatest number a square position can get
 */
extern int g_maxpos;

/*
 * Returns the new position after moving in one direction
 */
int movement (int position, int direction);

/*
 * Returns the new direction after turning in +-90 degrees.
 */
int turn (int direction, int directive);
#endif
