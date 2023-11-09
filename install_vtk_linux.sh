#!/bin/bash

# cast parameter1 to lowercase
arg1=$(echo $1 | awk '{print tolower($0)}')
INITIAL_DIR=$PWD
cd "${0%/*}"
VTKLIB_DIR=$PWD/Source/ThirdParty/VTKLibrary

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
cmake --install . --config $VTK_BUILD_CONFIG --prefix $VTKLIB_DIR/Linux/$VTK_BUILD_CONFIG

# move files to expected locations
mv $VTKLIB_DIR/Linux/$VTK_BUILD_CONFIG/include/vtk-9.3 $VTKLIB_DIR/Public

cd ../../..

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