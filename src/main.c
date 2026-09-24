#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "shapes.h"
#include "vectors.h"

int main(void)
{
    const int width = 800, height = 600;
    InitWindow(width, height, "Triangle mesh - C + raylib");
    if (!IsWindowReady()) {
        return 1;
    }
    SetTargetFPS(60);

    mesh cube = get_cube(); //cube made up of series of triangles

    vec3d vCamera = {0};
    
    const float near = 0.1f, far = 1000.0f;
    const float fov_degrees = 90.0f;
    const float aspect = (float)height / (float)width;
    const float focal = 1.0f / tanf(fov_degrees * 0.5f * DEG2RAD);

    // screen projection matrix
    mat4x4 projection = {0};
    projection.m[0][0] = aspect * focal;
    projection.m[1][1] = focal;
    projection.m[2][2] = far / (far - near);
    projection.m[3][2] = -far * near / (far - near);
    projection.m[2][3] = 1.0f;

    // translation matrix
    mat4x4 translate = {0};
    translate.m[0][0] = translate.m[1][1] = translate.m[2][2] = 1.0f;

    float angle = 0.0f; // radians

    myCamera camera = {0};

    float v = 0.1;

    while (!WindowShouldClose()) {
        // rotation angle for cube
        angle = fmodf(angle + GetFrameTime(), 8.0f * PI)*0;
        mat4x4 rotation_z = MakeRotationZ(angle);
        mat4x4 rotation_x = MakeRotationX(angle * 0.50f);
        mat4x4 rotation_y = MakeRotationY(angle * 0.25f);

        // camera movement
        if (IsKeyDown(KEY_W)) {
            camera.pos.z = camera.pos.z + v * cosf(camera.angle.phi);
            camera.pos.x = camera.pos.x + v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_S)) {
            camera.pos.z = camera.pos.z - v * cosf(camera.angle.phi);
            camera.pos.x = camera.pos.x - v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_A)) {
            camera.pos.x = camera.pos.x - v * cosf(camera.angle.phi);
            camera.pos.z = camera.pos.z + v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_D)) {
            camera.pos.x = camera.pos.x + v * cosf(camera.angle.phi);
            camera.pos.z = camera.pos.z - v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_Q)) {
            camera.pos.y = camera.pos.y - v;
        }
        if (IsKeyDown(KEY_E)) {
            camera.pos.y = camera.pos.y + v;
        }

        Vector2 mouseDel = GetMouseDelta();
        if ((mouseDel.x != 0 || mouseDel.y != 0) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
        {
            camera.angle.phi += mouseDel.x/100;
            // camera.angle.theta += mouseDel.y/100;
        }


        BeginDrawing();
        ClearBackground(BLACK);

        // loop through triangles
        for (size_t i = 0; i < vector_size(&cube.tris); i++) {

            triangle transformed = vector_get(&cube.tris, i); // note: not actually transformed yet
            triangle projected;
            bool drawable = true;

            // Model -> rotated model -> world/view space. Keep the mesh unchanged.

            // loop through points
            for (int j = 0; j < 3; j++) {
                vec3d point = transformed.p[j];
                // The unit cube's centre is (0.5, 0.5, 0.5).
                // So make it 0,0,0
                point.x -= 0.5f;
                point.y -= 0.5f;
                point.z -= 0.5f;

                // Rotate in X-Axis, Y-Axis, Z-Axis
                if ( 
                    !MultiplyMatrixVector(point, &point, &rotation_z) 
                    || !MultiplyMatrixVector(point, &point, &rotation_x)  
                    || !MultiplyMatrixVector(point, &point, &rotation_y) 
                    ) 
                {
                    drawable = false;
                    break;
                }

                // Offset into the screen
                point.z += 3.0f; 

                // translate position into camera space
                point.x -= camera.pos.x;
                point.y -= camera.pos.y;
                point.z -= camera.pos.z;

                // inverse rotation matrices with camera angles
                mat4x4 camera_rotation_x = MakeInverseRotationX(camera.angle.theta);
                mat4x4 camera_rotation_y = MakeInverseRotationY(camera.angle.phi);

                // apply inverse rotation matrices to point
                if ( !MultiplyMatrixVector(point, &point, &camera_rotation_x)
                  || !MultiplyMatrixVector(point, &point, &camera_rotation_y))
                {
                    drawable = false;
                    break;
                }

                // apply translated/transformed point to 
                transformed.p[j] = point; 
            }
            // check drawable before continuing
            if (!drawable) { continue; } 

            // calculate normal
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

            if (l == 0) {continue;printf("length == 0");}
            
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
                vec3d point = transformed.p[j];

                // project onto screen
                if (point.z < near || point.z > far
                    || !MultiplyMatrixVector(point, &projected.p[j], &projection)) { 
                    drawable = false;
                    break;
                }

                // at this stage the triangle is between x:[-1,1] and y:[-1,1]
                // so we need to scale it into view
                projected.p[j].x = (projected.p[j].x + 1.0f) * 0.5f * width;
                projected.p[j].y = (projected.p[j].y + 1.0f) * 0.5f * height;
            }

            // finally draw the projected triangle
            if (drawable) { 
                draw_triangle(&projected, WHITE);
            }
        }

        EndDrawing();
    }

    vector_free(&cube.tris);
    CloseWindow();
    return 0;
}
