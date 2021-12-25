#!/bin/bash


INITIAL_DIR=$PWD
cd "${0%/*}"

mkdir Temporary
cd    Temporary

wget -O download.tar.gz "https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1-linux-x86_64.tar.gz"
tar -xf download.tar.gz
rm download.tar.gz
mv cmake-3.22.1-linux-x86_64 CMake

wget -O download.tar.gz "https://github.com/Kitware/VTK/archive/refs/tags/v9.1.0.tar.gz"
tar -xf download.tar.gz
rm download.tar.gz
mv VTK-9.1.0 VTK

cd    VTK
mkdir Build
mkdir Install
cd    Build
../../CMake/bin/cmake .. -DVTK_GROUP_ENABLE_Rendering=NO
../../CMake/bin/cmake --build   . --config Release --parallel 32
../../CMake/bin/cmake --install . --prefix ../Install

cd ..
mkdir -p ../../Source/ThirdParty/VTK/Public      && cp -a ./Install/include/vtk-9.1/. "$_"
mkdir -p ../../Intermediate/ThirdParty/VTK/Linux && cp -a ./Install/lib/.             "$_"
mkdir -p ../../Binaries/ThirdParty/VTK/Linux     && cp -a ./Install/bin/.             "$_"

cd ../..
rm -rf Temporary

cd %INITIAL_DIR%