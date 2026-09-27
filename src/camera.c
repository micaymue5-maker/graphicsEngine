#include <math.h>
#include "camera.h"
#include "vectors.h"
#include "shapes.h"


// Screen projection matrix
mat4x4 projection = {0};

// Turns an empty mat4x4 into a screen projection matrix
void make_projection_matrix(mat4x4* projection)
{
    const float focal = 1.0f / tanf(fov_degrees * 0.5f * (3.1415927f / 180.0f));
    
    projection->m[0][0] = aspect * focal;
    projection->m[1][1] = focal;
    projection->m[2][2] = far / (far - near);
    projection->m[3][2] = -far * near / (far - near);
    projection->m[2][3] = 1.0f;
}

float get_camera_angle_theta(Camera3d camera)
{
    // Norm of z/x line
    float norm = sqrtf(
        powf(camera.target.x - camera.position.x, 2.0f) + 
        powf(camera.target.z - camera.position.z, 2.0f)
    );

    // Norm of y line
    float normy = fabsf(camera.target.y - camera.position.y);

    // Handle trivial cases
    if ( norm == 0 )
    {
        if (normy > 0) {return 90.0f;}
        return -90.0f;
    }
    
    return atanf(normy / norm);
}

float get_camera_angle_phi(Camera3d camera)
{
    // Norm of z/y line
    float norm = sqrtf(
        powf(camera.target.z - camera.position.z, 2.0f) +
        powf(camera.target.y - camera.target.y, 2.0f)
    );
    // Norm of x line
    float normx = fabsf(camera.target.x - camera.position.x);

    // Handle trivial cases
    if ( norm == 0 )
    {
        if (normx > 0) {return 90.0f;}
        return -90.0f;
    }
    
    return atanf(normx / norm);
}

void temp(mesh* myMesh, Camera3d camera)
{
    make_projection_matrix(&projection);

    for (size_t i = 0; i < vector_size(&myMesh->tris); i++) 
    {

        triangle transformed = vector_get(&myMesh->tris, i); // note: not actually transformed yet
        triangle projected;
        bool drawable = true;

        // Model -> rotated model -> world/view space. Keep the mesh unchanged.

        // loop through points
        for (int j = 0; j < 3; j++) {
            // temp point
            vec3d point = transformed.p[j];

            // Move cube centre to (0,0,0)
            point.x -= 0.5f;
            point.y -= 0.5f;
            point.z -= 0.5f;


            // Offset into the screen
            point.z += 3.0f; 

            // Translate position into camera space
            point.x -= camera.position.x;
            point.y -= camera.position.y;
            point.z -= camera.position.z;

            // get camera rotation
            float camera_theta = get_camera_angle_theta(camera);
            float camera_phi = get_camera_angle_phi(camera);

            // Inverse rotation matrices for camera angles
            mat4x4 camera_rotation_x = MakeInverseRotationX(camera_theta);
            mat4x4 camera_rotation_y = MakeInverseRotationY(camera_phi);

            // Apply inverse rotation matrices
            if ( 
                    !MultiplyMatrixVector(point, &point, &camera_rotation_y)
                || !MultiplyMatrixVector(point, &point, &camera_rotation_x)
            )
            {
                drawable = false; break;
            }

            // Apply translated/transformed point to output
            transformed.p[j] = point; 
        }
        // check drawable before continuing
        if (!drawable) { continue; } 

        // calculate triangle normal 
        vec3d normal, line1, line2;

        line1.x = transformed.p[1].x - transformed.p[0].x;
        line1.y = transformed.p[1].y - transformed.p[0].y;
        line1.z = transformed.p[1].z - transformed.p[0].z;
        
        line2.x = transformed.p[2].x - transformed.p[0].x;
        line2.y = transformed.p[2].y - transformed.p[0].y;
        line2.z = transformed.p[2].z - transformed.p[0].z;

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
        

        // culling
        if (
            normal.x * (transformed.p[0].x) +
            normal.y * (transformed.p[0].y) +
            normal.z * (transformed.p[0].z) > 0.0f
        )
        { continue; }

        // loop through points
        for (int j = 0; j < 3; j++) {

            // Temp object for ease of reading
            vec3d point = transformed.p[j];

            // project onto screen
            if (point.z < near || point.z > far || !MultiplyMatrixVector(point, &projected.p[j], &projection)) 
            { 
                drawable = false; break;
            }

            // Scale triangle into view
            projected.p[j].x = (projected.p[j].x + 1.0f) * 0.5f * width;
            projected.p[j].y = (projected.p[j].y + 1.0f) * 0.5f * height;

        }

        // Draw triangle
        if (drawable) { 
            fill_triangle(&projected, WHITE);
            draw_triangle(&projected, BLACK);
        }
    }
}
