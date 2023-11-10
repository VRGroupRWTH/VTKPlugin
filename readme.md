## VTK Plugin for Unreal Engine 5+

**This branch is fully cross-compatible between Windows and Unix (Linux/Mac).** See "How" in the [Remarks on Linux/Mac](#remarks-on-linuxmac) section.

A plugin that links the [VTK Library](https://github.com/Kitware/VTK) to [Unreal Engine 5.3](https://docs.unrealengine.com/5.3/en-US/).

Currently, this plugin does the following:
- Download & build VTK (infer/change the version in the install script file)
- Expose VTK to your complete Unreal project (public, but you may want this private)
- Provide a blueprint function "DistanceBetweenTwoPoints" for testing `vtkMath.h` (based on an [official VTK example](https://examples.vtk.org/site/Cxx/SimpleOperations/DistanceBetweenPoints/), cross-compatible)
- Provide a blueprint function "ReadStructuredGridTest" for more complex vtk includes (loosely based on an [official VTK example](https://examples.vtk.org/site/Cxx/IO/ReadStructuredGrid/#download-and-build-readstructuredgrid)
- ✔️ Complete cross-compatibility for Windows/Linux/Mac (tested)

This plugin is quite verbose as it aims to be a foundation for implementing & testing VTK functionality in UE.
Check the Unreal Log for `[VtkPlugin]` to see what's happening (also valid for the blueprint functions).

### ✔️ Remarks on Windows

Delay-loading (which would be better practice) is currently disabled for Windows and all VTK dlls are instantly loaded.
This prevents module reloading.

### ✔️ Remarks on Linux/Mac

Delay-loading is enabled on Unix-based systems and thus also module reloading (theoretically).

To achieve cross-compatibility of all VTK methods, the `VtkWrapper` module with [RTTI](https://en.wikipedia.org/wiki/Run-time_type_information) enabled was implemented to wrap all VTK-native code.
Each VTK functionality needs to be wrapped explicitly before it is exposed (e.g., to `VtkPlugin` which implements the blueprints).  
This is needed, as contrary to Windows Unreal Engine on Unix is by default compiled without RTTI support (thoroughly discussed [here](https://forums.unrealengine.com/t/rtti-failed-compiling-when-enabled-for-4-23-linux/455083/22)).
Globally enabling RTTI on Unix is not (easily) done, so this approach orients the way the `OpenExrWrapper` is implemented as suggested [here](https://forums.unrealengine.com/t/rtti-failed-compiling-when-enabled-for-4-23-linux/455083/13).

⚠️ The provided `VtkWrapper` implementation is just an example and better solutions exist.

## Installation

#### Automated Installation

You can use the `[*.bat|*.sh|*.command]` installation scripts to download and build VTK for UE automatically.

By default it will do a `Release` build, but you can tell it to do a `Debug` build by starting it with `Debug` as a parameter.  
**If you do a Debug build,** be aware to change the respective paths in your `VtkLibrary.Build.cs` and `VtkPlugin.cpp` to `Debug` as well.

```powershell 
# Release build
.\install_vtk_windows.bat

# Debug build
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

If you are using Linux or Mac the scripts or configuration may not be fully functional.
Please consider a pull request if this is the case and you fix it. :-)

If there are linking errors with the VTK library, have a look in the `Source/ThirdParty/VtkLibrary` folder:
- Check if your VTK binaries are in the correct folder
- Check if your VTK includes are present
- Check if `VtkLibrary.Build.cs` correctly finds your VTK includes & binaries

If the necessary VTK binaries don't exist, they may have not been built.
Check which VTK module is missing and enable it explicitly in your build configuration for VTK.
- If you used our installation script: Add `-DVTK_MODULE_ENABLE_VTK_<module_name>=WANT` to the cmake call
- If you build yourself with the commandline: Same as above
- If you build yourself using the CMake GUI: Check the `Advanced` box, then you can search for the missing module name
