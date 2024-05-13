// A position in a 2D cartesian coordinate system.
struct posn;

// posn_create(x, y) creates a position with x as the x- and y as the
//   y-coordinate.
// effects: allocates heap memory (client must call posn_destroy)
// time:    O(1)
struct posn *posn_create(int x, int y);

// posn_destroy(pos) releases all resources used by *pos.
// effects: invalidates *pos
// time:    O(1)
void posn_destroy(struct posn *pos);

// posn_print(pos) prints the position *pos to output.
// effects: writes to output
// time:    O(1)
void posn_print(const struct posn *pos);