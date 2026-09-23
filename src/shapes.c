#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vectors.h"
#include "shapes.h"
#include "raster.h"

void draw_rect(Rect *rect);
void rotate_rect(Rect *rect, DoubleAngle3 *angles);
void rotate_vector(double vec[3], DoubleAngle3 *angles, double result[3]);
void draw_triangle(Tri *obj);
void rotate_triangle(Tri *obj, DoubleAngle3 *angles);
Cube get_cube_from_centre(double c[3], double w, double phi, double theta, double psi, Color colour);
void draw_cube(Cube *obj);


void draw_rect(Rect *rect)
{
    double *vec1 = rect->vec1;
    double *vec2 = rect->vec2;
    double *vec3 = rect->vec3;
    double *vec4 = rect->vec4;
    drawLine((int)round(vec1[0]), (int)round(vec2[0]), (int)round(vec1[1]), (int)round(vec2[1]), WHITE, 1);
    drawLine((int)round(vec2[0]), (int)round(vec3[0]), (int)round(vec2[1]), (int)round(vec3[1]), WHITE, 1);
    drawLine((int)round(vec3[0]), (int)round(vec4[0]), (int)round(vec3[1]), (int)round(vec4[1]), WHITE, 1);
    drawLine((int)round(vec4[0]), (int)round(vec1[0]), (int)round(vec4[1]), (int)round(vec1[1]), WHITE, 1);
}

void rotate_rect(Rect *rect, DoubleAngle3 *angles)
{
    double *vec1 = rect->vec1;
    double *vec2 = rect->vec2;
    double *vec3 = rect->vec3;
    double *vec4 = rect->vec4;

    // Centre points
    double c[3] = {
        (vec1[0] + vec2[0] + vec3[0] + vec4[0]) / 4.0,
        (vec1[1] + vec2[1] + vec3[1] + vec4[1]) / 4.0,
        (vec1[2] + vec2[2] + vec3[2] + vec4[2]) / 4.0
    };

    // centre rectangle about (0,0,0)
    vec1[0] -= c[0];    vec1[1] -= c[1];    vec1[2] -= c[2];
    vec2[0] -= c[0];    vec2[1] -= c[1];    vec2[2] -= c[2];
    vec3[0] -= c[0];    vec3[1] -= c[1];    vec3[2] -= c[2];
    vec4[0] -= c[0];    vec4[1] -= c[1];    vec4[2] -= c[2];

    rotate_vector(vec1, angles, vec1);
    rotate_vector(vec2, angles, vec2);
    rotate_vector(vec3, angles, vec3);
    rotate_vector(vec4, angles, vec4);

    for (int i = 0; i < 3; i++) {
        vec1[i] += c[i];
        vec2[i] += c[i];
        vec3[i] += c[i];
        vec4[i] += c[i];
    }
}

void draw_triangle(Tri *obj) 
{
    double *vec1 = obj->vec1;
    double *vec2 = obj->vec2;
    double *vec3 = obj->vec3;

    drawLine((int)round(vec1[0]), (int)round(vec2[0]), (int)round(vec1[1]), (int)round(vec2[1]), WHITE, 1);
    drawLine((int)round(vec2[0]), (int)round(vec3[0]), (int)round(vec2[1]), (int)round(vec3[1]), WHITE, 1);
    drawLine((int)round(vec3[0]), (int)round(vec1[0]), (int)round(vec3[1]), (int)round(vec1[1]), WHITE, 1);
}

