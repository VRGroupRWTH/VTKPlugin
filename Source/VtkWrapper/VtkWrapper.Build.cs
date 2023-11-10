// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VtkWrapper : ModuleRules
{
	public VtkWrapper(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		bUseRTTI = true;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"VtkLibrary",
			}
			);
	}
}
