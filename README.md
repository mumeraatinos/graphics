# OpenGL Primitive Examples

This project renders a reference sheet of common OpenGL primitive shapes using GLUT.

## Part 1 Screenshot

![OpenGL primitive examples](assets/primitive_cat.png)

## Part B Cartoon Scene

The second assignment part is in `modules/frontier_scene`. It contains the redesigned formal 3D cartoon character scene for the near/far plane requirement, with the current source in `frontier_scene.cpp` and final screenshot in `part_b_camera_orbit_screenshot.png`.

## Part 1 What It Shows

- `GL_POINTS`
- `GL_LINES`
- `GL_LINE_STRIP`
- `GL_LINE_LOOP`
- `GL_TRIANGLES`
- `GL_TRIANGLE_STRIP`
- `GL_TRIANGLE_FAN`
- `GL_QUADS`
- `GL_QUAD_STRIP`
- `GL_POLYGON`

## Build

```powershell
g++ src\primitive_catalog.cpp -o primitive_catalog.exe -lfreeglut -lopengl32 -lglu32
.\primitive_catalog.exe
```
