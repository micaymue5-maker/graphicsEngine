#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vectors.h"
#include "logs.h"

/* Code Structure:
   ---------------

   > open file
   > create temp Vertex and Face to store values in
   > loop through line
        >check start of line for v or f
        >push values to temps

*/


void load_obj (const char *filename, Mesh* output)
{
    FILE *file = fopen(filename,"r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Temporary capacities for dynamic resizing
    int v_cap = 1000, f_cap = 1000;

    Vertex *vertices = malloc(v_cap * sizeof(Vertex));
    Face *faces = malloc(f_cap * sizeof(Face));
    int vertex_count = 0;
    int face_count = 0;
    float maxX, maxY, maxZ;
    float minX, minY, minZ;
    float shiftX, shiftY, shiftZ;
    float scale;

    if (vertices == NULL || faces == NULL)
    {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    char line[256];
    fgets(line, sizeof(line), file);
    if (strncmp(line, "v ",2) == 0)
    {
        Vertex v;
        sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
        (vertices)[vertex_count] = v;
        vertex_count++;

        maxX = minX = v.x;
        maxY = minY = v.y;
        maxZ = minZ = v.z;
    }
    while (fgets(line, sizeof(line), file))
    {
        // Parce vertices
        if (strncmp(line, "v ",2) == 0)
        {
            if (vertex_count >=  v_cap)
            {
                v_cap *= 2;
                vertices = realloc(vertices, v_cap * sizeof(Vertex));
                if (vertices == NULL)
                {
                    printf("Failed to reallocate memory");
                    exit(EXIT_FAILURE);
                }
            }
            Vertex v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            (vertices)[vertex_count] = v;
            vertex_count++;

            maxX = v.x>maxX ? v.x : maxX; 
            maxY = v.y>maxY ? v.y : maxY; 
            maxZ = v.z>maxZ ? v.z : maxZ; 
            
            minX = v.x<minX ? v.x : minX; 
            minY = v.y<minY ? v.y : minY; 
            minZ = v.z<minZ ? v.z : minZ; 
        }
        else if (strncmp(line, "f ",2) == 0)
        {
            if (face_count >= f_cap)
            {
                f_cap *= 2;
                faces = realloc(faces, f_cap * sizeof(Face));
                if (faces == NULL)
                {
                    printf("Failed to reallocate memory");
                    exit(EXIT_FAILURE);
                }
            }
            int f1; // face vertex indexes
            int f2; 
            int f3; 
            sscanf(line, "f %d %d %d", &f1, &f2, &f3);

            // Convert Wavefront 1-based indexing to C 0-based indexing
            f1--; f2--; f3--; 

            Face face;

            face.v1 = vertices[f1];
            face.v2 = vertices[f2];
            face.v3 = vertices[f3];
            face.normal = (Vertex){
                .x = (
                    (face.v2.y - face.v1.y) * (face.v3.z - face.v1.z) - 
                    (face.v2.z - face.v1.z) * (face.v3.y - face.v1.y)
                ),
                .y = (
                    (face.v2.z - face.v1.z) * (face.v3.x - face.v1.x) - 
                    (face.v2.x - face.v1.x) * (face.v3.z - face.v1.z)    
                ),
                .z = (
                    (face.v2.x - face.v1.x) * (face.v3.y - face.v1.y) - 
                    (face.v2.y - face.v1.y) * (face.v3.x - face.v1.x)    
                )
            };
            

            faces[face_count] = face; // (*faces) references the object and not the address. 
            face_count++;
        }
    }
    if (vertex_count == 0)
    {
        log_message("ERROR","Failed to load verteices in object. script: parce_obj.c");
        return;
    }
    if (face_count == 0)
    {
        log_message("ERROR","Failed to load face in object. script: parce_obj.c");
        return;
    }

    shiftX = (maxX+minX)/2;
    shiftY = (maxY+minY)/2;
    shiftZ = (maxZ+minZ)/2;
    
    scale = (maxY-minY>scale) ? maxY-minY : scale;
    scale = (maxZ-minZ>scale) ? maxZ-minZ : scale;

    scale = (scale==0) ? 1 : scale;


    for (int i = 0; i < face_count; i++)
    {
        vector_push(
            output, 
            (Face){
                .v1 = {
                    .x = (faces[i].v1.x - shiftX)/scale,
                    .y = (faces[i].v1.y - shiftY)/scale,
                    .z = (faces[i].v1.z - shiftZ)/scale,
                },
                .v2 = {
                    .x = (faces[i].v2.x - shiftX)/scale,
                    .y = (faces[i].v2.y - shiftY)/scale,
                    .z = (faces[i].v2.z - shiftZ)/scale,
                },
                .v3 = {
                    .x = (faces[i].v3.x - shiftX)/scale,
                    .y = (faces[i].v3.y - shiftY)/scale,
                    .z = (faces[i].v3.z - shiftZ)/scale,
                },
                .normal = faces[i].normal
            }
        );
    }

    free(faces);
    free(vertices);

    fclose(file);
}
