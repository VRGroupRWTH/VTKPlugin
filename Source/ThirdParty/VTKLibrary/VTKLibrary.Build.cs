// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class VTKLibrary : ModuleRules
{
	public VTKLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		
		// Load includes
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");
		
		// Load binaries
		if (Target.IsInPlatformGroup(UnrealPlatformGroup.Windows))
		{
			// If you want to load a debug build, change the build directory here
			var libSearchPath = Path.Combine(ModuleDirectory, "Windows", "x64", "Debug");
			//var dllSearchPath = Path.Combine(ModuleDirectory, "Windows", "x64", "Debug");
			var dllSearchPath = Path.Combine(PluginDirectory, "Binaries", "ThirdParty", "Win64");

			if (!Directory.Exists(libSearchPath) || !Directory.Exists(dllSearchPath))
				Console.WriteLine("Did not find lib- or dll-path for VTKLibrary!");
			
			// Libs
			foreach (var libPath in Directory.EnumerateFiles(libSearchPath, "*.lib"  , SearchOption.AllDirectories))
			{
				PublicAdditionalLibraries.Add(libPath);
			}

			// DLLs
			foreach (var dllPath in Directory.EnumerateFiles(dllSearchPath, "*.dll"  , SearchOption.AllDirectories))
			{
				var dllName = dllPath.Substring(dllPath.LastIndexOf('\\') + 1);
				PublicDelayLoadDLLs.Add(dllName);
				RuntimeDependencies.Add(dllPath);
			}
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string dylibSearchPath = Path.Combine(PluginDirectory, "Binaries", "ThirdParty", "Mac");
			
			if (!Directory.Exists(dylibSearchPath))
				Console.WriteLine("Did not find dylib-path for VTKLibrary!");

			// DyLibs
			foreach (var dylibPath in Directory.EnumerateFiles(dylibSearchPath, "*.dylib"  , SearchOption.AllDirectories))
			{
				PublicDelayLoadDLLs.Add(dylibPath);
				RuntimeDependencies.Add(dylibPath);
			}
		}
		else if (Target.IsInPlatformGroup(UnrealPlatformGroup.Unix))
		{
			var soSearchPath = Path.Combine(PluginDirectory, "Binaries", "ThirdParty", "Linux");
			
			if (!Directory.Exists(soSearchPath))
				Console.WriteLine("Did not find so-path for VTKLibrary!");
			
			// SOs
			foreach (var soPath in Directory.EnumerateFiles(soSearchPath, "*.so"  , SearchOption.AllDirectories))
			{
				PublicAdditionalLibraries.Add(soPath);
				PublicDelayLoadDLLs.Add(soPath);
				RuntimeDependencies.Add(soPath);
			}
		}
	}
}
