#include "raylib_compat.h"
#include <math.h>

#include "shapes.h"
#include "vectors.h"

const int width = 800;
const int height = 600;
const float near = 0.1f, far = 1000.0f;
const float fov_degrees = 90.0f;

bool all_verts_on_screen(Vertex* ptr[3])
{
    if(
        ptr[0]->x < 0 &&
        ptr[1]->x < 0 &&
        ptr[2]->x < 0 
    ) 
    {return false;}
    if(
        ptr[0]->x > width &&
        ptr[1]->x > width &&
        ptr[2]->x > width 
    )
    {return false;}
    if(
        ptr[0]->y < 0 &&
        ptr[1]->y < 0 &&
        ptr[2]->y < 0 
    ) 
    {return false;}
    if(
        ptr[0]->y > height &&
        ptr[1]->y > height &&
        ptr[2]->y > height 
    )
    {return false;}
    return true;
}

void raster_object(Mesh *object, myCamera *camera, rotation_mats *rotation, mat4x4 *projection){
    const float aspect = (float)height / (float)width;
    const float focal = 1.0f / tanf(fov_degrees * 0.5f * (M_PI / 180.0f));

    // loop through triangles in object
    for (size_t i = 0; i < vector_size(object); i++) {

        Face transformed = vector_get(object, i); // note: not actually transformed yet

        Vertex *transformed_vertices_ptr[3] = {
            &transformed.v1,
            &transformed.v2,
            &transformed.v3
        };

        bool drawable = true;

        // Model -> rotated model -> world/view space. Keep the mesh unchanged.

        // loop through points
        for (int j = 0; j < 3; j++) {
            // temp point
            Vertex point = *transformed_vertices_ptr[j];

            // Rotate in X-Axis, Y-Axis, Z-Axis
            if ( 
               !MultiplyMatrixVector(point, &point, &rotation->z) 
            || !MultiplyMatrixVector(point, &point, &rotation->x)  
            || !MultiplyMatrixVector(point, &point, &rotation->y) 
                ) 
            {
                drawable = false; break;
            }

            // Offset into the screen
            point.z += 3.0f; 

            // Translate position into camera space
            point.x -= camera->pos.x;
            point.y -= camera->pos.y;
            point.z -= camera->pos.z;

            // Inverse rotation matrices for camera angles
            mat4x4 camera_rotation_x = MakeInverseRotationX(camera->angle.theta);
            mat4x4 camera_rotation_y = MakeInverseRotationY(camera->angle.phi);

            // Apply inverse rotation matrices
            if ( 
                    !MultiplyMatrixVector(point, &point, &camera_rotation_y)
                || !MultiplyMatrixVector(point, &point, &camera_rotation_x)
                )
            {
                drawable = false; break;
            }

            // Apply translated/transformed point to output
            *transformed_vertices_ptr[j] = point; 
        }
        // check drawable before continuing
        if (!drawable) { continue; } 

        // calculate triangle normal 
        Vertex normal, line1, line2;

        line1.x = transformed.v2.x - transformed.v1.x;
        line1.y = transformed.v2.y - transformed.v1.y;
        line1.z = transformed.v2.z - transformed.v1.z;
        
        line2.x = transformed.v3.x - transformed.v1.x;
        line2.y = transformed.v3.y - transformed.v1.y;
        line2.z = transformed.v3.z - transformed.v1.z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        //normal length
        float l = sqrtf(powf(normal.x,2.0f) + powf(normal.y,2.0f) + powf(normal.z,2.0f));

        if (l == 0) {continue;}
        
        //normalise
        normal.x /= l;
        normal.y /= l;
        normal.z /= l;
        

        // cull faces pointing away
        if (
            normal.x * (transformed.v1.x) +
            normal.y * (transformed.v1.y) +
            normal.z * (transformed.v1.z) > 0.0f
        )
        { continue; }

        // loop through points
        for (int j = 0; j < 3; j++) {

            // Temp object for ease of reading
            Vertex point = *transformed_vertices_ptr[j];

            // project onto screen
            if (point.z < near || point.z > far || !MultiplyMatrixVector(point, transformed_vertices_ptr[j], projection)) 
            { 
                drawable = false; break;
            }

            // Scale triangle into view
            transformed_vertices_ptr[j]->x = (transformed_vertices_ptr[j]->x + 1.0f) * 0.5f * width;
            transformed_vertices_ptr[j]->y = (transformed_vertices_ptr[j]->y + 1.0f) * 0.5f * height;

        }

        // Draw triangle
        if (!drawable) {continue;}

        if (!all_verts_on_screen(transformed_vertices_ptr))
        {
            continue;
        }


        fill_triangle(&transformed, WHITE);
        draw_triangle(&transformed, BLACK);
  }
}
