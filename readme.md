## VTK Plugin for Unreal Engine 5+

A plugin that links the [VTK Library](https://github.com/Kitware/VTK) to [Unreal Engine 5.3](https://docs.unrealengine.com/5.3/en-US/).

Currently, this plugin does the following:
- Download & build VTK (infer/change the version in the install script file)
- Expose VTK to your complete Unreal project (public, but you may want this private)
- Provide a blueprint function "DistanceBetweenTwoPoints" for testing `vtkMath.h` (based on an [official VTK example](https://examples.vtk.org/site/Cxx/SimpleOperations/DistanceBetweenPoints/), cross-compatible)
- Provide a blueprint function "ReadStructuredGridTest" for more complex vtk includes (loosely based on an [official VTK example](https://examples.vtk.org/site/Cxx/IO/ReadStructuredGrid/#download-and-build-readstructuredgrid), only Windows due to RTTI)
- ⚠️ Conditional cross-compatibility for Windows/Linux/Mac (tested)
  - Windows: VTK should work as-is without further adjustments
  - Linux/Mac: Certain VTK includes won't compile as they use [RTTI mechanics](https://en.wikipedia.org/wiki/Run-time_type_information)

This plugin is quite verbose as it aims to be a foundation for implementing & testing VTK functionality in UE.
Check the Unreal Log for `[VtkPlugin]` to see what's happening (also valid for the blueprint functions).

### ⚠️ Remarks on Windows

- Delay-loading (which would be better practice) is currently disabled and all VTK dlls are instantly loaded.
  - This prevents module reloading.

### ⚠️ Remarks on Linux/Mac

- No full cross-compatibility as VTK employs [RTTI](https://en.wikipedia.org/wiki/Run-time_type_information) which is by default not supported on Unreal Editor compiled for Unix ([also discussed here](https://forums.unrealengine.com/t/rtti-failed-compiling-when-enabled-for-4-23-linux/455083/22))
  - ~~**Solution A:** Recompile Unreal Engine with RTTI support~~
    -Not advised, will clash with other Plugins
  - **Solution B:** Add Wrapper module with RTTI enabled ([suggested here](https://forums.unrealengine.com/t/rtti-failed-compiling-when-enabled-for-4-23-linux/455083/13))
    - Adding a `bUseRTTI=true` in a `*.Build.cs` file enabled RTTI only for that module
    - Also see Unreals `OpenExrWrapper` module
    - Use PImpl pattern to handle all VTK code in `.cpp` files and only expose UE safe headers
    - See `VtkWrapper` branch

## Installation

#### Automated Installation

You can use the `[*.bat|*.sh|*.command]` installation scripts to download and build VTK for UE automatically.

By default it will do a `Release` build, but you can tell it to do a `Debug` build by starting it with `Debug` as a parameter.  
**If you do a Debug build,** don't delete your build folder (`./Temporary`) at the end of the script. The `*.dll` files store the path to its debug symbols which remain there.

```powershell 
# Release build
.\install_vtk_windows.bat

# Debug build (do not delete build folder at the end)
.\install_vtk_windows.bat debug
```

#### Manual Installation

Alternatively, download & build the VTK library yourself and copy the files to the correct locations:
- Copy the includes to `VtkPlugin/Source/ThirdParty/VtkLibrary/Public`
- Windows
  - Copy `*.lib` files to `VtkPlugin/Source/ThirdParty/VtkLibrary/[Release|Debug]/lib` folder
  - Copy `*.dll` files to `VtkPlugin/Source/ThirdParty/VtkLibrary/[Release|Debug]/bin` folder
- Linux
  - Copy `*.so` files to `VtkPlugin/Source/ThirdParty/VtkLibrary/Linux/[Release|Debug]/lib` folder
- Mac
  - Copy `*.dylib` files to `VtkPlugin/Source/ThirdParty/VtkLibrary/Mac/[Release|Debug]/lib` folder

**Prerequisites:**

- [CMake](https://cmake.org/)

## Troubleshooting

For test data have a look at the VTKData repository (files here are referenced in the [official VTK examples](https://examples.vtk.org/site/Cxx)): https://github.com/open-cv/VTKData

If there are linking errors with the VTK library, have a look in the `Source/ThirdParty/VtkLibrary` folder:
- Check if your VTK binaries are in the correct folder
- Check if your VTK includes are present
- Check if `VtkLibrary.Build.cs` correctly finds your VTK includes & binaries

If the necessary VTK binaries don't exist, they may have not been built.
Check which VTK module is missing and enable it explicitly in your build configuration for VTK.
- If you used our installation script: Add `-DVTK_MODULE_ENABLE_VTK_<module_name>=WANT` to the cmake call
- If you build yourself with the commandline: Same as above
- If you build yourself using the CMake GUI: Check the `Advanced` box, then you can search for the missing module name
