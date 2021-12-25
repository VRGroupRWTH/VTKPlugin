using System.IO;
using UnrealBuildTool;

public class VTK : ModuleRules
{
  public VTK(ReadOnlyTargetRules target) : base(target)
  {
    Type = ModuleType.External;
    
    PublicIncludePaths.Add(Path.Combine(PluginDirectory, "Source", "ThirdParty", "VTK", "Public"));

    if      (target.Platform == UnrealTargetPlatform.Win64)
    {
      foreach (var value in Directory.EnumerateFiles(Path.Combine(PluginDirectory, "Intermediate", "ThirdParty", "VTK", "Win64"), "*.lib"  , SearchOption.AllDirectories))
      {
        PublicAdditionalLibraries.Add(value);
      }
      foreach (var value in Directory.EnumerateFiles(Path.Combine(PluginDirectory, "Binaries"    , "ThirdParty", "VTK", "Win64"), "*.dll"  , SearchOption.AllDirectories))
      {
        PublicDelayLoadDLLs      .Add(value);
        RuntimeDependencies      .Add(value);
      }
    }
    else if (target.Platform == UnrealTargetPlatform.Mac  )
    {
      foreach (var value in Directory.EnumerateFiles(Path.Combine(PluginDirectory, "Intermediate", "ThirdParty", "VTK", "Mac"  ), "*.dylib", SearchOption.AllDirectories))
      {
        PublicDelayLoadDLLs      .Add(value);
        RuntimeDependencies      .Add(value);
      }
    }
    else if (target.Platform == UnrealTargetPlatform.Linux)
    {
      foreach (var value in Directory.EnumerateFiles(Path.Combine(PluginDirectory, "Intermediate", "ThirdParty", "VTK", "Linux"), "*.so"   , SearchOption.AllDirectories))
      {
        PublicAdditionalLibraries.Add(value);
        PublicDelayLoadDLLs      .Add(value);
        RuntimeDependencies      .Add(value);
      }
    }
  }
}
