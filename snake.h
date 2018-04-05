/*
 * Stores a fraction of a snake string.
 */
struct snake {
	// struct snake *previous;
	struct snake *next;
	int *directive;
	int direction;
	int next_dir;
	int position;
};

/*
 * updates the position of a snake point
 */
void move_snake(struct snake *snake);
