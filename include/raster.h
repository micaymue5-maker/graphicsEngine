#ifndef RASTER_H
#define RASTER_H

#include <raylib.h>
#include <vectors.h>

// Draw during a frame, between BeginDrawing() and EndDrawing().
// Coordinates start at the top-left: x increases right, y increases down.
void raster_put_pixel(int x, int y, Color color);
void drawLine(int xi, int xf, int yi, int yf, Color colour, int thickness);
void drawRect(int cx, int cy, int w, int h, Color colour, int thickness);
void drawTriangle(IntVector *vec1, IntVector *vec2, IntVector *vec3, Color colour, int thickness);

// Add declarations for your own line and triangle functions here.

#endif
