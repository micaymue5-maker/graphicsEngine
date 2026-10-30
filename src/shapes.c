#include "shapes.h"
#include "raster.h"

#include <math.h>
#include <stdio.h>

Mesh get_cube(void)
{
    Face cube[12] = {
        // SOUTH
        {
            .v1 = {-0.5f, -0.5f, -0.5f},
            .v2 = {-0.5f, 0.5f, -0.5f},
            .v3 = {0.5f, 0.5f, -0.5f}
        },
        {
            .v1 = {-0.5f, -0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, -0.5f},
            .v3 = {0.5f, -0.5f, -0.5f}
        },

        // EAST
        {
            .v1 = {0.5f, -0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, -0.5f},
            .v3 = {0.5f, 0.5f, 0.5f}
        },
        {
            .v1 = {0.5f, -0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, 0.5f},
            .v3 = {0.5f, -0.5f, 0.5f}
        },

        // NORTH
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {0.5f, 0.5f, 0.5f},
            .v3 = {-0.5f, 0.5f, 0.5f}
        },
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, 0.5f, 0.5f},
            .v3 = {-0.5f, -0.5f, 0.5f}
        },

        // WEST
        {
            .v1 = {-0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, 0.5f, 0.5f},
            .v3 = {-0.5f, 0.5f, -0.5f}
        },
        {
            .v1 = {-0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, 0.5f, -0.5f},
            .v3 = {-0.5f, -0.5f, -0.5f}
        },

        // TOP
        {
            .v1 = {-0.5f, 0.5f, -0.5f},
            .v2 = {-0.5f, 0.5f, 0.5f},
            .v3 = {0.5f, 0.5f, 0.5f}
        },
        {
            .v1 = {-0.5f, 0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, 0.5f},
            .v3 = {0.5f, 0.5f, -0.5f}
        },

        // BOTTOM
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, -0.5f, 0.5f},
            .v3 = {-0.5f, -0.5f, -0.5f}
        },
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, -0.5f, -0.5f},
            .v3 = {0.5f, -0.5f, -0.5f}
        }
    };
    for (Face* ptr = cube; ptr != cube + 12; ptr++)
    {
        ptr->normal = (Vertex){
            .x = (
                (ptr->v2.y - ptr->v1.y) * (ptr->v3.z - ptr->v1.z) - 
                (ptr->v2.z - ptr->v1.z) * (ptr->v3.y - ptr->v1.y)
            ),
            .y = (
                (ptr->v2.z - ptr->v1.z) * (ptr->v3.x - ptr->v1.x) - 
                (ptr->v2.x - ptr->v1.x) * (ptr->v3.z - ptr->v1.z)    
            ),
            .z = (
                (ptr->v2.x - ptr->v1.x) * (ptr->v3.y - ptr->v1.y) - 
                (ptr->v2.y - ptr->v1.y) * (ptr->v3.x - ptr->v1.x)    
            )
        };
    }
    Mesh cube_mesh;
    vector_init(&cube_mesh);
    for (size_t i = 0; i < sizeof cube / sizeof cube[0]; i++) {
        vector_push(&cube_mesh, cube[i]);
    }
    return cube_mesh;
}

Mesh get_cube_at_pos(float x, float y, float z, float scale)
{
    Face cube[12] = {
        // SOUTH
        {
            .v1 = {-0.5f, -0.5f, -0.5f},
            .v2 = {-0.5f, 0.5f, -0.5f},
            .v3 = {0.5f, 0.5f, -0.5f}
        },
        {
            .v1 = {-0.5f, -0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, -0.5f},
            .v3 = {0.5f, -0.5f, -0.5f}
        },

        // EAST
        {
            .v1 = {0.5f, -0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, -0.5f},
            .v3 = {0.5f, 0.5f, 0.5f}
        },
        {
            .v1 = {0.5f, -0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, 0.5f},
            .v3 = {0.5f, -0.5f, 0.5f}
        },

        // NORTH
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {0.5f, 0.5f, 0.5f},
            .v3 = {-0.5f, 0.5f, 0.5f}
        },
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, 0.5f, 0.5f},
            .v3 = {-0.5f, -0.5f, 0.5f}
        },

        // WEST
        {
            .v1 = {-0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, 0.5f, 0.5f},
            .v3 = {-0.5f, 0.5f, -0.5f}
        },
        {
            .v1 = {-0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, 0.5f, -0.5f},
            .v3 = {-0.5f, -0.5f, -0.5f}
        },

        // TOP
        {
            .v1 = {-0.5f, 0.5f, -0.5f},
            .v2 = {-0.5f, 0.5f, 0.5f},
            .v3 = {0.5f, 0.5f, 0.5f}
        },
        {
            .v1 = {-0.5f, 0.5f, -0.5f},
            .v2 = {0.5f, 0.5f, 0.5f},
            .v3 = {0.5f, 0.5f, -0.5f}
        },

        // BOTTOM
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, -0.5f, 0.5f},
            .v3 = {-0.5f, -0.5f, -0.5f}
        },
        {
            .v1 = {0.5f, -0.5f, 0.5f},
            .v2 = {-0.5f, -0.5f, -0.5f},
            .v3 = {0.5f, -0.5f, -0.5f}
        }
    };
    for (int i = 0; i < 12; i++)
    {
        // pointer to v1
        Vertex *v_ptr[3] = {
            &cube[i].v1,
            &cube[i].v2,
            &cube[i].v3
        };

        // increment memory address
        for (int v = 0; v < 3; v++)
        {
            v_ptr[v]->x += x;
            v_ptr[v]->y += y;
            v_ptr[v]->z += z;
            
            v_ptr[v]->x *= scale;
            v_ptr[v]->y *= scale;
            v_ptr[v]->z *= scale;
        }
    }
    
    Mesh cube_mesh;
    vector_init(&cube_mesh);
    for (size_t i = 0; i < sizeof cube / sizeof cube[0]; i++) {
        vector_push(&cube_mesh, cube[i]);
    }
    return cube_mesh;
}

