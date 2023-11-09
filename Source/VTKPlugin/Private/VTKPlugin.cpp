// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTKPlugin.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FVTKPluginModule"

void FVTKPluginModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] Delay-loading VTK module..."));

	UE_LOG(LogTemp, Error, TEXT("[VTKPlugin] VTK is currently NOT delayloaded (see VTKLibrary.Build.cs), as this causes UE to crash. Meaning, all VTK libraries are already loaded."));
	//LoadDLLs();

	UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] %d VTK libraries delay-loaded!"), DynamicLinkLibraries.Num());
}



void FVTKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handles
	UnloadDLLs();
}

FString FVTKPluginModule::GetBinariesDir()
{
	auto Plugin = IPluginManager::Get().FindPlugin("VTKPlugin");
	check(Plugin.IsValid());
	
	FString BaseDir = Plugin->GetBaseDir();

	// If you want to load a debug or release build based on UE build, specify here
#if UE_BUILD_SHIPPING ||UE_BUILD_SHIPPING_WITH_EDITOR
	FString BuildDir = "Release";
#else
	FString BuildDir = "Release";
#endif
	
	
#if PLATFORM_WINDOWS
	//FString BinariesDir = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VTKLibrary/Win64"));
	FString BinariesDir = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/VTKLibrary"), *BuildDir, "bin");
#elif PLATFORM_LINUX
	//FString BinariesDir = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VTKLibrary/Linux"));
	FString BinariesDir = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/VTKLibrary/Linux"), *BuildDir, "bin");
#elif PLATFORM_MAC
	//FString BinariesDir = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VTKLibrary/Mac"));
	FString BinariesDir = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/VTKLibrary/Mac"), *BuildDir, "bin");
#endif

	if (!FPaths::DirectoryExists(BinariesDir))
	{
		UE_LOG(LogTemp, Error, TEXT("[VTKPlugin] VTK Binaries directory does not exist: %s"), *BinariesDir);
	}

	return BinariesDir;
}

FString FVTKPluginModule::GetExtensionFilter()
{
#if PLATFORM_WINDOWS
	return "dll";
#elif PLATFORM_LINUX
	return "so";
#elif PLATFORM_MAC
	return "dylib";
#endif
}

void FVTKPluginModule::LoadDLLs()
{
	const FString VTKBinariesDir = GetBinariesDir();
	const FString ExtFilter = GetExtensionFilter();

	TArray<FString> Files;
	TArray<FString> FilesAdded;
	IFileManager::Get().FindFiles(Files, *VTKBinariesDir, *ExtFilter);
	// Since the order of DLLs are unknown,
	// try all possible combinations until all are loaded or none combinations are left.
	for (const auto& i : Files)
	{
		for (const auto& File : Files)
		{
			// DLL already loaded?
			if (FilesAdded.Contains(File))
				continue;

			// Get DLL handle
			const FString Path = FPaths::Combine(VTKBinariesDir, File);
			void* Dll  = FPlatformProcess::GetDllHandle(*Path);
			
			// If DLL handle valid => keep
			if (Dll)
			{
				DynamicLinkLibraries.Add(Dll);
				FilesAdded.Add(File);
			}
		}
		
		// All DLLs loaded
		if (DynamicLinkLibraries.Num() == Files.Num())
			break;
	}
}

void FVTKPluginModule::UnloadDLLs()
{
	for (const auto& LibraryHandle : DynamicLinkLibraries)
	{
		// VTK will throw an exception when the library is unloaded in Windows.
		// Problem is also stated here:
		// https://discourse.vtk.org/t/vtkcommoninformationkeymanager-destruction-crash-corrupt-vfptr-table/2371/3
		// This is not a bigger problem without a debugger attached,
		// but the suggested solution is no not unload VTK manually at all.
		//
		// If you know a fix or want to do it anyway, remove the platform check.
		// TODO: fix unloading for windows

#if !PLATFORM_WINDOWS
		FPlatformProcess::FreeDllHandle(LibraryHandle);
#endif
	}
	
	DynamicLinkLibraries.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVTKPluginModule, VTKPlugin)
