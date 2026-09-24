#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raster.h"
#include "shapes.h"
#include "vectors.h"

// These stubs capture the real pixel rasteriser without opening a window.
static unsigned char pixels[600][800];
static unsigned long pixel_calls;
static int frame;
static int window_closed;
static int in_frame;
static unsigned long first_frame_hash;
static int changed_frame;
int demo_main(void);

void DrawPixel(int x, int y, Color colour)
{
    assert(x >= 0 && x < 800 && y >= 0 && y < 600);
    assert(colour.a == 255);
    pixels[y][x] = 1;
    pixel_calls++;
}

void InitWindow(int width, int height, const char *title)
{
    assert(width == 800 && height == 600 && title != NULL);
    frame = window_closed = in_frame = changed_frame = 0;
}
bool IsWindowReady(void) { return true; }
void SetTargetFPS(int fps) { assert(fps > 0); }
bool WindowShouldClose(void) { return frame >= 800; }
float GetFrameTime(void) { return 1.0f / 60.0f; }
void BeginDrawing(void) { assert(!in_frame); in_frame = 1; }
void ClearBackground(Color colour)
{
    (void)colour;
    assert(in_frame);
    memset(pixels, 0, sizeof pixels);
    pixel_calls = 0;
}
void EndDrawing(void)
{
    assert(in_frame && pixel_calls > 100);
    unsigned long hash = 5381;
    for (int y = 0; y < 600; y++) {
        for (int x = 0; x < 800; x++) {
            if (pixels[y][x]) { hash = hash * 33u + (unsigned long)(y * 800 + x); }
        }
    }
    if (frame == 0) { first_frame_hash = hash; }
    else if (hash != first_frame_hash) { changed_frame = 1; }
    frame++;
    in_frame = 0;
}
void CloseWindow(void) { window_closed = 1; }

static void near(float actual, float expected)
{
    assert(fabsf(actual - expected) < 1.0e-4f);
}

static void test_storage(void)
{
    TriVector list;
    vector_init(&list);
    for (int i = 0; i < 100; i++) {
        triangle value = {.p = {{(float)i, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
        vector_push(&list, value);
    }
    assert(vector_size(&list) == 100 && list.capacity >= 100);
    for (size_t i = 0; i < vector_size(&list); i++) {
        triangle value = vector_get(&list, i);
        near(value.p[0].x, (float)i);
        near(value.p[2].z, 9);
    }
    vector_free(&list);
    assert(list.data == NULL && list.size == 0 && list.capacity == 0);
    vector_push(&list, (triangle){.p = {{42, 0, 0}}});
    near(vector_get(&list, 0).p[0].x, 42);
    vector_free(&list);

    mesh cube = get_cube();
    assert(vector_size(&cube.tris) == 12);
    for (size_t i = 0; i < cube.tris.size; i++) {
        triangle t = cube.tris.data[i];
        vec3d a = {t.p[1].x-t.p[0].x, t.p[1].y-t.p[0].y, t.p[1].z-t.p[0].z};
        vec3d b = {t.p[2].x-t.p[0].x, t.p[2].y-t.p[0].y, t.p[2].z-t.p[0].z};
        vec3d n = {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
        near(n.x*n.x+n.y*n.y+n.z*n.z, 1);
        // Outward winding is needed for the later back-face culling stage.
        assert(n.x*(t.p[0].x-0.5f)+n.y*(t.p[0].y-0.5f)+n.z*(t.p[0].z-0.5f) > 0);
    }
    vector_free(&cube.tris);
}

static void test_matrices(void)
{
    mat4x4 identity = MakeRotationX(0);
    vec3d v = {2, 3, 4}, out;
    assert(MultiplyMatrixVector(v, &out, &identity));
    near(out.x, 2); near(out.y, 3); near(out.z, 4);
    mat4x4 x = MakeRotationX(PI / 2);
    assert(MultiplyMatrixVector((vec3d){0,1,0}, &out, &x));
    near(out.x, 0); near(out.y, 0); near(out.z, 1);
    mat4x4 z = MakeRotationZ(PI / 2);
    assert(MultiplyMatrixVector((vec3d){1,0,0}, &out, &z));
    near(out.x, 0); near(out.y, 1); near(out.z, 0);
    assert(MultiplyMatrixVector(v, &v, &z)); // in-place output
    near(v.x, -3); near(v.y, 2); near(v.z, 4);
    for (int i = 0; i < 100; i++) {
        x = MakeRotationX((float)i * 0.13f);
        z = MakeRotationZ((float)i * -0.17f);
        v = (vec3d){2,3,4};
        assert(MultiplyMatrixVector(v, &v, &z));
        assert(MultiplyMatrixVector(v, &v, &x));
        near(v.x*v.x + v.y*v.y + v.z*v.z, 29);
    }
    mat4x4 projection = {0};
    projection.m[0][0] = 0.75f;
    projection.m[1][1] = 1;
    projection.m[2][2] = 1000.0f / 999.9f;
    projection.m[3][2] = -100.0f / 999.9f;
    projection.m[2][3] = 1;
    assert(MultiplyMatrixVector((vec3d){1,1,2}, &out, &projection));
    near(out.x, 0.375f); near(out.y, 0.5f);
    assert(MultiplyMatrixVector((vec3d){0,0,0.1f}, &out, &projection));
    near(out.z, 0);
    assert(MultiplyMatrixVector((vec3d){0,0,1000}, &out, &projection));
    near(out.z, 1);
    out = (vec3d){7,8,9};
    assert(!MultiplyMatrixVector((vec3d){0,0,0}, &out, &projection));
    near(out.x, 7); near(out.y, 8); near(out.z, 9);
}

static void test_lines(void)
{
    // Every octant, horizontal/vertical, reversed endpoints, and a single pixel.
    for (int dx = -5; dx <= 5; dx++) {
        for (int dy = -5; dy <= 5; dy++) {
            memset(pixels, 0, sizeof pixels);
            pixel_calls = 0;
            raster_draw_line(10, 10, 10+dx, 10+dy, WHITE);
            assert(pixels[10][10] && pixels[10+dy][10+dx]);
            int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
            assert(pixel_calls == (unsigned long)(steps+1));
        }
    }
    triangle t = {.p = {{20,20,0}, {30,20,0}, {20,30,0}}};
    memset(pixels, 0, sizeof pixels);
    draw_triangle(&t, WHITE);
    assert(pixels[20][25] && pixels[25][20] && pixels[25][25]);
}

int main(void)
{
    test_storage();
    test_matrices();
    test_lines();
    assert(demo_main() == 0);
    assert(window_closed && frame == 800 && changed_frame);
    puts("PASS: triangle storage, cube winding, transforms/projection, rasterisation, 800 rendered frames");
    return 0;
}
