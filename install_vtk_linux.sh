#!/bin/bash

# cast parameter1 to lowercase
arg1=$(echo $1 | awk '{print tolower($0)}')

INITIAL_DIR=$PWD
cd "${0%/*}"

# check for build config
if [ $arg1="debug" ]; then
  VTK_BUILD_CONFIG=Debug
else
  VTK_BUILD_CONFIG=Release
fi

mkdir Temporary
cd    Temporary

# download vtk library (exchange for other version if needed)
curl -L "https://github.com/Kitware/VTK/archive/refs/tags/v9.3.0.rc0.tar.gz" -o download.tar.gz
tar -xf download.tar.gz
rm download.tar.gz
mv VTK-9.3.0.rc0 VTKLibrary

# if download/extraction didn't work cancel
if [ ! -d "VTKLibrary" ]; then
  exit
fi

cd    VTKLibrary
mkdir -p Build/$VTK_BUILD_CONFIG
mkdir -p Install/$VTK_BUILD_CONFIG

# build
printf "Doing a $VTK_BUILD_CONFIG Build."

cd    Build
cmake .. -DVTK_GROUP_ENABLE_Rendering=DONT_WANT
cmake --build   . --config $VTK_BUILD_CONFIG --parallel 20
cmake --install . --config $VTK_BUILD_CONFIG --prefix ../Install/$VTK_BUILD_CONFIG

# copy files to expected locations
cd ..
mkdir -p ../../Source/ThirdParty/VTKLibrary/Public        && cp -a ./Install/$VTK_BUILD_CONFIG/include/vtk-9.3/. "$_"
mkdir -p ../../Source/ThirdParty/VTKLibrary/Linux/x64/$VTK_BUILD_CONFIG && cp -a ./Install/$VTK_BUILD_CONFIG/lib/. "$_"
mkdir -p ../../Source/ThirdParty/VTKLibrary/Linux/x64/$VTK_BUILD_CONFIG && cp -a ./Install/$VTK_BUILD_CONFIG/bin/. "$_"
mkdir -p ../../Binaries/ThirdParty/Linux && cp -a ./Install/$VTK_BUILD_CONFIG/bin/. "$_"

cd ../..

# cleanup?
printf 'Delete the temporary build folder (./Temporary)? [Y,N]?'
read answer

if [ "$answer" != "${answer#[Yy]}" ] ;then
    printf 'Deleting temporary build folder...'
    rm -rf Temporary
else
    printf 'Keeping build folder.'
fi

cd $INITIAL_DIR