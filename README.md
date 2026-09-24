# Graphics Engine — Triangle Wireframe in C

A C17 learning project following javidx9's 3D Graphics Engine series. The current
stage draws an automatically rotating, perspective-projected cube made from
12 triangles. Raylib supplies the window and `DrawPixel`; the line rasterisation,
transformations and projection are implemented in this project.

## Build and run

Requires a C17 compiler, Make, raylib and pkg-config. On the configured Apple
Silicon Mac, raylib and pkg-config are installed through Homebrew.

```sh
make
make run
```

Run `make test` for headless checks of triangle storage, matrix transforms,
projection, cube winding, line rasterisation and 800 frames of the actual demo
loop. The tests capture `DrawPixel` calls without opening a raylib window.

Run these commands in the project folder. The executable is
`build/pixel_shapes`. Press Escape or close the window to quit. Rotation is
automatic and uses elapsed time; there are no Q/E/W/S/A/D controls.

In VS Code, open this folder, use Cmd+Shift+B to build, or choose
Terminal > Run Task > Run C project. Build the whole project rather than just the
active source file. Generated files under `build/` are ignored by Git.

## Types and ownership

- `vec3d`: a point/vector with floating-point `x`, `y`, `z` components.
- `triangle`: exactly three `vec3d` vertices in `p[3]`.
- `mat4x4`: a 4-by-4 transformation matrix, following the tutorial's layout.
- `TriVector`: a growable triangle array (`data`, `size`, `capacity`), the C
  equivalent of the tutorial's `std::vector<triangle>`.
- `mesh`: owns a `TriVector tris`. The lowercase name avoids raylib's `Mesh` type.

`get_cube()` creates a mesh using the original SOUTH, EAST, NORTH, WEST, TOP and
BOTTOM face ordering. It copies the fixed cube triangles into owned heap storage,
so it never returns a pointer to a local array. Release the mesh with
`vector_free(&cube.tris)` once finished. Do not copy an owning mesh and then free
both copies. Triangle values themselves contain no pointers and can be copied.

## Files

| File | Responsibility |
| --- | --- |
| `include/vectors.h` | Geometry/container types and math declarations |
| `src/vectors.c` | Triangle storage, matrix-vector multiplication, X/Z rotation matrices |
| `include/shapes.h`, `src/shapes.c` | Cube mesh construction and projected triangle outlines |
| `include/raster.h`, `src/raster.c` | One-pixel Bresenham lines, using `DrawPixel` |
| `src/main.c` | Window, animation, transform/project/draw loop and cleanup |
| `Makefile` | Compile all `src/*.c` and link raylib using pkg-config |
| `tests/test_engine.c` | Headless regression tests and window/pixel stubs |

Headers declare the shared types and functions; source files contain their
implementations. The compiler builds each `.c` separately, and the linker combines
those object files with raylib. A header include does not include a `.c` file.

## Rendering pipeline

For each frame, `main.c`:

1. Advances a rotation angle using `GetFrameTime()` and builds zero-initialised
   rotation matrices. Z rotation is followed by X rotation at half the speed.
2. Copies each triangle out of the mesh. Original model vertices remain unchanged.
3. Subtracts the cube centre `(0.5, 0.5, 0.5)` so rotation is about its centre.
4. Rotates each point and adds 3 to z, placing the model in front of the camera.
5. Projects the triangle, dividing by homogeneous w (which is depth for this
   projection matrix).
6. Maps projected x/y coordinates into window pixels.
7. Calls `draw_triangle`, which sends three edges to `raster_draw_line`.

The projection matrix uses `height / width` for its horizontal scale, a vertical
field of view of 90 degrees, and near/far distances of 0.1 and 1000. Unspecified
matrix elements must be zero. The tutorial stores translation in `m[3][0..2]`;
do not mix that indexing with a differently laid-out matrix implementation.

`MultiplyMatrixVector(input, &output, &matrix)` takes its input by value and
writes through an output pointer, replacing the tutorial's C++ output reference.
Its boolean return is false for an invalid divide or non-finite result; on failure
it leaves the output unchanged. Passing the same variable as input and output
is safe because the input was copied before writing.

The low-level line API uses the order `(x0, y0, x1, y1, colour)`. Custom names are
`raster_draw_line` and `draw_triangle`, avoiding raylib's `DrawLine` and
`DrawTriangle`. The old rectangle, thick-line and separate X/Y-line APIs have been
removed; all visible shape edges now come from `triangle` objects.

## Scope and next steps

This is the Part 1 wireframe stage. Unlike the tutorial's rotation around the unit
cube's corner, this demo rotates around its centre. It otherwise uses the same
stages: rotation, forward translation, projection, screen mapping, rasterisation.

The cube remains safely in front of the near plane under every rotation. A guard
skips triangles with a vertex outside the near/far range; it is not proper plane
clipping. The low-level rasteriser expects finite, reasonably bounded screen
coordinates. The demo guarantees that for this cube; arbitrary large/offscreen
models will need clipping before rasterisation.

For Part 2, add normals, back-face culling and lighting at the marked point after
transformation and before projection; add depth sorting and filled triangle
rasterisation after projection. The triangle container can already grow for model
loading, but no OBJ loader, face filling, culling or lighting is implemented yet.

## Tutorial references

- [Part 1 video](https://www.youtube.com/watch?v=ih20l3pJoeU)
- [Part 2 video](https://www.youtube.com/watch?v=XgMWc6LumG4)
- [Author's Part 1 source](https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part1.cpp)
- [Author's Part 2 source](https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part2.cpp)

The tutorial is by javidx9 / OneLoneCoder; this project adapts the concepts to C
and raylib instead of its C++ console framework. The cube's face labels and vertex
ordering are retained from the user's tutorial-based mesh.
