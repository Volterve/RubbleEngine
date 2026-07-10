# RubbleEngine

A minimal C++/OpenGL game engine, built as a static library (`EngineCore`) and
driven by a small GLFW app (`runtime`).

## Requirements

- A C++20 compiler (MSVC, Clang, or GCC)
- [CMake](https://cmake.org/) 3.20+
- [Python 3](https://www.python.org/) on your PATH (used to generate the GL loader)
- Git + an internet connection **the first time you build**

GLFW, glad, and glm are downloaded automatically by CMake — you do **not** need
to install OpenGL, GLFW, or any SDK by hand.

## Build & run

```sh
git clone <repo-url> RubbleEngine
cd RubbleEngine
cmake -B build            # first run downloads glfw/glad/glm (needs internet)
cmake --build build
```

Then run the app:

- Windows (MSVC): `build\runtime\Debug\runtime.exe`
- Linux / macOS:  `./build/runtime/runtime`

You should see a shaded cube.

## Develop

This is a CMake project, so open the **folder** (not a `.sln`) — CMake stays the
source of truth. When you add a new `.cpp`, list it in the relevant
`CMakeLists.txt`; new `.h` files need no change.

### Visual Studio (2022 / 2026)

1. **File → Open → Folder…** and pick the repo root.
2. Wait for the "CMake generation" in the Output window to finish.
3. Set the startup item dropdown in the toolbar to **`runtime.exe`**.
4. Press **F5** (debug) or **Ctrl+F5** (run). Set breakpoints in the gutter.

### VS Code

1. Install the **C/C++** and **CMake Tools** extensions (both by Microsoft).
2. **File → Open Folder…** and pick the repo root.
3. When prompted, select a **Kit** (your Visual Studio / Clang / GCC compiler).
4. Build with **F7**, then run/debug with the ▶ / 🐞 buttons in the status bar.

Both editors re-run CMake automatically when you save a `CMakeLists.txt`.

## Controls

| Key            | Action        |
| -------------- | ------------- |
| `W A S D`      | Pan camera    |
| `Q` / `E`      | Rotate 90 deg |
| Scroll wheel   | Zoom          |
| `Esc`          | Quit          |

## Layout

```
engine/    EngineCore static library (Camera, Shader) — no window code
runtime/   GLFW app that creates the window and draws a cube
shaders/   GLSL shaders (basic.vert / basic.frag)
```

The engine has no windowing dependency, so the same `EngineCore` library can
later be linked into a Qt level editor that supplies its own OpenGL context.
