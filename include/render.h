#ifndef RENDER_H
#define RENDER_H

#include "shapes.h"
#include "vectors.h"

void raster_object(Mesh *object, myCamera *camera, rotation_mats *rotation, mat4x4 *projection);

#endif

