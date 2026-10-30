#include "raylib_compat.h"
#include <math.h>

#include "shapes.h"
#include "vectors.h"

const int width = 800;
const int height = 600;
const float near = 0.1f, far = 1000.0f;
const float fov_degrees = 90.0f;

bool all_verts_on_screen(Vertex* v1, Vertex* v2, Vertex* v3)
{
    if(
        v1->x < 0 &&
        v2->x < 0 &&
        v3->x < 0 
    ) 
    {return false;}
    if(
        v1->x > width &&
        v2->x > width &&
        v3->x > width 
    )
    {return false;}
    if(
        v1->y < 0 &&
        v2->y < 0 &&
        v3->y < 0 
    ) 
    {return false;}
    if(
        v1->y > height &&
        v2->y > height &&
        v3->y > height 
    )
    {return false;}
    return true;
}

bool mat_is_equal(mat4x4 a, mat4x4 b, float tol)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (fabsf(a.m[i][j] - b.m[i][j]) >= tol){return false;}
        }
    }
    return true;
}

void raster_object(Mesh *object, myCamera *camera, rotation_mats *rotation, mat4x4 *projection){
    const float aspect = (float)height / (float)width;
    const float focal = 1.0f / tanf(fov_degrees * 0.5f * (M_PI / 180.0f));

    mat4x4 eye4x4 = {0};
    eye4x4.m[0][0] = eye4x4.m[1][1] = eye4x4.m[2][2] = eye4x4.m[3][3] = 1;

    // loop through triangles in object
    for (size_t i = 0; i < vector_size(object); i++) {

        Face transformed = vector_get(object, i); // note: not actually transformed yet

        Vertex* v1 = &transformed.v1;
        Vertex* v2 = &transformed.v2;
        Vertex* v3 = &transformed.v3;


        bool drawable = true;

        // Model -> rotated model -> world/view space. Keep the mesh unchanged.

        // Rotate in X-Axis, Y-Axis, Z-Axis
        if (
            mat_is_equal(eye4x4, rotation->x, (float)0.01) 
            && 
            mat_is_equal(eye4x4, rotation->x, (float)0.01) 
            && 
            mat_is_equal(eye4x4, rotation->x, (float)0.01)
        )
        {} else if ( 
            !MultiplyMatrixVector(*v1, v1, &rotation->x)  
            ||
            !MultiplyMatrixVector(*v1, v1, &rotation->y) 
            ||
            !MultiplyMatrixVector(*v1, v1, &rotation->z) 
            ||
            !MultiplyMatrixVector(*v2, v2, &rotation->x)  
            ||
            !MultiplyMatrixVector(*v2, v2, &rotation->y) 
            ||
            !MultiplyMatrixVector(*v2, v2, &rotation->z) 
            ||
            !MultiplyMatrixVector(*v3, v3, &rotation->x)  
            ||
            !MultiplyMatrixVector(*v3, v3, &rotation->y) 
            ||
            !MultiplyMatrixVector(*v3, v3, &rotation->z) 
        ) { continue; }

        // Offset into the screen
        v1->z += 3.0f; 
        v2->z += 3.0f; 
        v3->z += 3.0f; 

        // Translate position into camera space
        v1->x -= camera->pos.x;
        v2->x -= camera->pos.x;
        v3->x -= camera->pos.x;

        v1->y -= camera->pos.y;
        v2->y -= camera->pos.y;
        v3->y -= camera->pos.y;

        v1->z -= camera->pos.z;
        v2->z -= camera->pos.z;
        v3->z -= camera->pos.z;


        // Inverse rotation matrices for camera angles
        mat4x4 camera_rotation_x = MakeInverseRotationX(camera->angle.theta);
        mat4x4 camera_rotation_y = MakeInverseRotationY(camera->angle.phi);

        // Apply inverse rotation matrices
        if ( 
            !MultiplyMatrixVector(*v1, v1, &camera_rotation_y)
            || 
            !MultiplyMatrixVector(*v1, v1, &camera_rotation_x)
            ||
            !MultiplyMatrixVector(*v2, v2, &camera_rotation_y)
            || 
            !MultiplyMatrixVector(*v2, v2, &camera_rotation_x)
            ||
            !MultiplyMatrixVector(*v3, v1, &camera_rotation_y)
            || 
            !MultiplyMatrixVector(*v3, v1, &camera_rotation_x)
        )
        { continue; }


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

        if (l == 0) { continue; }
        
        //normalise
        normal.x /= l;
        normal.y /= l;
        normal.z /= l;
        

        // cull faces pointing away
        if (
            normal.x * (transformed.v1.x) +
            normal.y * (transformed.v1.y) +
            normal.z * (transformed.v1.z) > 0.0f
        ){ continue; }

        // project onto screen
        if (
            v1->z < near || v1->z > far || !MultiplyMatrixVector(*v1, v1, projection)
            ||
            v2->z < near || v2->z > far || !MultiplyMatrixVector(*v2, v2, projection)
            ||
            v3->z < near || v3->z > far || !MultiplyMatrixVector(*v3, v3, projection)
        ) 
        { continue; }

        // Scale triangle into view
        v1->x = (v1->x + 1.0f) * 0.5f * width;
        v1->y = (v1->y + 1.0f) * 0.5f * height;

        v2->x = (v2->x + 1.0f) * 0.5f * width;
        v2->y = (v2->y + 1.0f) * 0.5f * height;

        v3->x = (v3->x + 1.0f) * 0.5f * width;
        v3->y = (v3->y + 1.0f) * 0.5f * height;


        // Draw triangle
        if (!drawable) { continue; }

        if (!all_verts_on_screen(v1, v2, v3)) { continue; }


        fill_triangle(&transformed, WHITE);
        draw_triangle(&transformed, BLACK);
  }
}
