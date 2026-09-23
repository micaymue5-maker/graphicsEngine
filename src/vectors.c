#include "stdio.h"
#include "stdlib.h"
#include "vectors.h"


// initialise the vector
void vector_init(TriVector *vec) {
    vec -> size = 0;
    vec -> capacity = 12; //start with number of triangles in 3d cube
    vec -> data = malloc(vec -> capacity * sizeof(triangle));

    if (vec -> data == NULL) {
        perror("Failed to allocate memory to vector");
        exit(1);
    }
}

// Push new element to end of vector
void vector_push(TriVector *vec, triangle obj_tri) { 
    // If the vector is full, double capacity
    if (vec -> size >= vec -> capacity) {
        vec -> capacity *= 2;
        int *new_data = realloc(vec -> data, vec -> capacity * sizeof(int));

        if (new_data == NULL) {
            perror("Failed to reallocate memory for vector");
            exit(1);
        }

        vec -> data = new_data;
    }

    // Insert the element and increment size
    vec -> data[vec -> size] = obj_tri;
    vec -> size++;
}

// Get an item at a specific index
triangle vector_get(TriVector *vec, size_t index) {
    if (index >= vec -> size) {
        printf("Index out of bounds!/n");
        exit(1);
    }
    return vec -> data[index];
}

// Free allocated memory
void vector_free(TriVector *vec) {
    free(vec -> data);
    vec -> data = NULL;
    vec -> size = 0;
    vec -> capacity = 0;
}

int vector_size(TriVector *vec) {
    return vec -> size;
}

void MultiplyMatrixVector(vec3d i, vec3d o, mat4x4 m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    // fourth element in vector needed for 4x4 matmul
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0){
         o.x /= w; o.y /= w; o.z /= w;
    }
};