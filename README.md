# Solar System OpenGL Simulation

A small interactive 3D solar-system scene implemented in C++ with the fixed-function OpenGL API and FreeGLUT. The application renders a stylized Sun, Earth, Jupiter, stars, Vega, a black hole, and two representations of the ISS, with simple animation and keyboard/mouse interaction.

This project was developed as part of postgraduate diploma coursework in computer graphics.

## Objectives

- Practice 2D and 3D rendering with OpenGL primitives.
- Build reusable scene-drawing and initialization functions.
- Apply transformations, perspective projection, depth testing, and animation timers.
- Demonstrate keyboard and mouse callbacks in an interactive graphics application.

## Features

- Procedurally generated star field.
- Stylized Sun, Earth, and Jupiter models built from OpenGL primitives and generated vertices.
- Simplified ISS models, Vega point light representation, and black-hole illustration.
- Perspective camera and depth-buffered rendering.
- Toggle animation with the `S` key or left mouse button.
- Animated planetary angles, Sun rotation, ISS movement, and typewriter-style scene text.

## Technology stack

- C++
- OpenGL and GLU (legacy fixed-function pipeline)
- FreeGLUT / GLUT-compatible headers and libraries
- Windows API integration through `windows.h`
- Code::Blocks project configuration with MinGW/GCC

There is no database, network service, environment-variable configuration, package manager manifest, or external runtime service in this project.

## Project structure

```text
.
├── main.cpp       # Application entry point, scene setup, rendering, and interaction
├── Solar.cbp      # Code::Blocks project file
├── Screenshot.png # Example application screenshot
├── .gitignore     # Publication and build-artifact exclusions
└── README.md      # Project documentation
```

`bin/`, `obj/`, `*.depend`, and `*.layout` are local/generated Code::Blocks files and are intentionally excluded from future commits.

## Requirements

- Windows
- Code::Blocks with a MinGW/GCC toolchain, or an equivalent C++ compiler setup
- OpenGL development libraries supplied by Windows
- FreeGLUT development headers and libraries

The current Code::Blocks project file contains machine-specific MinGW paths. On another computer, configure the compiler's include and library directories for the local FreeGLUT installation before building.

## Build and run

1. Install Code::Blocks with MinGW and install/configure FreeGLUT.
2. Open `Solar.cbp` in Code::Blocks.
3. Adjust the compiler and linker paths if they differ from the original development machine.
4. Select the `Debug` or `Release` target and build the project.
5. Run the resulting executable from Code::Blocks or the generated `bin` directory.

The project links against `freeglut`, `opengl32`, `glu32`, `winmm`, and `gdi32`.

## Controls

- Press `S` to start or pause animation.
- Click the left mouse button to start or pause animation.

## Testing

No automated tests are included. Verification is currently manual: build the application, launch the window, confirm that the scene renders, and exercise the animation controls.

## Architecture and design notes

The application is intentionally kept as a single-source coursework project. Global scene state is initialized in `init()`, frame rendering is coordinated by `display()`, viewport updates are handled by `reshape()`, and animation scheduling is handled by the GLUT timer callback. Individual `draw...` functions encapsulate the rendering of major scene elements.

The scene uses immediate-mode OpenGL and generated geometry rather than external model or texture files. This keeps the project self-contained but also reflects the limitations of a teaching-oriented legacy OpenGL implementation.

## Limitations and possible improvements

- The project depends on Windows and a local FreeGLUT/MinGW setup.
- The Code::Blocks project contains machine-specific paths that should be made portable before broader distribution.
- The rendering uses deprecated immediate-mode OpenGL and has no lighting, shader, texture, or model-loading pipeline.
- There is no automated test suite or cross-platform build configuration.
- Future work could separate scene objects into classes/modules, replace immediate mode with modern OpenGL, add deterministic animation timing, and provide a CMake-based build.

## Portfolio note

This repository demonstrates practical experience with C++, OpenGL rendering, geometric construction, transformations, callbacks, and real-time animation in a compact academic project. The visual style and implementation are intentionally simple and should be read in that coursework context.

## AI assistance disclosure

AI-assisted development tools were used where appropriate for development assistance, debugging, code review, research, and documentation. The project remains the author's coursework project; this disclosure does not imply that AI independently authored or owns it.

## License

This project is licensed under the [MIT License](LICENSE). This assumes the project author has permission to redistribute the coursework source and that it does not contain restricted instructor-provided code or other third-party material.

## Screenshots

The repository includes `Screenshot.png` as an example render. A short demo GIF would also improve the portfolio presentation, especially for showing the animation controls; it should be captured from the running application rather than generated from a still image.
