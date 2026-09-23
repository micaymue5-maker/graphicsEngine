#ifndef VECTORS_H
#define VECTORS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    triangle *data;
    size_t size;
    size_t capacity;
} TriVector;

typedef struct {
    int vec[2];
} IntVec2;

typedef struct {
    int vec[3];
} IntVec3;

typedef struct {
    int vec[4];
} IntVec4;

typedef struct {
    double theta; // Y-axis rotation in degrees
    double phi;   // Z-axis rotation in degrees
    double psi;   // X-axis rotation in degrees
} DoubleAngle3;

typedef struct {
    float x, y, z;
} TriVec3;

typedef struct {
    TriVec3 p[3]; 
} triangle;

typedef struct {
    TriVector tris;
} Mesh;

typedef struct {
    float m[4][4];
} mat4x4;

typedef struct {
    float x, y, z;
} vec3d;

void vector_init(TriVector *vec);
void vector_push(TriVector *vec, triangle obj_tri);
triangle vector_get(TriVector *vec, size_t index);
void vector_free(TriVector *vec);
int vector_size(TriVector *vec);
void MultiplyMatrixVector(vec3d i, vec3d o, mat4x4 m);

#endif
