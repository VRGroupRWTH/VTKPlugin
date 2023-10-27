@echo off
setlocal enabledelayedexpansion

set INITIAL_DIR=%CD%
cd /d "%~dp0"

mkdir Temporary
cd    Temporary

curl -L "https://github.com/Kitware/VTK/archive/refs/tags/v9.3.0.rc0.zip" -o download.zip
tar -xf download.zip
del download.zip
move VTK-9.3.0.rc0 VTKLibrary

cd    VTKLibrary
mkdir Build
mkdir Install
cd    Build
cmake .. -DVTK_GROUP_ENABLE_Rendering=NO
cmake --build   . --config Release --parallel 32
cmake --install . --prefix ..\Install

cd ..
xcopy /s Install\include\vtk-9.3 ..\..\Source\ThirdParty\VTKLibrary\Public\
xcopy /s Install\lib             ..\..\Source\ThirdParty\VTKLibrary\x64\Release\
xcopy /s Install\bin             ..\..\Source\ThirdParty\VTKLibrary\x64\Release\

cd ..\..
rmdir /s /q Temporary

cd /d %INITIAL_DIR%