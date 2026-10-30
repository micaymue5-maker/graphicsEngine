#ifndef VECTORS_H
#define VECTORS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    Vertex normal;
} Face;

// A growable collection of triangles, not a mathematical vector.
// Owns data: initialise once, then free once; do not shallow-copy its ownership.
typedef struct {
    Face *data;
    size_t size;
    size_t capacity;
} Mesh;

typedef struct {
    float theta;
    float phi;
} PolarAngle;

typedef struct {
    float m[4][4];
} mat4x4;

typedef struct {
    mat4x4 x;
    mat4x4 y;
    mat4x4 z;
} rotation_mats;

typedef struct {
    Vertex pos;
    PolarAngle angle;
} myCamera;

void vector_init(Mesh *vec);
void vector_push(Mesh *vec, Face value);
Face vector_get(const Mesh *vec, size_t index);
void vector_free(Mesh *vec);
size_t vector_size(const Mesh *vec);

// Tutorial matrix layout: translation is in m[3][0..2].
// Writes through output; returns false if the homogeneous divide is invalid.
// Input is copied, so MultiplyMatrixVector(v, &v, &matrix) is safe.
bool MultiplyMatrixVector(Vertex input, Vertex *output, const mat4x4 *matrix);

// Angles are radians. All unspecified matrix entries are zero-initialised.
mat4x4 MakeRotationX(float radians);
mat4x4 MakeRotationZ(float radians);
mat4x4 MakeRotationY(float radians);
mat4x4 MakeInverseRotationX(float radians);
mat4x4 MakeInverseRotationZ(float radians);
mat4x4 MakeInverseRotationY(float radians);
#endif
