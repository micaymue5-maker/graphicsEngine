#ifndef VECTORS_H
#define VECTORS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float x, y, z;
} vec3d;

typedef struct {
    vec3d p[3];
} triangle;

// A growable collection of triangles, not a mathematical vector.
// Owns data: initialise once, then free once; do not shallow-copy its ownership.
typedef struct {
    triangle *data;
    size_t size;
    size_t capacity;
} TriVector;

typedef struct {
    float theta;
    float phi;
} PolarAngle;

typedef struct {
    float m[4][4];
} mat4x4;

typedef struct {
    vec3d pos;
    PolarAngle angle;
} myCamera;

void vector_init(TriVector *vec);
void vector_push(TriVector *vec, triangle value);
triangle vector_get(const TriVector *vec, size_t index);
void vector_free(TriVector *vec);
size_t vector_size(const TriVector *vec);

// Tutorial matrix layout: translation is in m[3][0..2].
// Writes through output; returns false if the homogeneous divide is invalid.
// Input is copied, so MultiplyMatrixVector(v, &v, &matrix) is safe.
bool MultiplyMatrixVector(vec3d input, vec3d *output, const mat4x4 *matrix);

// Angles are radians. All unspecified matrix entries are zero-initialised.
mat4x4 MakeRotationX(float radians);
mat4x4 MakeRotationZ(float radians);
mat4x4 MakeRotationY(float radians);
mat4x4 MakeInverseRotationX(float radians);
mat4x4 MakeInverseRotationZ(float radians);
mat4x4 MakeInverseRotationY(float radians);
#endif
