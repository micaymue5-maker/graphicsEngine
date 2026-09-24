#include "shapes.h"
#include "raster.h"

#include <math.h>

mesh get_cube(void)
{
    const triangle cube[12] = {
        // SOUTH
        { .p = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f}
        }},
        { .p = {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        }},

        // EAST
        { .p = {
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        }},
        { .p = {
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f}
        }},

        // NORTH
        { .p = {
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f}
        }},
        { .p = {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 1.0f}
        }},

        // WEST
        { .p = {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f}
        }},
        { .p = {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}
        }},

        // TOP
        { .p = {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}
        }},
        { .p = {
            {0.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f}
        }},

        // BOTTOM
        { .p = {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f}
        }},
        { .p = {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        }}
    };
    mesh cube_mesh;
    vector_init(&cube_mesh.tris);
    for (size_t i = 0; i < sizeof cube / sizeof cube[0]; i++) {
        vector_push(&cube_mesh.tris, cube[i]);
    }
    return cube_mesh;
}

void draw_triangle(const triangle *obj, Color colour)
{
    for (int i = 0; i < 3; i++) {
        vec3d start = obj->p[i];
        vec3d end = obj->p[(i + 1) % 3];
        raster_draw_line((int)roundf(start.x), (int)roundf(start.y),
                         (int)roundf(end.x), (int)roundf(end.y), colour);
    }
}

void fill_top_flat_triangle(const triangle *obj, Color colour)
{
    float invslope1 = (obj->p[2].x - obj->p[0].x) / (obj->p[2].y - obj->p[0].y);
    float invslope2 = (obj->p[2].x - obj->p[1].x) / (obj->p[2].y - obj->p[1].y);

    float curx1, curx2 = obj->p[2].x;

    for (int scanLineY = obj->p[2].y; scanLineY > obj->p[0].y; scanLineY--)
    {
        raster_draw_line((int)curx1, scanLineY, (int)curx2, scanLineY, colour);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void fill_bottom_flat_triangle(const triangle *obj, Color colour)
{
    float invslope1 = (obj->p[1].x - obj->p[0].x) / (obj->p[1].y - obj->p[0].y);
    float invslope2 = (obj->p[2].x - obj->p[0].x) / (obj->p[2].y - obj->p[0].y);

    float curx1, curx2 = obj->p[0].x;

    for (int scanLineY = obj->p[0].y; scanLineY > obj->p[1].y; scanLineY--)
    {
        raster_draw_line((int)curx1, scanLineY, (int)curx2, scanLineY, colour);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void sort_vertices_ascending_by_y(triangle *obj)
{
    float vec1 = obj->p[0].y;
    float vec2 = obj->p[1].y;
    float vec3 = obj->p[2].y;
    vec3d temp;
    if (obj->p[0].y > obj->p[1].y) {
        temp = obj->p[1];
        obj->p[1] = obj->p[0];
        obj->p[0] = temp;
    }
    if (obj->p[1].y > obj->p[2].y) {
        temp = obj->p[2];
        obj->p[2] = obj->p[1];
        obj->p[1] = temp;
    }
}

void fill_triangle(const triangle *obj, Color colour)
{
    sort_vertices_ascending_by_y(obj);

    // check for trivial case of bottom-flat triangle
    if (obj->p[1].y == obj->p[2].y) 
    {
        fill_bottom_flat_triangle(obj, colour);
    }

    // Check for trivial case of top-flat triangle
    else if (obj->p[0].y == obj->p[1].y)
    {
        fill_top_flat_triangle(obj, colour);
    }
    else
    {
        vec3d v4 = {
            .x = (int)(obj->p[0].x + ( (obj->p[1].y - obj->p[0].y) / (obj->p[2].y - obj->p[0].y) ) * (obj->p[2].x - obj->p[0].x)),
            .y = obj->p[1].y,
            .z = 0.0f
        };

        triangle bottom = {
            .p[0] = obj->p[0],
            .p[1] = obj->p[1],
            .p[2] = v4
        };
        triangle top = {
            .p[0] = obj->p[1],
            .p[1] = v4,
            .p[2] = obj->p[2]
        };

        fill_bottom_flat_triangle(&bottom, colour);
        fill_top_flat_triangle(&top, colour);
    }
}
