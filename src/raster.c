#include "raster.h"

#include <stdlib.h>

void raster_draw_line(int x0, int y0, int x1, int y1, Color colour)
{
    // Generalised Bresenham: the same loop handles every octant and axis lines.
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    for (;;) {
        DrawPixel(x0, y0, colour);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * error;
        if (e2 >= dy) {
            error += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            error += dx;
            y0 += sy;
        }
    }
}
