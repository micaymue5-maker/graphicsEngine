#include "vectors.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void vector_init(TriVector *vec)
{
    vec->size = 0;
    vec->capacity = 12;
    vec->data = malloc(vec->capacity * sizeof *vec->data);
    if (vec->data == NULL) {
        perror("Failed to allocate triangle storage");
        exit(EXIT_FAILURE);
    }
}

void vector_push(TriVector *vec, triangle value)
{
    if (vec->size == vec->capacity) {
        const size_t max_capacity = SIZE_MAX / sizeof *vec->data;
        if (vec->capacity > max_capacity / 2) {
            fputs("Triangle storage is too large\n", stderr);
            exit(EXIT_FAILURE);
        }
        size_t capacity = vec->capacity == 0 ? 12 : vec->capacity * 2; //double capacity if full
        triangle *data = realloc(vec->data, capacity * sizeof *data);
        if (data == NULL) {
            perror("Failed to grow triangle storage");
            exit(EXIT_FAILURE);
        }
        vec->data = data;
        vec->capacity = capacity;
    }
    vec->data[vec->size++] = value;
}

triangle vector_get(const TriVector *vec, size_t index)
{
    if (index >= vec->size) {
        fputs("Triangle index out of bounds\n", stderr);
        exit(EXIT_FAILURE);
    }
    return vec->data[index];
}

void vector_free(TriVector *vec)
{
    free(vec->data);
    *vec = (TriVector){0};
}

size_t vector_size(const TriVector *vec)
{
    return vec->size;
}

bool MultiplyMatrixVector(vec3d input, vec3d *output, const mat4x4 *matrix)
{
    // Homogeneous input is (x, y, z, 1). For projection, w becomes depth.
    float w = input.x * matrix->m[0][3] + input.y * matrix->m[1][3]
            + input.z * matrix->m[2][3] + matrix->m[3][3];
    if (!isfinite(w) || fabsf(w) < 1.0e-6f) {
        return false;
    }

    vec3d result = {
        (input.x * matrix->m[0][0] + input.y * matrix->m[1][0]
         + input.z * matrix->m[2][0] + matrix->m[3][0]) / w,
        (input.x * matrix->m[0][1] + input.y * matrix->m[1][1]
         + input.z * matrix->m[2][1] + matrix->m[3][1]) / w,
        (input.x * matrix->m[0][2] + input.y * matrix->m[1][2]
         + input.z * matrix->m[2][2] + matrix->m[3][2]) / w
    };
    if (!isfinite(result.x) || !isfinite(result.y) || !isfinite(result.z)) {
        return false;
    }
    *output = result;
    return true;
}

mat4x4 MakeRotationX(float radians)
{
    float c = cosf(radians), s = sinf(radians);
    mat4x4 matrix = {0};
    matrix.m[0][0] = matrix.m[3][3] = 1.0f;
    matrix.m[1][1] = matrix.m[2][2] = c;
    matrix.m[1][2] = s;
    matrix.m[2][1] = -s;
    return matrix;
}

mat4x4 MakeRotationZ(float radians)
{
    float c = cosf(radians), s = sinf(radians);
    mat4x4 matrix = {0};
    matrix.m[2][2] = matrix.m[3][3] = 1.0f;
    matrix.m[0][0] = matrix.m[1][1] = c;
    matrix.m[0][1] = s;
    matrix.m[1][0] = -s;
    return matrix;
}

mat4x4 MakeRotationY(float radians)
{
    float c = cosf(radians), s = sinf(radians);
    mat4x4 matrix = {0};
    matrix.m[0][0] = matrix.m[2][2] = c;
    matrix.m[1][1] = matrix.m[3][3] = 1.0f;
    matrix.m[0][2] = -s;
    matrix.m[2][0] = s;
    return matrix;
}

mat4x4 MakeInverseRotationX(float radians)
{
    float c = cosf(radians), s = sinf(radians);
    mat4x4 matrix = {0};
    matrix.m[0][0] = matrix.m[3][3] = 1.0f;
    matrix.m[1][1] = matrix.m[2][2] = c;
    matrix.m[2][1] = s;
    matrix.m[1][2] = -s;
    return matrix;
}

mat4x4 MakeInverseRotationZ(float radians)
{
    float c = cosf(radians), s = sinf(radians);
    mat4x4 matrix = {0};
    matrix.m[2][2] = matrix.m[3][3] = 1.0f;
    matrix.m[0][0] = matrix.m[1][1] = c;
    matrix.m[1][0] = s;
    matrix.m[0][1] = -s;
    return matrix;
}

mat4x4 MakeInverseRotationY(float radians)
{
    float c = cosf(radians), s = sinf(radians);
    mat4x4 matrix = {0};
    matrix.m[0][0] = matrix.m[2][2] = c;
    matrix.m[1][1] = matrix.m[3][3] = 1.0f;
    matrix.m[2][0] = -s;
    matrix.m[0][2] = s;
    return matrix;
}
