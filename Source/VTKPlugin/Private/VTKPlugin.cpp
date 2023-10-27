// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTKPlugin.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#include "Test.h"

#define LOCTEXT_NAMESPACE "FVTKPluginModule"

void FVTKPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	const FString BaseDir         = IPluginManager::Get().FindPlugin("VTKPlugin")->GetBaseDir();
	const FString AbsoluteBaseDir = IFileManager  ::Get().ConvertToAbsolutePathForExternalAppForRead(*BaseDir);

	// Add on the relative location of the third party dll and load it
#if PLATFORM_WINDOWS
	const FString VTKDir          = FPaths::Combine(*AbsoluteBaseDir, TEXT("/Binaries/ThirdParty/VTKLibrary/Win64/"));
	const FString Filter          = FPaths::Combine(VTKDir, TEXT("*.dll"));

	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *Filter, true, false);
	while (Files.Num() != DynamicLinkLibraries.Num()) // Since the order of DLLs are unknown, try repeatedly until all are loaded.
	{
		for (auto& File : Files)
		{
			const FString Path = FPaths::Combine(VTKDir, File);
			auto          Dll  = FPlatformProcess::GetDllHandle(*Path);
			
			if (Dll && !DynamicLinkLibraries.Contains(Dll))
				DynamicLinkLibraries.Add(Dll);
		}
	}

#elif PLATFORM_MAC
	const FString VTKDir          = FPaths::Combine(*AbsoluteBaseDir, TEXT("/Binaries/ThirdParty/VTKLibrary/Mac/"));
	const FString Filter          = FPaths::Combine(VTKDir, TEXT("*.dylib"));

	UE_LOG(LogTemp, Warning, TEXT("VTKPlugin.cpp has not been setup for MacOS yet!"))

#elif PLATFORM_LINUX
	const FString VTKDir          = FPaths::Combine(*AbsoluteBaseDir, TEXT("/Binaries/ThirdParty/VTKLibrary/Linux/x86_64-unknown-linux-gnu"));
	const FString Filter          = FPaths::Combine(VTKDir, TEXT("*.so"));

	UE_LOG(LogTemp, Warning, TEXT("VTKPlugin.cpp has not been setup for Linux yet!"))

#endif

	Test();
}

void FVTKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	for (const auto& LibraryHandle : DynamicLinkLibraries)
	{
    	FPlatformProcess::FreeDllHandle(LibraryHandle);
	}
	DynamicLinkLibraries.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVTKPluginModule, VTKPlugin)
