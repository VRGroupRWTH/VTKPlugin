@echo off
setlocal enabledelayedexpansion

set arg1=%1

set INITIAL_DIR=%CD%
cd /d "%~dp0"

if /I "%arg1%" == "Debug" (
    set VTK_BUILD_CONFIG=Debug
) ELSE (
    set VTK_BUILD_CONFIG=Release
)

mkdir Temporary
cd    Temporary

curl -L "https://github.com/Kitware/VTK/archive/refs/tags/v9.3.0.rc0.zip" -o download.zip
tar -xf download.zip
del download.zip
move VTK-9.3.0.rc0 VTKLibrary

cd    VTKLibrary
mkdir Build\%VTK_BUILD_CONFIG%
mkdir Install\%VTK_BUILD_CONFIG%

echo "Doing a %VTK_BUILD_CONFIG% Build."

cd    Build
cmake .. -DVTK_GROUP_ENABLE_Rendering=DONT_WANT
cmake --build   . --config %VTK_BUILD_CONFIG% --parallel 20
cmake --install . --config %VTK_BUILD_CONFIG% --prefix ..\Install\%VTK_BUILD_CONFIG%

cd ..
xcopy /s Install\%VTK_BUILD_CONFIG%\include\vtk-9.3 ..\..\Source\ThirdParty\VTKLibrary\Public\
xcopy /s Install\%VTK_BUILD_CONFIG%\lib             ..\..\Source\ThirdParty\VTKLibrary\Windows\x64\%VTK_BUILD_CONFIG%\
xcopy /s Install\%VTK_BUILD_CONFIG%\bin             ..\..\Source\ThirdParty\VTKLibrary\Windows\x64\%VTK_BUILD_CONFIG%\
xcopy /s Install\%VTK_BUILD_CONFIG%\bin             ..\..\Binaries\ThirdParty\Win64\

cd ..\..

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