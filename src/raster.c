#include "raster.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "vectors.h"

void drawLine(int xi, int xf, int yi, int yf, Color colour, int thickness);
void drawXLine(int xi, int xf, int y, Color colour, int thickness);
void drawYLine(int x, int yi, int yf, Color colour, int thickness);
void drawRect(int cx, int cy, int w, int h, Color colour, int thickness);
void drawTriangle(IntVector *vec1,IntVector *vec2,IntVector *vec3, Color colour, int thickness);

// generalised Bresenham algorithm
void drawLine(int xi, int xf, int yi, int yf, Color colour, int thickness)
{
    // handle verticle / horizontal lines
    if(xi==xf){drawYLine(xi,yi,yf,colour,thickness);}
    else if(yi==yf){drawXLine(xi,xf,yi,colour,thickness);}
    else {
        if (thickness != 1)
        {
            for (int n = 0; n < thickness; n++)
            {
                drawLine(
                    xi + (pow(-1,n+1) * (int)floor(1 + n * 0.5)), 
                    xf + (pow(-1,n+1) * (int)floor(1 + n * 0.5)), 
                    yi, yf, colour, 1);
            }
        }
        int dx = abs(xf - xi), dy = -abs(yf - yi);
        int sx = xi < xf ? 1 : -1;
        int sy = yi < yf ? 1 : -1;
        int error = dx + dy;

        while (1)
        {
            DrawPixel(xi, yi, colour);
            if (xi == xf && yi == yf) { break; }
            int e2 = 2 * error;
            if (e2 >= dy)
            {
                error += dy;
                xi += sx;
            } 
            if (e2 <= dx)
            {
                error += dx;
                yi += sy;
            }
        }
    }
}

void drawXLine(int xi, int xf, int y, Color colour, int thickness)
{
    if (thickness <= 0) { return; }
    if (xi > xf) {
        int temp = xi;
        xi = xf;
        xf = temp;
    }

    // Even widths put the extra pixel on the negative side of the centre.
    int before = thickness / 2;
    int after = (thickness - 1) / 2;

    // Extend every row at both ends, filling square corners at line joins.
    for (int row = y - before; row <= y + after; row++) {
        for (int x = xi - before; x <= xf + after; x++) {
            DrawPixel(x, row, colour);
        }
    }
}

void drawYLine(int x, int yi, int yf, Color colour, int thickness)
{
    if (thickness <= 0) { return; }
    if (yi > yf) {
        int temp = yi;
        yi = yf;
        yf = temp;
    }

    // Use the same pixel alignment as drawXLine so their corners meet.
    int before = thickness / 2;
    int after = (thickness - 1) / 2;

    for (int column = x - before; column <= x + after; column++) {
        for (int y = yi - before; y <= yf + after; y++) {
            DrawPixel(column, y, colour);
        }
    }
}

void drawRect(int cx, int cy, int w, int h, Color colour, int thickness)
{
    if (thickness != 1){
        ;
    }
    drawLine(cx-w/2, cx-w/2, cy-h/2,cx+w/2,colour,thickness);
    drawLine(cx+w/2, cx+w/2, cy-h/2,cx+w/2,colour,thickness);
    drawLine(cx-w/2, cx+w/2, cy-h/2,cx-w/2,colour,thickness);
    drawLine(cx-w/2, cx+w/2, cy+h/2,cx+w/2,colour,thickness);
}

void drawTriangle(IntVector *vec1,IntVector *vec2,IntVector *vec3, Color colour, int thickness) 
{
    drawLine(
        vector_get(vec1, 0), vector_get(vec2, 0),
        vector_get(vec1, 1), vector_get(vec2, 1),
        colour, thickness
    );
    drawLine(
        vector_get(vec2, 0), vector_get(vec3, 0),
        vector_get(vec2, 1), vector_get(vec3, 1),
        colour, thickness
    );
    drawLine(
        vector_get(vec3, 0), vector_get(vec1, 0),
        vector_get(vec3, 1), vector_get(vec1, 1),
        colour, thickness
    );
}
