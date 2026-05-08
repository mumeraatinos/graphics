$ErrorActionPreference = "Stop"

$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"

& "C:\msys64\mingw32\bin\g++.exe" `
    part_f_jet_graphics_h.cpp `
    -o part_f_jet_graphics_h.exe `
    -static `
    -static-libgcc `
    -static-libstdc++ `
    -I..\retro_bitmap\vendor\winbgim\include `
    -L..\retro_bitmap\vendor\winbgim\lib `
    -lbgi `
    -lgdi32 `
    -lcomdlg32 `
    -luuid `
    -loleaut32 `
    -lole32

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Built part_f_jet_graphics_h.exe"
