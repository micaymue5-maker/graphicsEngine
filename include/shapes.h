#ifndef SHAPES_H
#define SHAPES_H

#include "raylib_compat.h"
#include "vectors.h"


// Returns an owned triangle list. Release it with vector_free(&cube.tris).
Mesh get_cube(void);
Mesh get_cube_at_pos(float x, float y, float z, float scale);

// Draw a triangle already projected and converted to screen-pixel coordinates.
void draw_triangle(const Face *obj, Color colour);
void fill_triangle(Face *obj, Color colour);
void scale_mesh(Mesh* output, float scale);
void move_mesh (Mesh* output, float dx, float dy, float dz);
void rotate_mesh(Mesh* output, float rx, float ry, float rz);
#endif

