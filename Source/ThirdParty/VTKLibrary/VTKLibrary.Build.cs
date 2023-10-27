// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class VTKLibrary : ModuleRules
{
	public VTKLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		// Infer the correct folder to load the DLLs/Libs from
		string BuildFolder = "";
		switch (Target.Configuration)
		{
			case UnrealTargetConfiguration.Debug:
			case UnrealTargetConfiguration.DebugGame:
				BuildFolder = "Debug";
				break;
			case UnrealTargetConfiguration.Development:
				//BuildFolder = "RelWithDebInfo";
				BuildFolder = "Debug";
				break;
			case UnrealTargetConfiguration.Shipping:
				BuildFolder = "Release";
				break;
			default:
				BuildFolder = "Release";
				break;
		}
		
		// Load Library
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Libs
			foreach (var value in Directory.EnumerateFiles(Path.Combine(ModuleDirectory, "x64", BuildFolder), "*.lib"  , SearchOption.AllDirectories))
			{
				PublicAdditionalLibraries.Add(value);
			}

			// DLLs
			foreach (var value in Directory.EnumerateFiles(Path.Combine(ModuleDirectory, "x64", BuildFolder), "*.dll"  , SearchOption.AllDirectories))
			{
				PublicDelayLoadDLLs.Add(value.Substring(value.LastIndexOf('\\') + 1));
				RuntimeDependencies.Add(value);
			}
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// Libs & DLLs
			// ToDo: Test & fix path (e.g. x86_64-unknown-linux-gnu)
			foreach (var value in Directory.EnumerateFiles(Path.Combine(ModuleDirectory, "Linux", "x86_64-unknown-linux-gnu"), "*.so"  , SearchOption.AllDirectories))
			{
				PublicAdditionalLibraries.Add(value);
				PublicDelayLoadDLLs.Add(value);
				RuntimeDependencies.Add(value);
			}
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			// DyLibs
			// ToDo: Test
			//throw new Exception()
			foreach (var value in Directory.EnumerateFiles(Path.Combine(ModuleDirectory, "Mac", BuildFolder), "*.dylib"  , SearchOption.AllDirectories))
			{
				PublicDelayLoadDLLs.Add(value);
				RuntimeDependencies.Add(value);
			}
		}
	}
}
