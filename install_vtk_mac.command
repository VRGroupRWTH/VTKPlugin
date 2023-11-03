#!/bin/bash


arg1=$1

INITIAL_DIR=$PWD
cd "${0%/*}"

if [ "${arg1,,}" = "debug" ]; then
  BUILD_CONFIG=Debug
else
  BUILD_CONFIG=Release
fi

mkdir Temporary
cd    Temporary

wget -O download.tar.gz "https://github.com/Kitware/VTK/archive/refs/tags/v9.3.0.rc0.tar.gz"
tar -xf download.tar.gz
rm download.tar.gz
mv VTK-9.3.0.rc0 VTKLibrary

cd    VTKLibrary
mkdir -p Build/$BUILD_CONFIG
mkdir -p Install/$BUILD_CONFIG

printf "Doing a $VTK_BUILD_CONFIG Build."

cd    Build
cmake .. -DVTK_GROUP_ENABLE_Rendering=DONT_WANT
cmake --build   . --config $BUILD_CONFIG --parallel 20
cmake --install . --config $BUILD_CONFIG --prefix ../Install/$BUILD_CONFIG

cd ..
mkdir -p ../../Source/ThirdParty/VTKLibrary/Public        && cp -a ./Install/$BUILD_CONFIG/include/vtk-9.3/. "$_"
mkdir -p ../../Source/ThirdParty/VTKLibrary/Mac/$BUILD_CONFIG && cp -a ./Install/$BUILD_CONFIG/lib/. "$_"
mkdir -p ../../Source/ThirdParty/VTKLibrary/Mac/$BUILD_CONFIG && cp -a ./Install/$BUILD_CONFIG/bin/. "$_"
mkdir -p ../../Binaries/ThirdParty/Mac && cp -a ./Install/$BUILD_CONFIG/bin/. "$_"

cd ../..

printf 'Delete the temporary build folder (./Temporary)? [Y,N]?'
read answer

if [ "$answer" != "${answer#[Yy]}" ] ;then
    printf 'Deleting temporary build folder...'
    rm -rf Temporary
else
    printf 'Keeping build folder.'
fi

cd %INITIAL_DIR%