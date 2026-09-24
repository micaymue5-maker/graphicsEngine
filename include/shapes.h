#ifndef SHAPES_H
#define SHAPES_H

#include <raylib.h>
#include "vectors.h"

// Lowercase avoids raylib's unrelated Mesh type.
typedef struct {
    TriVector tris;
} mesh;

// Returns an owned triangle list. Release it with vector_free(&cube.tris).
mesh get_cube(void);

// Draw a triangle already projected and converted to screen-pixel coordinates.
void draw_triangle(const triangle *obj, Color colour);
void fill_triangle(triangle *obj, Color colour);
#endif
