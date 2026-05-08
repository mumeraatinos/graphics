# Part E - graphics.h Image Creation

This folder contains a separate C/C++ `graphics.h` submission for Part E. The program opens directly on a `graphics.h` recreation of the Part A OpenGL lines-and-shapes reference sheet.

## Hosted Output Page

[https://gl.applications.co.zw/parts/part-e.html](https://gl.applications.co.zw/parts/part-e.html)

## Screenshots

<img src="part_e_all_primitives.png" alt="Part E recreated primitive page" width="780">

## What It Shows

The program redraws the Part A primitive reference sheet using classic BGI/WinBGIm drawing functions:

- `GL_POINTS`
- `GL_LINES`
- `GL_LINE_STRIP`
- `GL_LINE_LOOP`
- `GL_TRIANGLES`
- `GL_TRIANGLE_STRIP`
- `GL_TRIANGLE_FAN`
- `GL_QUADS`
- Larger `GL_QUAD_STRIP`
- `GL_POLYGON`

## Files

- `part_e_graphics_h.cpp`: main C++ source file using `#include <graphics.h>`.
- `part_e_all_primitives.bmp`: saved `graphics.h` recreation of the Part A primitive sheet.
- `part_e_all_primitives.png`: web-friendly PNG version of the recreated primitive page.
- `README.md`: build, run, and marking notes for Part E.

## Build

This part uses the vendored WinBGIm files in `vendor/winbgim`:

- `vendor/winbgim/include/graphics.h`
- `vendor/winbgim/include/winbgim.h`
- `vendor/winbgim/lib/libbgi.a`

These files were added from the WinBGIm helper repository at <https://github.com/acsfid/graphics.h>.

The provided `libbgi.a` is a 32-bit library, so this project is built with the MSYS2 32-bit MinGW compiler at `C:\msys64\mingw32\bin\g++.exe`.

Quick build command from this folder:

```powershell
.\build.ps1
```

Example MinGW command:

```powershell
$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"
C:\msys64\mingw32\bin\g++.exe part_e_graphics_h.cpp -o part_e_graphics_h.exe -static -static-libgcc -static-libstdc++ -Ivendor\winbgim\include -Lvendor\winbgim\lib -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

## Run

```powershell
.\part_e_graphics_h.exe
```

To generate the output BMP automatically and close the program:

```powershell
.\part_e_graphics_h.exe --save-and-exit
```

That command saves `part_e_all_primitives.bmp`.

## Controls

- `S`: save the current page as `part_e_all_primitives.bmp`.
- `Q` or `Esc`: close the program.

## Marking Notes

The work is kept separate from the OpenGL parts and uses `graphics.h` drawing calls only. It demonstrates points, lines, connected line strips, closed loops, triangle structures, quad structures, text labels, and a direct BGI recreation of the Part A primitive reference sheet.
