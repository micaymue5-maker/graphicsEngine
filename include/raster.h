#ifndef RASTER_H
#define RASTER_H

#include <raylib.h>

// One-pixel-wide Bresenham line, including both endpoints.
// Screen coordinates: x increases right, y increases down.
// Call inside BeginDrawing()/EndDrawing(). Prefix avoids raylib name collisions.
void raster_draw_line(int x0, int y0, int x1, int y1, Color colour);

#endif
