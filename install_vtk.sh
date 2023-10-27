#!/bin/bash


INITIAL_DIR=$PWD
cd "${0%/*}"

mkdir Temporary
cd    Temporary

wget -O download.tar.gz "https://github.com/Kitware/VTK/archive/refs/tags/v9.3.0.rc0.tar.gz"
tar -xf download.tar.gz
rm download.tar.gz
mv VTK-9.3.0.rc0 VTKLibrary

cd    VTKLibrary
mkdir Build
mkdir Install
cd    Build
cmake .. -DVTK_GROUP_ENABLE_Rendering=NO
cmake --build   . --config Release --parallel 32
cmake --install . --prefix ../Install

cd ..
mkdir -p ../../Source/ThirdParty/VTKLibrary/Public        && cp -a ./Install/include/vtk-9.1/. "$_"
mkdir -p ../../Source/ThirdParty/VTKLibrary/Linux/Release && cp -a ./Install/lib/. "$_"
mkdir -p ../../Source/ThirdParty/VTKLibrary/Linux/Release && cp -a ./Install/bin/. "$_"

cd ../..
rm -rf Temporary

cd %INITIAL_DIR%