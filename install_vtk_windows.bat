@echo off
setlocal enabledelayedexpansion

set arg1=%1
set INITIAL_DIR=%CD%
cd /d "%~dp0"
set VTKLIB_DIR=%CD%\Source\ThirdParty\VtkLibrary

REM check for build config
if /I "%arg1%" == "Debug" (
    set VTK_BUILD_CONFIG=Debug
) ELSE (
    set VTK_BUILD_CONFIG=Release
)

mkdir Temporary
cd    Temporary

REM download vtk library (exchange for other version if needed)
curl -L "https://github.com/Kitware/VTK/archive/refs/tags/v9.3.0.rc0.zip" -o download.zip
tar -xf download.zip
del download.zip
move VTK-9.3.0.rc0 VTKLibrary

REM if download/extraction didn't work cancel
if not exist VTKLibrary\ exit

cd    VTKLibrary
mkdir Build\%VTK_BUILD_CONFIG%
mkdir Install\%VTK_BUILD_CONFIG%

REM build
echo "Doing a %VTK_BUILD_CONFIG% Build."

cd    Build
cmake .. -DVTK_GROUP_ENABLE_Rendering=DONT_WANT
cmake --build   . --config %VTK_BUILD_CONFIG% --parallel 20
cmake --install . --config %VTK_BUILD_CONFIG% --prefix %VTKLIB_DIR%\%VTK_BUILD_CONFIG%

REM move includes to expected locations (the same for all build configs)
move %VTKLIB_DIR%\%VTK_BUILD_CONFIG%\include\vtk-9.3 %VTKLIB_DIR%\Public

cd ..\..\..

REM cleanup?
CHOICE /C YN /M "Delete the temporary build folder (./Temporary)?"
if errorlevel 2 goto KEEP_TEMP
if errorlevel 1 goto DELETE_TEMP

:DELETE_TEMP
echo "Deleting temporary build folder..."
rmdir /s /q Temporary
goto END

:KEEP_TEMP
goto END

:END
cd /d %INITIAL_DIR%