void scale_mesh(Mesh* output, float scale)
{
    for (int i = 0; i < (int)output->size; i++)
    {
        output->data[i].v1.x *= scale;
        output->data[i].v1.y *= scale;
        output->data[i].v1.z *= scale;

        output->data[i].v2.x *= scale;
        output->data[i].v2.y *= scale;
        output->data[i].v2.z *= scale;

        output->data[i].v3.x *= scale;
        output->data[i].v3.y *= scale;
        output->data[i].v3.z *= scale;
    }
}

void move_mesh (Mesh* output, float dx, float dy, float dz)
{
    for (int i = 0; i < (int)output->size; i++)
    {
        output->data[i].v1.x += dx;
        output->data[i].v1.y += dy;
        output->data[i].v1.z += dz;

        output->data[i].v2.x += dx;
        output->data[i].v2.y += dy;
        output->data[i].v2.z += dz;

        output->data[i].v3.x += dx;
        output->data[i].v3.y += dy;
        output->data[i].v3.z += dz;
    }
}

void rotate_mesh(Mesh* output, float rx, float ry, float rz)
{
    struct rotation {
        const mat4x4 x;
        const mat4x4 y;
        const mat4x4 z;
    };
    struct rotation r = {
        .x = MakeRotationX(rx * 0.50f),
        .y = MakeRotationY(ry * 0.25f),
        .z = MakeRotationZ(rz),
    };

    for (int i = 0; i < (int)output->size; i++)
    {
        MultiplyMatrixVector(output->data[i].v1,&(output->data[i].v1), &r.x);
        MultiplyMatrixVector(output->data[i].v1,&(output->data[i].v1), &r.y);
        MultiplyMatrixVector(output->data[i].v1,&(output->data[i].v1), &r.z);

        MultiplyMatrixVector(output->data[i].v2,&(output->data[i].v2), &r.x);
        MultiplyMatrixVector(output->data[i].v2,&(output->data[i].v2), &r.y);
        MultiplyMatrixVector(output->data[i].v2,&(output->data[i].v2), &r.z);
        
        MultiplyMatrixVector(output->data[i].v3,&(output->data[i].v3), &r.x);
        MultiplyMatrixVector(output->data[i].v3,&(output->data[i].v3), &r.y);
        MultiplyMatrixVector(output->data[i].v3,&(output->data[i].v3), &r.z);
    }
}


void draw_triangle(const Face *obj, Color colour)
{
    const Vertex *v_ptr[3] = {
        &obj->v1,
        &obj->v2,
        &obj->v3
    };

    for (int v = 0; v < 3; v++) // loop through vertices
    {
        Vertex start = *v_ptr[v];
        Vertex end = *v_ptr[(v+1) % 3];

        raster_draw_line((int)roundf(start.x), (int)roundf(start.y),
                        (int)roundf(end.x), (int)roundf(end.y), colour);
    }
}

