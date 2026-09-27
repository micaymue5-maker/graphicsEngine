#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include "vectors.h"

typedef struct {
vec3d position;
vec3d target; // Camera target it looks-at
vec3d up;     // Camera up vector (in its frame of reference)
float fovy;   // Camera fov apature in Y (degrees)
} Camera3d;

const float width = 800;
const float height = 600;
const float near = 0.1f, far = 1000.0f;
const float fov_degrees = 90.0f;
const float aspect = (float)height / (float)width;

void make_projection_matrix(mat4x4* projection);
void temp(mesh *myMesh, Camera3d camera);
#endif
