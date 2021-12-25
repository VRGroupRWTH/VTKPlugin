@echo off
setlocal enabledelayedexpansion

set INITIAL_DIR=%CD%
cd /d "%~dp0"

mkdir Temporary
cd    Temporary

curl -L "https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1-windows-x86_64.zip" -o download.zip
tar -xf download.zip
del download.zip
move cmake-3.22.1-windows-x86_64 CMake

curl -L "https://github.com/Kitware/VTK/archive/refs/tags/v9.1.0.zip" -o download.zip
tar -xf download.zip
del download.zip
move VTK-9.1.0 VTK

cd    VTK
mkdir Build
mkdir Install
cd    Build
..\..\CMake\bin\cmake .. -DVTK_GROUP_ENABLE_Rendering=NO
..\..\CMake\bin\cmake --build   . --config Release --parallel 32
..\..\CMake\bin\cmake --install . --prefix ..\Install

cd ..
xcopy /s Install\include\vtk-9.1 ..\..\Source\ThirdParty\VTK\Public\
xcopy /s Install\lib             ..\..\Intermediate\ThirdParty\VTK\Win64\
xcopy /s Install\bin             ..\..\Binaries\ThirdParty\VTK\Win64\    

cd ..\..
rmdir /s /q Temporary

cd /d %INITIAL_DIR%