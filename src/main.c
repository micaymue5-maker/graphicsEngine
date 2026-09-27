#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "shapes.h"
#include "vectors.h"
#include "camera.h"

int main(void)
{
    const int width = 800, height = 600; // screen
    InitWindow(width, height, "Triangle mesh - C + raylib");
    if (!IsWindowReady()) { return 1; }
    SetTargetFPS(60);

    
    // screen projection matrix
    mat4x4 projection = {0};
    make_projection_matrix(&projection);
    
    // translation matrix
    mat4x4 translate = {0};
    translate.m[0][0] = translate.m[1][1] = translate.m[2][2] = 1.0f;
    
    // objects
    mesh cube = get_cube(); //cube made up of series of triangles
    Camera3d camera = {
        .position = {0.0f, 0.0f, 0.0f},
        .target = {0.0f, 0.0f, 1.0f},
        .fovy = 90.0f,
        .up = {0.0f, 1.0f, 0.0f}
    };
    
    // floats
    float angle = 0.0f; // radians
    float v = 0.1;
    
    while (!WindowShouldClose()) {

        // rotation angle for cube
        angle = fmodf(angle + GetFrameTime(), 8.0f * PI)*0;

        // cube rotation matrices
        mat4x4 rotation_z = MakeRotationZ(angle);
        mat4x4 rotation_x = MakeRotationX(angle * 0.50f);
        mat4x4 rotation_y = MakeRotationY(angle * 0.25f);

        // camera movement
        if (IsKeyDown(KEY_W)) 
        {
            camera.position.z += v * cosf(camera.angle.phi);
            camera.position.x += v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_S)) 
        {
            camera.position.z -= v * cosf(camera.angle.phi);
            camera.position.x -= v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_A)) 
        {
            camera.position.x -= v * cosf(camera.angle.phi);
            camera.position.z += v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_D)) 
        {
            camera.position.x += v * cosf(camera.angle.phi);
            camera.position.z -= v * sinf(camera.angle.phi);
        }
        if (IsKeyDown(KEY_Q)) 
        {
            camera.position.y -= v;
        }
        if (IsKeyDown(KEY_E)) 
        {
            camera.position.y += v;
        }

        // Handle mouse movement
        Vector2 mouseDel = GetMouseDelta();
        if ((mouseDel.x != 0 || mouseDel.y != 0) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
        {
            camera.angle.phi += mouseDel.x/100;
            camera.angle.theta -= mouseDel.y/100;
        }


        BeginDrawing();
        ClearBackground(BLACK);

        // loop through triangles
        
        temp(&cube,camera);
        EndDrawing();
    }

    vector_free(&cube.tris);
    CloseWindow();
    return 0;
}