void rotate_triangle(Tri *obj, DoubleAngle3 *angles)
{
    double *vec1 = obj->vec1;
    double *vec2 = obj->vec2;
    double *vec3 = obj->vec3;

    double c[3] = {
        (vec1[0] + vec2[0] + vec3[0]) / 3,
        (vec1[1] + vec2[1] + vec3[1]) / 3,
        (vec1[2] + vec2[2] + vec3[2]) / 3,
    };

    // centre triangle about (0,0,0)
    vec1[0] -= c[0];    vec1[1] -= c[1];    vec1[2] -= c[2];
    vec2[0] -= c[0];    vec2[1] -= c[1];    vec2[2] -= c[2];
    vec3[0] -= c[0];    vec3[1] -= c[1];    vec3[2] -= c[2];

    rotate_vector(vec1, angles, vec1);
    rotate_vector(vec2, angles, vec2);
    rotate_vector(vec3, angles, vec3);

    // move triangle back
    for (int i = 0; i < 3; i++){
    vec1[i] += c[i];
    vec2[i] += c[i];
    vec3[i] += c[i];
    }
}

void draw_cube(Cube *obj)
{
    // ideal case has trivial shape
    if (obj->phi == 0 && obj->theta == 0 && obj->psi == 0) {
        drawLine(obj->vec1[0],obj->vec5[0],obj->vec1[1],obj->vec5[1], obj->colour, 1);
        drawLine(obj->vec5[0],obj->vec7[0],obj->vec5[1],obj->vec7[1], obj->colour, 1);
        drawLine(obj->vec7[0],obj->vec3[0],obj->vec7[1],obj->vec3[1], obj->colour, 1);
        drawLine(obj->vec3[0],obj->vec1[0],obj->vec3[1],obj->vec1[1], obj->colour, 1);
    }

    // else have to rotate coordinates
    else {
        // temp coordinates to work with
        double tempv1[3];
        double tempv2[3];
        double tempv3[3];
        double tempv4[3];
        double tempv5[3];
        double tempv6[3];
        double tempv7[3];
        double tempv8[3];

        // define angle object for rotation function
        DoubleAngle3 angle = {
            .phi = obj->phi,
            .theta = obj->theta,
            .psi = obj->psi
        };

        for (int i = 0; i < 3; i++) {
            tempv1[i] = obj->vec1[i] - obj->centre[i];
            tempv2[i] = obj->vec2[i] - obj->centre[i];
            tempv3[i] = obj->vec3[i] - obj->centre[i];
            tempv4[i] = obj->vec4[i] - obj->centre[i];
            tempv5[i] = obj->vec5[i] - obj->centre[i];
            tempv6[i] = obj->vec6[i] - obj->centre[i];
            tempv7[i] = obj->vec7[i] - obj->centre[i];
            tempv8[i] = obj->vec8[i] - obj->centre[i];
        }

        // rotate temp vectors
        rotate_vector(tempv1, &angle, tempv1);
        rotate_vector(tempv2, &angle, tempv2);
        rotate_vector(tempv3, &angle, tempv3);
        rotate_vector(tempv4, &angle, tempv4);
        rotate_vector(tempv5, &angle, tempv5);
        rotate_vector(tempv6, &angle, tempv6);
        rotate_vector(tempv7, &angle, tempv7);
        rotate_vector(tempv8, &angle, tempv8);

        for (int i = 0; i < 3; i++) {
            tempv1[i] += obj->centre[i];
            tempv2[i] += obj->centre[i];
            tempv3[i] += obj->centre[i];
            tempv4[i] += obj->centre[i];
            tempv5[i] += obj->centre[i];
            tempv6[i] += obj->centre[i];
            tempv7[i] += obj->centre[i];
            tempv8[i] += obj->centre[i];
        }

        // draw lines from temp vectors
        drawLine((int)round(tempv1[0]),(int)round(tempv2[0]),(int)round(tempv1[1]),(int)round(tempv2[1]),obj->colour,1);
        drawLine((int)round(tempv1[0]),(int)round(tempv5[0]),(int)round(tempv1[1]),(int)round(tempv5[1]),obj->colour,1);
        drawLine((int)round(tempv1[0]),(int)round(tempv3[0]),(int)round(tempv1[1]),(int)round(tempv3[1]),obj->colour,1);
        drawLine((int)round(tempv8[0]),(int)round(tempv7[0]),(int)round(tempv8[1]),(int)round(tempv7[1]),obj->colour,1);
        drawLine((int)round(tempv8[0]),(int)round(tempv4[0]),(int)round(tempv8[1]),(int)round(tempv4[1]),obj->colour,1);
        drawLine((int)round(tempv8[0]),(int)round(tempv6[0]),(int)round(tempv8[1]),(int)round(tempv6[1]),obj->colour,1);
        drawLine((int)round(tempv6[0]),(int)round(tempv5[0]),(int)round(tempv6[1]),(int)round(tempv5[1]),obj->colour,1);
        drawLine((int)round(tempv6[0]),(int)round(tempv2[0]),(int)round(tempv6[1]),(int)round(tempv2[1]),obj->colour,1);
        drawLine((int)round(tempv4[0]),(int)round(tempv3[0]),(int)round(tempv4[1]),(int)round(tempv3[1]),obj->colour,1);
        drawLine((int)round(tempv4[0]),(int)round(tempv2[0]),(int)round(tempv4[1]),(int)round(tempv2[1]),obj->colour,1);
        drawLine((int)round(tempv7[0]),(int)round(tempv5[0]),(int)round(tempv7[1]),(int)round(tempv5[1]),obj->colour,1);
        drawLine((int)round(tempv7[0]),(int)round(tempv3[0]),(int)round(tempv7[1]),(int)round(tempv3[1]),obj->colour,1);
    }
}

