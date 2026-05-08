$ErrorActionPreference = "Stop"

$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"

& "C:\msys64\mingw32\bin\g++.exe" `
    part_e_graphics_h.cpp `
    -o part_e_graphics_h.exe `
    -static `
    -static-libgcc `
    -static-libstdc++ `
    -Ivendor\winbgim\include `
    -Lvendor\winbgim\lib `
    -lbgi `
    -lgdi32 `
    -lcomdlg32 `
    -luuid `
    -loleaut32 `
    -lole32

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Built part_e_graphics_h.exe"
