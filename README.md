# Pixel shapes in C

A small C17 project for learning to draw shapes by choosing individual pixels.
The current demo opens an 800 x 600 window and draws a rotating wireframe cube.
Pixel drawing lives in `src/raster.c`; shape construction and rotation live in
`src/shapes.c`.

## Build and run

This Mac already has Apple Clang, Make, Homebrew raylib, and pkg-config installed.
From Terminal:

```sh
cd /Users/nick/Desktop/raylibProject
make
make run
```

`make` builds `build/pixel_shapes`. `make run` builds if needed, then launches it.
Press Escape or use the window's close button to exit.
Use `make -B` if you need to force a complete rebuild.

In VS Code, open this folder and use Cmd+Shift+B to build. Use
Terminal > Run Task > Run C project to build and run. These tasks use the
Makefile; building just the active C file will omit the other source files and
raylib linker settings.

## Three-axis rotation

Hold the keys below to change the cube's orientation:

| Keys | Axis | Angle field |
| --- | --- | --- |
| Q / E | X | `psi` (+ / -) |
| W / S | Y | `theta` (+ / -) |
| A / D | Z | `phi` (+ / -) |

Each held key changes its angle by 10 degrees per frame, with the current target
of 20 FPS. Speed therefore depends on the actual frame rate.

`DoubleAngle3` in `include/vectors.h` stores all three angles in degrees.
`rotate_vector`, `rotate_rect`, and `rotate_triangle` accept this type. Rectangle
and triangle rotation modifies their vertices about their centroids. Cube drawing
rotates temporary copies about `Cube.centre`, using the stored absolute angles.

The combined transform is `Rz(phi) * Ry(theta) * Rx(psi)`: X rotation is applied
first, then Y, then Z. Setting `psi` to zero recovers the previous Y-then-Z
rotation. The cube controls edit these Euler angles; with other angles already
set, changing one is not necessarily a rotation around a fixed screen axis.
The rendering uses orthographic projection: x and y are drawn, z is omitted.
Screen y increases downward, so positive Z rotation appears clockwise.

The cube constructor now takes:

```c
get_cube_from_centre(centre, width, phi, theta, psi, colour);
```

For example, pass three zeros for an initially unrotated cube. A rotation value
for the shape functions can be initialised as:

```c
DoubleAngle3 angles = { .theta = 0, .phi = 0, .psi = 10 };
```

All three fields must be initialised; omitted fields in a brace initialiser are
zero-initialised automatically.

## Project files

```text
raylibProject/
  Makefile                       Build instructions
  include/raster.h               Your drawing module's public declarations
  src/main.c                     Program entry point, window, frame loop
  src/raster.c                   Your drawing module's implementations
  include/shapes.h               Shape types and function declarations
  src/shapes.c                   Shape drawing and three-axis rotation
  include/vectors.h              Vector types and DoubleAngle3
  src/vectors.c                  Dynamic integer vector functions
  .vscode/c_cpp_properties.json   Editor C17 and header lookup settings
  .vscode/tasks.json              Editor commands that invoke Make
  build/                         Generated executable, .o and .d files
```

The `src` and `include` directory names are conventions, not C requirements.
The Makefile automatically finds `.c` files directly inside `src/`.
All project source files use C, with Clang's `-std=c17` mode.

## How a C program is built

1. **Preprocess:** resolve `#include`, macros, and conditional directives. For
   example, `#include "raster.h"` makes its declarations available in `main.c`.
2. **Compile:** compile each resulting translation unit separately. Here,
   `src/main.c` becomes `build/main.o` and `src/raster.c` becomes `build/raster.o`.
   These object files contain machine code, but some referenced functions still
   need to be resolved.
3. **Link:** combine the object files and link raylib to produce
   `build/pixel_shapes`. The linker resolves `raster_put_pixel` from `raster.o`
   and records the program's dependency on the installed raylib shared library.
   macOS loads that library when the executable runs.

The Makefile runs these commands for you. `-Iinclude` tells the compiler where
to find your headers; `-c` compiles without linking; `-o` names an output file.
Warnings are enabled, and `-g -O0` keeps debugging straightforward.
The generated `.d` files track included headers, so editing `raster.h` rebuilds
the affected object files next time you run Make.

## What headers do

`include/raster.h` contains this function **declaration**:

```c
void raster_put_pixel(int x, int y, Color color);
```

It tells the compiler the function's name, parameter types, and return type
(`void` means no returned value). This lets `main.c` call the function with type
checking without needing its implementation in the same file.

`src/raster.c` contains the **definition**: the function body that calls
`DrawPixel`. It includes its own header so the compiler can check that the
definition agrees with the public declaration. `src/main.c` includes the header
to use that interface. You do not include `raster.c`: compile it separately and
link its object file.

The `#ifndef RASTER_H` / `#define RASTER_H` / `#endif` lines are an **include
guard**. They prevent the header's contents from being processed more than once
within a translation unit. They do not prevent duplicate function definitions
across separate `.c` files; ordinary public function bodies belong in `.c` files.

The header itself includes `<raylib.h>` because its declaration uses raylib's
`Color` type. This makes `raster.h` usable without requiring callers to include
other headers in a particular order. Quoted includes first search relative to
the including file, then configured include paths; angle brackets use configured
system/library search paths. `-I` directories also participate in that search.

## Where raylib fits

Your functions choose which pixels belong to a shape. Raylib provides the window,
graphics context, input/event handling, and the `DrawPixel` operation that displays
each chosen pixel. `raster_put_pixel` is just a thin wrapper around that operation;
the shape algorithms are yours to implement.

There are two distinct parts of using raylib:

- **Declarations:** `<raylib.h>` gives the compiler types such as `Color` and
  declarations such as `InitWindow` and `DrawPixel`.
- **Compiled library:** the installed `libraylib.dylib` supplies the function
  implementations. Including the header alone does not link the library.

The Makefile asks `pkg-config --cflags raylib` for the header search flags and
`pkg-config --libs raylib` for the library search/link flags (`-L... -lraylib`).
This uses the existing Homebrew installation without copying raylib into the
project. The resulting executable depends on that installation.
VS Code's header settings only help the editor: the Makefile controls the build.

## Starting with the window

Read `src/main.c` from top to bottom:

1. `InitWindow` creates the window and graphics context.
2. `SetTargetFPS(20)` sets a target frame rate.
3. `while (!WindowShouldClose())` repeats until Escape or a close request.
4. `BeginDrawing` starts drawing the frame. `ClearBackground` clears it.
5. Put drawing calls after the clear and before `EndDrawing`.
6. `EndDrawing` presents the frame and normally handles frame timing and input
   polling. After the loop, `CloseWindow` releases the window and graphics context.

The window uses screen coordinates: `(0, 0)` is the top-left, x increases right,
and y increases down. For this window, integer pixel coordinates range from
0 to 799 horizontally and 0 to 599 vertically. The screen is cleared each frame,
so your shape functions must be called again each frame to keep shapes visible.
These calls draw into a graphics buffer; they do not directly write to a physical
display pixel. High-DPI display scaling can also affect that mapping.

The current drawing section calls `draw_cube`. Its keyboard controls update the
cube angles before the frame is drawn. To experiment with individual pixels,
place a `DrawPixel` call between `ClearBackground` and `EndDrawing`.

## Official references

- [Basic window example (raylib 6.0)](https://github.com/raysan5/raylib/blob/6.0/examples/core/core_basic_window.c)
- [Raylib API reference](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Raylib macOS setup and pkg-config](https://github.com/raysan5/raylib/wiki/Working-on-macOS)
