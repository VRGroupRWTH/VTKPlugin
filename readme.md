## VTK Plugin for Unreal Engine 5+

A plugin that links the [VTK Library](https://github.com/Kitware/VTK) to [Unreal Engine 5.3](https://docs.unrealengine.com/5.3/en-US/).

Currently, this plugin does the following:
- Download & build VTK (infer/change the version in the install script file)
- Expose VTK to your complete Unreal project (public)
- Provide a blueprint function "DistanceBetweenTwoPoints" for testing `vtkMath.h` (based on an [official VTK example](https://examples.vtk.org/site/Cxx/SimpleOperations/DistanceBetweenPoints/))
- Provide a blueprint function "ReadStructuredGridTest" for testing multiple vtk includes (loosely based on an [official VTK example](https://examples.vtk.org/site/Cxx/IO/ReadStructuredGrid/#download-and-build-readstructuredgrid))

This plugin is quite verbose as it aims to be a foundation for implementing & testing VTK functionality in UE.
Check the Unreal Log for `[VTKPlugin]` to see what's happening (also valid for the blueprint functions).

### Remarks on Windows

Delay-loading is currently disabled for Windows and all VTK dlls are copied from `VTKPlugin/Source/ThirdParty/VTKLibrary/bin` to `VTKPlugin/Binaries/Win64` on build.
This way they lie next to the `*-VTKPlugin.dll` files and are loaded instantly from Windows dll standard searchpaths when the `*-VTKPlugin.dll` files (i.e. the VTKPlugin itself) are loaded.
Why? Despite delay-loading, the `*-VTKPlugin.dll` files try to include `vtkSys-9.3.dll` and `vtkCommonCore-9.3.dll` which UE can't find on non-standard search paths.

I'm looking into having a more nice solution for Windows, but this seems to work quite stable as of now.
If you try to setup delay-loading yourself, be aware of your `PATH` variable. 
E.g., if you have VTK installed on your system your `PATH` variable probably points to the location of your installed VTK `*.dlls` (not the one in your UE project).
The `PATH` variable is part of the standard search paths, so UE will find all requested VTK `*.dlls` (despite they may not be the intended ones).

### Remarks on Linux/Mac

This plugin aims to be cross-compatible, but there may be broken commits for Linux/Mac as main development is done on Windows.
Once this plugin receives a stable cross-compatible state, this section is removed.

## Installation

You can use the `[*.bat|*.sh|*.command]` installation scripts to download and build VTK for UE automatically.
By default it will do a `Release` build, but you can tell it to do a `Debug` build by starting it with `Debug` as a parameter. **If you do a Debug build,** be aware to change the respective paths in your `VTKLibrary.Build.cs` and `VTKPlugin.cpp` to `Debug` as well.

Alternatively, download & build the VTK library yourself and copy the files to the correct locations:
- Copy the includes to `VTKPlugin/Source/ThirdParty/VTKLibrary/Public`
- Windows
  - Copy `*.lib` files to `VTKPlugin/Source/ThirdParty/VTKLibrary/[Release|Debug]/lib` folder
  - Copy `*.dll` files to `VTKPlugin/Source/ThirdParty/VTKLibrary/[Release|Debug]/bin` folder
- Linux
  - Copy `*.so` files to `VTKPlugin/Source/ThirdParty/VTKLibrary/Linux/[Release|Debug]/bin` folder
- Mac
  - Copy `*.dylib` files to `VTKPlugin/Source/ThirdParty/VTKLibrary/Mac/[Release|Debug]/bin` folder

```powershell 
# Release build
.\install_vtk_windows.bat

# Debug build
.\install_vtk_windows.bat debug
```

**Prerequisites:**

- [CMake](https://cmake.org/)
- C++17

## Troubleshooting

For test data have a look at the VTKData repository (files here are referenced in the [official VTK examples](https://examples.vtk.org/site/Cxx)): https://github.com/open-cv/VTKData

If you are using Linux or Mac the scripts or configuration may not be fully functional.
Please consider a pull request if this is the case and you fix it. :-)

If there are linking errors with the VTK library, have a look in the `Source/ThirdParty/VTKLibrary` folder:
- Check if your VTK binaries are in the correct folder
- Check if your VTK includes are present
- Check if `VTKLibrary.Build.cs` correctly finds your VTK includes & binaries

If the necessary VTK binaries don't exist, they may have not been built.
Check which VTK module is missing and enable it explicitly in your build configuration for VTK.
- If you used our installation script: Add `-DVTK_MODULE_ENABLE_VTK_<module_name>=WANT` to the cmake call
- If you build yourself with the commandline: Same as above
- If you build yourself using the CMake GUI: Check the `Advanced` box, then you can search for the missing module name
