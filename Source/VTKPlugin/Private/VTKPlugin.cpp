// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTKPlugin.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FVTKPluginModule"

void FVTKPluginModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading VTK module..."));

	LoadDLLs();

	UE_LOG(LogTemp, Warning, TEXT("%d VTK libraries loaded!"), DynamicLinkLibraries.Num());
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
	//BaseDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*BaseDir);

#if PLATFORM_WINDOWS
	return FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win64"));
#elif PLATFORM_LINUX
	return FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Linux"));
#elif PLATFORM_MAC
	return FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Mac"));
#endif
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
			
			const FString Path = FPaths::Combine(VTKBinariesDir, File);
			auto Dll  = FPlatformProcess::GetDllHandle(*Path);
			
			// If DLL valid => load
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

#if !PLATFORM_WINDOWS
		FPlatformProcess::FreeDllHandle(LibraryHandle);
#endif
	}
	
	DynamicLinkLibraries.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVTKPluginModule, VTKPlugin)
