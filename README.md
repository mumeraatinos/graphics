# OpenGL Primitive Examples

This project renders a reference sheet of common OpenGL primitive shapes using GLUT.

## Part 1 Screenshot

![OpenGL primitive examples](assets/primitive_cat.png)

## What It Shows

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
