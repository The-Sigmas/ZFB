# Zero-Frame Buffer

## Elevator Pitch
Our engine is a lightweight, fast, and fully C-based framebuffer game engine that directly interacts with memory-mapped video buffers, enabling ultra-low-latency rendering. It features a temporal framebuffer system that optimizes performance by updating only changed pixels, leading to efficient memory use and smooth animations. The goal is to provide a clean, extendable, and well-documented engine for retro-style, physics-based, and experimental games
---
## What makes ZFB Special?

| Feature                  | Our Engine  | Other Engines  |
|-------------------------|--------------|----------------------------------|
| **Memory-Mapped Rendering**  |  Direct pixel access | Uses GPU or abstraction layers |
| **Temporal Framebuffer**    |  Only updates changed pixels |  Redraws full screen per frame |
| **Written in Pure C**       |  No dependencies, lightweight |  Often requires libraries (SDL, OpenGL) |
| **Minimalist & Clean**      |  Simple API, easy to modify |  Often bloated with features |
| **Optimized for Retro / Low-Power Devices** | Works on older hardware | Assumes high GPU usage |
| **Exteding-Friendly**         | Fully documented, easy to extend | Hidden complexity |
---
## User Personas
| User Type | Needs |
|-----------|-------|
| **Indie Developers** | Want a fast and simple game engine for 2D games without GPU requirements. |
| **Retro Game Developers** | Need a framebuffer system that mimics classic hardware. |
| **Game Jam Participants** | Want an engine that boots up quickly, has a simple API, and is easy to deploy. |
| **Graphics Programmers** | Want to experiment with pixel-perfect rendering and visual effects. |
---
## User Stories
### User Story 1
 **As a game developer, I want to render pixels directly to the screen** so that I can create low-level graphics effects.

### Acceptance Criteria

- [ ] A function exists to set individual pixels at specific (x, y) coordinates.
- [ ] The framebuffer can be cleared and updated at a defined refresh rate.
- [ ] The pixel format (RGB, ARGB, etc.) is documented and consistent.
- [ ] The rendered pixels appear on the screen without extra processing.
- [ ] The system supports basic drawing operations (lines, rectangles, etc.).

### User Story 2
**As a performance-focused developer, I want to update only changed pixels** so that I can optimize rendering.

#### Acceptance Criteria

- [ ] A secondary buffer tracks which pixels have changed since the last frame.
- [ ] Only modified pixels are written to the framebuffer, reducing redundant updates.
- [ ] Performance benchmarking shows that partial updates improve frame times.
- [ ] A function exists to force full-screen updates when needed.
- [ ] Debug mode can visualize which pixels were updated in the last frame.


### User Story 3
**As an indie developer, I want an engine without external dependencies** so that I can build projects without worrying about third-party libraries.

#### Acceptance Criteria

- [ ] The engine is written in pure C with no third-party libraries required.
- [ ] The only system API dependencies are OS-level framebuffer access (e.g., /dev/fb0 on Linux).
- [ ] The compilation process is simple and works with standard C compilers (e.g., GCC, Clang).
- [ ] No linking against SDL, OpenGL, or external rendering libraries.
- [ ] The build system is documented to ensure users can compile and run without extra setup.

### User Story 4
 **As a graphics programmer, I want to modify the framebuffer in real-time** so that I can experiment with shaders and visual effects.

#### Acceptance Criteria
- [ ] A function exists to access and modify pixel data dynamically.
- [ ] The framebuffer updates in real-time, with low latency (~16ms per frame for 60 FPS).
- [ ] A function allows for real-time effects like color shifts, blurring, or distortions.
- [ ] Users can manipulate framebuffers without affecting engine stability.
- [ ] Debug mode allows visualizing buffer changes per frame

### User Story 5
**As a beginner in C, I want the code to be simple and well-documented** so that I can learn and understand it easily.

#### Acceptance Criteria
- [ ] Every function is clearly commented with explanations.
- [ ] The project includes a README with setup instructions.
- [ ] The code follows consistent naming conventions and formatting.
- [ ] A basic example program demonstrates rendering a simple scene.
- [ ] The engine provides error messages and logs for debugging.
- [ ] The documentation explains how the framebuffer works and how to modify it.

