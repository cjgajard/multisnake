#ifndef MOVEMENT_H
#define MOVEMENT_H
/*
 * 2D position.
 */
struct vector {
	int x;
	int y;
};
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
struct vector movement (enum direction d, struct vector v);
/*
 * Returns the new direction after turning in +-90 degrees.
 */
int turn (enum directive i, enum direction d);
/*
 */
int vector_Eq(struct vector this, struct vector other);
#endif
