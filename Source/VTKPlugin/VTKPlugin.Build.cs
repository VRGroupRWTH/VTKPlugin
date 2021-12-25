using UnrealBuildTool;

public class VTKPlugin : ModuleRules
{
  public VTKPlugin(ReadOnlyTargetRules target) : base(target)
  {
    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new[] {"Core", "Projects", "VTK"});
  }
}
