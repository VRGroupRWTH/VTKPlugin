// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VtkPlugin : ModuleRules
{
	public VtkPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"VtkLibrary",
				"Projects",
				// ... add other public dependencies that you statically link with here ...
				
				// Official VTK should better be a private module dependency
				// to only expose curated VTK functionality & improve reusability.
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
				"CoreUObject",
				"Engine",
				
				// (as said above, the VtkLibrary module should better be defined here)
				//"VtkLibrary",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