void fill_top_flat_triangle(const Face *obj, Color colour)
{
    if ((obj->v3.y - obj->v1.y) != 0 && (obj->v3.y - obj->v2.y) != 0)
    {
        float invslope1 = (obj->v3.x - obj->v1.x) / (obj->v3.y - obj->v1.y);
        float invslope2 = (obj->v3.x - obj->v2.x) / (obj->v3.y - obj->v2.y);

        // account for fractional y-coordinates
        float curx1 = obj->v3.x + (floorf(obj->v3.y) - obj->v3.y) * invslope1;
        float curx2 = obj->v3.x + (floorf(obj->v3.y) - obj->v3.y) * invslope2;

        for (int scanLineY = floorf(obj->v3.y); scanLineY > obj->v1.y; scanLineY--)
        {
            // apply rounding and check for empty span between rounded values
            if (curx1 > curx2 && floorf(curx1)-ceilf(curx2) >= 0) 
            {
                raster_draw_line((int)floorf(curx1), scanLineY, (int)ceilf(curx2), scanLineY, colour);
            }

            else if (ceilf(curx1) - floorf(curx2) <= 0)           
            {
                raster_draw_line((int)ceilf(curx1), scanLineY, (int)floorf(curx2), scanLineY, colour);
            }
            curx1 -= invslope1;
            curx2 -= invslope2;
        }
    } else {
        raster_draw_line(
            (int)roundf(obj->v1.x), (int)roundf(obj->v1.y), 
            (int)roundf(obj->v2.x), (int)roundf(obj->v2.y),
            colour);
    }
}

void fill_bottom_flat_triangle(const Face *obj, Color colour)
{
    if ((obj->v2.y - obj->v1.y) != 0 && (obj->v3.y - obj->v1.y) != 0)
    {
        float invslope2 = (obj->v2.x - obj->v1.x) / (obj->v2.y - obj->v1.y);
        float invslope1 = (obj->v3.x - obj->v1.x) / (obj->v3.y - obj->v1.y);

        // account for fractional y-coordinates
        float curx1 = obj->v3.x + (floorf(obj->v2.y) - obj->v3.y) * invslope1;
        float curx2 = obj->v2.x + (floorf(obj->v2.y) - obj->v2.y) * invslope2;

        for (int scanLineY = floorf(obj->v2.y); scanLineY > obj->v1.y; scanLineY--)
        {
            if (curx1 > curx2) {raster_draw_line((int)floorf(curx1), scanLineY, (int)ceilf(curx2), scanLineY, colour);}
            else               {raster_draw_line((int)ceilf(curx1), scanLineY, (int)floorf(curx2), scanLineY, colour);}
            curx1 -= invslope1;
            curx2 -= invslope2;
        }
    } else {
        raster_draw_line(
            (int)roundf(obj->v1.x), (int)roundf(obj->v1.y), 
            (int)roundf(obj->v2.x), (int)roundf(obj->v2.y),
            colour);
    }
}

void sort_vertices_ascending_by_y(Face *obj)
{
    Vertex temp;
    if (obj->v2.y > obj->v3.y) {
        temp = obj->v3;
        obj->v3 = obj->v2;
        obj->v2 = temp;
    }
    if (obj->v1.y > obj->v2.y) {
        temp = obj->v2;
        obj->v2 = obj->v1;
        obj->v1 = temp;
    }
    if (obj->v2.y > obj->v3.y) {
        temp = obj->v3;
        obj->v3 = obj->v2;
        obj->v2 = temp;
    }
}

void fill_triangle(Face *obj, Color colour)
{
    sort_vertices_ascending_by_y(obj);

    // check for horizontal line
    if ( obj->v1.y == obj->v3.y )
    {
        raster_draw_line(
            (int)roundf(obj->v1.x), (int)roundf(obj->v1.y), 
            (int)roundf(obj->v2.x), (int)roundf(obj->v2.y),
            colour);
        raster_draw_line(
            (int)roundf(obj->v3.x), (int)roundf(obj->v3.y), 
            (int)roundf(obj->v2.x), (int)roundf(obj->v2.y),
            colour);
    }

    // check for trivial case of bottom-flat triangle
    else if (obj->v2.y == obj->v3.y) 
    {
        fill_bottom_flat_triangle(obj, colour);
    }

    // Check for trivial case of top-flat triangle
    else if (obj->v1.y == obj->v2.y)
    {
        fill_top_flat_triangle(obj, colour);
    }
    else 
    {
        Vertex v4 = {
            .x = obj->v1.x + ( (obj->v2.y - obj->v1.y) / (obj->v3.y - obj->v1.y) ) * (obj->v3.x - obj->v1.x),
            .y = obj->v2.y,
            .z = 0.0f
        };

        Face bottom = {
            .v1 = obj->v1,
            .v2 = obj->v2,
            .v3 = v4
        };
        Face top = {
            .v1 = obj->v2,
            .v2 = v4,
            .v3 = obj->v3
        };

        fill_bottom_flat_triangle(&bottom, colour);
        fill_top_flat_triangle(&top, colour);
    }
}
