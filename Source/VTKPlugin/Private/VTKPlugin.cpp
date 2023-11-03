// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTKPlugin.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FVTKPluginModule"

#if 1
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
	//return FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/VTKLibrary/Windows/x64/Debug"));
	return FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win64"));
#elif PLATFORM_LINUX
	return Plugin->GetBaseDir() / TEXT("Binaries/ThirdParty/VTKLibrary/Linux");
#elif PLATFORM_MAC
	return Plugin->GetBaseDir() / TEXT("Binaries/ThirdParty/VTKLibrary/Mac");
#endif
}

FString FVTKPluginModule::GetExtensionFilter()
{
#if PLATFORM_WINDOWS
	return "dll";
#elif PLATFORM_LINUX
	return TEXT("so");
#elif PLATFORM_MAC
	return TEXT("dylib");
#endif
}

void FVTKPluginModule::LoadDLLs()
{
	const FString VTKBinariesDir = GetBinariesDir();
	const FString ExtFilter = GetExtensionFilter();

	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *VTKBinariesDir, *ExtFilter);
	// Since the order of DLLs are unknown, try all possible combinations until all are loaded or none combinations are left.
	for (const auto& i : Files)
	{
		for (const auto& File : Files)
		{
			const FString Path = FPaths::Combine(VTKBinariesDir, File);
			auto Dll  = FPlatformProcess::GetDllHandle(*Path);
			
			if (Dll && !DynamicLinkLibraries.Contains(Dll))
				DynamicLinkLibraries.Add(Dll);
		}
		
		if (DynamicLinkLibraries.Num() == Files.Num())
			break;
	}
}

void FVTKPluginModule::UnloadDLLs()
{
	for (const auto& LibraryHandle : DynamicLinkLibraries)
	{
		// VTK will throw an exception when the library is unloaded.
		// Problem is also stated here: https://discourse.vtk.org/t/vtkcommoninformationkeymanager-destruction-crash-corrupt-vfptr-table/2371/3
		// This is not a bigger problem without a debugger attached, but the suggested solution is no not unload VTK manually at all.
		// If you know a fix or want to do it anyway, uncomment below line.
		
		//FPlatformProcess::FreeDllHandle(LibraryHandle);
	}
	
	DynamicLinkLibraries.Empty();
}

#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVTKPluginModule, VTKPlugin)
