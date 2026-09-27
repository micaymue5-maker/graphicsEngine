#ifndef SHAPES_H
#define SHAPES_H

#include <raylib.h>
#include "vectors.h"

// Returns an owned triangle list. Release it with vector_free(&cube.tris).
mesh get_cube(void);
// Draw a 2d triangle from pixel coordinates
void draw_triangle(const triangle *obj, Color colour);
// fills in a 2d triangle with hex colour from pixel coordinates
void fill_triangle(triangle *obj, Color colour);
#endif
