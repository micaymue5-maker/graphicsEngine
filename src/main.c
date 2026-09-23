#include <raylib.h>

#include <raster.h>
#include <vectors.h>
#include <shapes.h>
#include <math.h>

int main(void){

    const int width = 800;
    const int height = 600;

    InitWindow(width, height, "Pixel shapes - C + raylib");
    if (!IsWindowReady()) {
        return 1;
    }
    SetTargetFPS(20);

    int x = 0; // displacement
    int v = 0; // velecity



    // Rect rectangle = {
    //     .vec1 = {100, 100, 0},
    //     .vec2 = {300, 100, 0},
    //     .vec3 = {300, 200, 0},
    //     .vec4 = {100, 200, 0},
    // };

    // Tri triangle = {
    //     .vec1 = {400, 400, 0},
    //     .vec2 = {325, 410, 0},
    //     .vec3 = {600, 600, 0}
    // };
    // double centre[3] = {400, 300, 0};

    // DoubleAngle3 rot_up = {
    //     .phi = 10,
    //     .theta = 0
    // };
    // DoubleAngle3 rot_down = {
    //     .phi = -10,
    //     .theta = 0
    // };
    // DoubleAngle3 rot_left = {
    //     .phi = 0,
    //     .theta = 10
    // };
    // DoubleAngle3 rot_right = {
    //     .phi = 0,
    //     .theta = -10
    // };

    
    triangle cube[12] = {
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
    
    
    //multiplication matrix
    float fNear = 0.1;
    float fFar = 1000.0;
    float fFov = 90.0;
    float fAspectRatio = (float)width/(float)height;
    float fFovRad = 1.0 / tanf(fFov * 0.5 / 180.0 * PI);
    
    mat4x4 matProj;
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0;
    matProj.m[3][3] = 0.0;

    while (!WindowShouldClose()) {


        // Hold W/S: Y axis, A/D: Z axis, Q/E: X axis (degrees per frame).
        // if (IsKeyDown(KEY_W)){mycube.theta += 10;}
        // if (IsKeyDown(KEY_A)){mycube.phi += 10;}
        // if (IsKeyDown(KEY_S)){mycube.theta -= 10;}
        // if (IsKeyDown(KEY_D)){mycube.phi -= 10;}
        // if (IsKeyDown(KEY_Q)){mycube.psi += 10;}
        // if (IsKeyDown(KEY_E)){mycube.psi -= 10;}



        
        
        // Update input and any changing state here.
        BeginDrawing();
        ClearBackground(BLACK);
        
        for (size_t i = 0; i < 12; ++i) {
            triangle tri = cube[i]; // input triangle
            triangle triProjected; // output triangle

            MutiplyMatrixVector(tri.p[0], triProjected.p[0], matProj); // project vectors in triangle
            MutiplyMatrixVector(tri.p[1], triProjected.p[1], matProj);
            MutiplyMatrixVector(tri.p[2], triProjected.p[2], matProj);

            
        }



        EndDrawing();
        x+=v;
    }


    CloseWindow();
    return 0;
}
