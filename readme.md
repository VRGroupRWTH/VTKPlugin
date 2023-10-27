# VTK Plugin for Unreal Engine 5+

A plugin that links the [VTK Library](https://github.com/Kitware/VTK) to [Unreal Engine 5.3](https://docs.unrealengine.com/5.3/en-US/).

Currently, this plugin does not much more but exposing VTK to Unreal and running a small test right after loading the plugin based on the [Distance between Points](https://examples.vtk.org/site/Cxx/SimpleOperations/DistanceBetweenPoints/) example.
Check the Unreal Log for "VTK Test: Distance Between Two Points".

#### Structure

Below is a summarized view on the current structure of this Plugin.

```
Plugins
|
|# This Plugin
|- VTKPlugin
   |- VTKPlugin.uplugin
   |- install_vtk.bat
   |- install_vtk.sh
   |- Source
      |
      |# The plain VTK library (includes & binaries)
      |- ThirdParty
      |  |- VTKLibrary
      |     |# VTK includes
      |     |- Public
      |     |# VTK binaries
      |     |- x64
      |        |- [Debug|Release]
      |
      |# The integration into UE
      |- VTKPlugin
```

## Installation

You can use the `[*.bat|*.sh]` installation scripts to download, build, install and link VTK to UE automatically.
These will build the complete VTK library.
Alternatively, download & build the VTK library yourself and copy the binaries to the `[Debug|Release]` folder.

**Prerequisites:**

- [CMake](https://cmake.org/)
- C++17

## Troubleshooting

If there are linking errors with the VTK library, have a look in the `Source/ThirdParty/VTKLibrary` folder:
- Check if your VTK binaries are in the correct folder
- Check if your VTK includes are present
- Check if `VTKLibrary.Build.cs` correctly finds your VTK includes & binaries
