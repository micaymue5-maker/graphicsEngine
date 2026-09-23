#ifndef SHAPES_H
#define SHAPES_H

#include <vectors.h>
#include <raylib.h>

typedef struct {
    double vec1[3];
    double vec2[3];
    double vec3[3];
    double vec4[3];
    double dtheta;
    double dphi;
    double dpsi;
    Color colour;
} Rect;

typedef struct {
    double vec1[3];
    double vec2[3];
    double vec3[3];
    double dtheta;
    double dphi;
    double dpsi;
    Color colour;
} Tri;

typedef struct {
    double vec1[3];
    double vec2[3];
    double vec3[3];
    double vec4[3];
    double vec5[3];
    double vec6[3];
    double vec7[3];
    double vec8[3];
    double centre[3];
    double theta;
    double phi;
    double psi;
    Color colour;
} Cube;

void draw_rect(Rect *rect);
void rotate_rect(Rect *rect, DoubleAngle3 *angles);
void rotate_vector(double vec[3], DoubleAngle3 *angles, double result[3]);
void draw_triangle(Tri *obj);
void rotate_triangle(Tri *obj, DoubleAngle3 *angles);
Cube get_cube_from_centre(double c[3], double w, double phi, double theta, double psi, Color colour);
void draw_cube(Cube *obj);

#endif
