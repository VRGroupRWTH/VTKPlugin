// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class VtkLibrary : ModuleRules
{
	public VtkLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		
		// If you want to load a debug build, specify here the Debug directory
		var BuildDir = "Release";
		
		// Load includes
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");
		
		// Load binaries
		if (Target.IsInPlatformGroup(UnrealPlatformGroup.Windows))
		{
			var libSearchPath = Path.Combine(ModuleDirectory, BuildDir, "lib");
			var dllSearchPath = Path.Combine(ModuleDirectory, BuildDir, "bin");

			if (!Directory.Exists(libSearchPath) || !Directory.Exists(dllSearchPath))
				Console.WriteLine("Did not find lib- or dll-path for VtkLibrary!");
			
			// Libs
			foreach (var libPath in Directory.EnumerateFiles(libSearchPath, "*.lib"  , SearchOption.AllDirectories))
			{
				PublicAdditionalLibraries.Add(libPath);
			}

			// DLLs
			foreach (var filePath in Directory.EnumerateFiles(dllSearchPath, "*.dll"  , SearchOption.AllDirectories))
			{
				var dllName = Path.GetFileName(filePath);
				var srcPath = Path.Combine("$(ModuleDir)", BuildDir, "bin", dllName);
				var destPath = Path.Combine("$(BinaryOutputDir)", dllName);
				
				// TODO: Investigate why we can't delayload VTK like this on Windows, uncomment if fix is found
				// This prevents the dlls to be auto-loaded when the plugin is read
				// and allows us to load them later in VtkPlugin.cpp during StartupModule.
				// That allows us to load dlls from specific paths, which is not (easily) possible otherwise on Windows.
				//PublicDelayLoadDLLs.Add(dllName);
				//RuntimeDependencies.Add(srcPath);

				// This adds a runtime dependency & copies the dll from source path to destination path on build
				RuntimeDependencies.Add(destPath, srcPath);

			}
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			var dylibSearchPath = Path.Combine(ModuleDirectory, "Mac", BuildDir, "lib");

			if (!Directory.Exists(dylibSearchPath))
				Console.WriteLine("Did not find dylib-path for VtkLibrary!");

			// DyLibs
			foreach (var filePath in Directory.EnumerateFiles(dylibSearchPath, "*.dylib"  , SearchOption.AllDirectories))
			{
				var dylibName = Path.GetFileName(filePath);
				var dylibPath = Path.Combine("$(ModuleDir)", "Mac", BuildDir, "lib", dylibName);

				PublicDelayLoadDLLs.Add(dylibPath);
				RuntimeDependencies.Add(dylibPath);
			}
		}
		else if (Target.IsInPlatformGroup(UnrealPlatformGroup.Unix))
		{
			var soSearchPath = Path.Combine(ModuleDirectory, "Linux", BuildDir, "lib");

			if (!Directory.Exists(soSearchPath))
				Console.WriteLine("Did not find so-path for VtkLibrary!");
			
			// SOs
			foreach (var filePath in Directory.EnumerateFiles(soSearchPath, "*.so"  , SearchOption.AllDirectories))
			{
				var soName = Path.GetFileName(filePath);
				var soPath = Path.Combine("$(ModuleDir)", "Linux", BuildDir, "lib", soName);

				PublicAdditionalLibraries.Add(soPath);
				PublicDelayLoadDLLs.Add(soPath);
				RuntimeDependencies.Add(soPath);
			}
		}
	}
}
