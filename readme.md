# VTK Plugin for Unreal Engine 5+

A plugin that links the [VTK Library](https://github.com/Kitware/VTK) to [Unreal Engine 5.3](https://docs.unrealengine.com/5.3/en-US/).

Currently, this plugin does not much more but exposing VTK to Unreal (publicly which may not be so smart), implements a small Blueprint function for testing and logs how many VTK Plugins it has loaded.
The included test function is the official [Distance between Points](https://examples.vtk.org/site/Cxx/SimpleOperations/DistanceBetweenPoints/) example.
Thre result of this test is printed in the Unreal Log as `Warning`.

## Installation

**Note:** Only the Windows build is currently tested. The configuration may not be functional for Linux or Mac (or any other OS).

You can use the `[*.bat|*.sh|*.command]` installation scripts to download, build, install and link VTK to UE automatically.
By default it will do a `Release` build, but you can tell it to do a `Debug` build by starting it with `Debug` as a parameter.  

Alternatively, download & build the VTK library yourself and copy the `*.dll|*.so|*.dylib` files to the `VTKPlugin/Binaries/ThirdParty/[Win64|Linux|Mac]` folder, `*.lib` files to the `VTKPlugin/Source/ThirdParty/VTKLibrary/[Windows|Linx|Mac]/x64` folder along with the necessary includes to `VTKPlugin/Source/ThirdParty/VTKLibrary/Public`.

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

If you are using Linux or Mac the scripts or configuration may not be fully functional.
Please consider a pull request if this is the case and you fix it. :-)

If there are linking errors with the VTK library, have a look in the `Source/ThirdParty/VTKLibrary` folder:
- Check if your VTK binaries are in the correct folder
- Check if your VTK includes are present
- Check if `VTKLibrary.Build.cs` correctly finds your VTK includes & binaries

If the necessary VTK binaries don't exist, they may have not been built.
Check which VTK module is missing and enable it explicitly in your build configuration for VTK.
- If you used our installation script: Add `-DVTK_MODULE_ENABLE_VTK_<module_name>=YES` to the cmake call
- If you build yourself with the commandline: Same as above
- If you build yourself using the CMake GUI: Check the `Advanced` box, then you can search for the missing module name