Cube get_cube_from_centre(double c[3], double w, double phi, double theta, double psi, Color colour)
{
    Cube cube = {
        .vec1 = { c[0]+w/2, c[1]+w/2, c[2]+w/2 },
        .vec2 = { c[0]+w/2, c[1]+w/2, c[2]-w/2 },
        .vec3 = { c[0]+w/2, c[1]-w/2, c[2]+w/2 },
        .vec4 = { c[0]+w/2, c[1]-w/2, c[2]-w/2 },
        .vec5 = { c[0]-w/2, c[1]+w/2, c[2]+w/2 },
        .vec6 = { c[0]-w/2, c[1]+w/2, c[2]-w/2 },
        .vec7 = { c[0]-w/2, c[1]-w/2, c[2]+w/2 },
        .vec8 = { c[0]-w/2, c[1]-w/2, c[2]-w/2 },
        .centre = {c[0], c[1], c[2]},
        .phi = phi,
        .theta = theta,
        .psi = psi,
        .colour = colour
    };
    return cube;
}

void rotate_vector(double vec[3], DoubleAngle3 *angles, double result[3]) 
{
    /*
    Angles are in degrees. Apply X (psi), then Y (theta), then Z (phi).
    For column vectors, the combined matrix is Rz(phi) * Ry(theta) * Rx(psi).
    Screen projection uses x and y; input and output may share storage.
    */
    double theta_rad = angles -> theta * (PI / 180.0);
    double phi_rad = angles -> phi * (PI / 180.0);
    double psi_rad = angles -> psi * (PI / 180.0);

    // precompute sine and cosine values
    double cos_p = cos(phi_rad);
    double sin_p = sin(phi_rad);
    double cos_t = cos(theta_rad);
    double sin_t = sin(theta_rad);
    double cos_s = cos(psi_rad);
    double sin_s = sin(psi_rad);

    // Combined rotation about all three coordinate axes.
    double R[3][3] = {
        { cos_p*cos_t, cos_p*sin_t*sin_s - sin_p*cos_s, cos_p*sin_t*cos_s + sin_p*sin_s },
        { sin_p*cos_t, sin_p*sin_t*sin_s + cos_p*cos_s, sin_p*sin_t*cos_s - cos_p*sin_s },
        { -sin_t,     cos_t*sin_s,                     cos_t*cos_s }
    };

    // define initial vector
    double v[3] = {vec[0], vec[1], vec[2]};

    // perform the multiplication
    for (int i = 0; i < 3; i++) {
        result[i] = 0.0;
        for (int j = 0; j < 3; j++) {
            result[i] += R[i][j] * v[j];
        }
    }
}


