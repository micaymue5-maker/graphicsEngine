#include <math.h>
#include "raylib_compat.h"
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "shapes.h"
#include "vectors.h"
#include "render.h"
#include "parse_obj.h"
#include "logs.h"

void camera_movement(myCamera *camera)
{
    float v = 0.1;
    if (IsKeyDown(KEY_W)) 
    {
        camera->pos.z = camera->pos.z + v * cosf(camera->angle.phi);
        camera->pos.x = camera->pos.x + v * sinf(camera->angle.phi);
    }
    if (IsKeyDown(KEY_S)) 
    {
        camera->pos.z = camera->pos.z - v * cosf(camera->angle.phi);
        camera->pos.x = camera->pos.x - v * sinf(camera->angle.phi);
    }
    if (IsKeyDown(KEY_A)) 
    {
        camera->pos.x = camera->pos.x - v * cosf(camera->angle.phi);
        camera->pos.z = camera->pos.z + v * sinf(camera->angle.phi);
    }
    if (IsKeyDown(KEY_D)) 
    {
        camera->pos.x = camera->pos.x + v * cosf(camera->angle.phi);
        camera->pos.z = camera->pos.z - v * sinf(camera->angle.phi);
    }
    if (IsKeyDown(KEY_Q)) 
    {
        camera->pos.y = camera->pos.y - v;
    }
    if (IsKeyDown(KEY_E)) 
    {
        camera->pos.y = camera->pos.y + v;
    }
    
    // Handle mouse movement
    Vector2 mouseDel = GetMouseDelta();
    if ((mouseDel.x != 0 || mouseDel.y != 0) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
    {
        camera->angle.theta -= mouseDel.y/100;
        camera->angle.phi += mouseDel.x/100;
    }

}

#define MAX_MS_TRACKED 250 // Tracks frames from 0ms up to 250ms (4 FPS)

typedef struct {
    uint64_t buckets[MAX_MS_TRACKED + 1];
    uint64_t total_frames;
    double total_time_ms;
} FixedTracker;

void tracker_push(FixedTracker *t, double frame_time_seconds) {
    double ms = frame_time_seconds * 1000.0;
    t->total_time_ms += ms;
    t->total_frames++;

    // Cast to integer to find the right millisecond bucket
    int bucket_idx = (int)ms;
    if (bucket_idx < 0) bucket_idx = 0;
    if (bucket_idx > MAX_MS_TRACKED) bucket_idx = MAX_MS_TRACKED; // Cap extremes

    t->buckets[bucket_idx]++;
}

void tracker_report(FixedTracker *t) {
    if (t->total_frames == 0) return;

    printf("\n=== FIXED MEMORY PERFORMANCE REPORT ===\n");
    printf("Total Frames: %llu\n", t->total_frames);
    printf("Average: %.2f ms (%.1f FPS)\n", t->total_time_ms / t->total_frames, 1000.0 / (t->total_time_ms / t->total_frames));

    // Calculate exact percentile targets
    uint64_t target_1pct = t->total_frames / 100;
    if (target_1pct == 0) target_1pct = 1;

    // To find WORST 1%, we loop backwards from the slowest bucket (MAX_MS_TRACKED)
    uint64_t frames_found = 0;
    double worst_1pct_sum = 0;

    for (int i = MAX_MS_TRACKED; i >= 0; i--) {
        uint64_t count = t->buckets[i];
        if (count == 0) continue;

        if (frames_found + count >= target_1pct) {
            uint64_t needed = target_1pct - frames_found;
            worst_1pct_sum += needed * i;
            break;
        } else {
            worst_1pct_sum += count * i;
            frames_found += count;
        }
    }

    printf("Worst 1%% Average: %.2f ms\n", worst_1pct_sum / target_1pct);
}

int main(void)
{
    const int width = 800, height = 600; // screen
    InitWindow(width, height, "Triangle mesh - C + raylib");
    if (!IsWindowReady()) { return 1; }
    SetTargetFPS(60);
    
    const float near = 0.1f, far = 1000.0f;
    const float fov_degrees = 90.0f;
    const float aspect = (float)height / (float)width;
    const float focal = 1.0f / tanf(fov_degrees * 0.5f * DEG2RAD);

    FixedTracker frame_tracker;
    
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
    
    // objects
    Mesh cube = get_cube(); //cube made up of series of triangless
    Mesh cube2 = get_cube_at_pos(2,3,0,2);
    Mesh lil_guy;
    myCamera camera = {0};
    
    // my lil fella
    vector_init(&lil_guy);
    const char *lil_guy_path = "imports/humanoid_tri.obj";
    load_obj(lil_guy_path, &lil_guy);
    float pi = 3.14159265f;
    rotate_mesh(&lil_guy, pi,0.0f,0.0f);



    // floats
    float angle = 0.0f; // radians
    // float v = 0.1;

    while (!WindowShouldClose()) {
        clock_t start = clock();

        camera_movement(&camera);
        // rotation angle for cube
        angle = fmodf(angle + GetFrameTime(), 8.0f * PI)*0;

        // cube rotation matrices
        
        rotation_mats cube_rotation = {
            .x = MakeRotationX(angle * 0.50f),
            .z = MakeRotationZ(angle),
            .y = MakeRotationY(angle * 0.25f)
        };

        // camera movement


        BeginDrawing();
        ClearBackground(BLACK);

        // loop through triangles
        raster_object(&cube, &camera, &cube_rotation, &projection);
        raster_object(&cube2, &camera, &cube_rotation, &projection);
        raster_object(&lil_guy, &camera, &cube_rotation, &projection);
        EndDrawing();

        clock_t end = clock();
        double frame_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Frame time: %fs   (%dfps)\n",frame_time,(int)(1/frame_time));
        tracker_push(&frame_tracker, frame_time);

    }

    vector_free(&cube);
    vector_free(&cube2);
    vector_free(&lil_guy);
    CloseWindow();

    tracker_report(&frame_tracker);
    print_log_file();
    return 0;
}
