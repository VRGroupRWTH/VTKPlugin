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

#if PLATFORM_WINDOWS
	// VTK will throw an exception when the library is delay-loaded in Windows.
	// The exception happens before this codeblock triggers and may originate in vtk.
	// TODO: fix delay-loading for windows
	
	UE_LOG(LogTemp, Error, TEXT("[VTKPlugin] VTK is currently NOT delayloaded on Windows, as this causes UE to crash (dlls are available directly at runtime)."));
#else
	LoadDLLs();
#endif

	UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] %d VTK libraries delay-loaded!"), DynamicLinkLibraryHandles.Num());
}



void FVTKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

#if PLATFORM_WINDOWS
	// VTK will throw an exception when the library is unloaded in Windows and the debugger is attached.
	// Problem is also stated here:
	// https://discourse.vtk.org/t/vtkcommoninformationkeymanager-destruction-crash-corrupt-vfptr-table/2371/3
	// This is not a bigger problem without a debugger attached,
	// but the suggested solution is no not unload VTK manually at all.
	//
	// If you know a fix or want to do it anyway, remove the platform check.
	// TODO: fix unloading for windows

	UE_LOG(LogTemp, Error, TEXT("[VTKPlugin] Not freeing dlls on Windows because dlls are not delay-loaded & this additionally generates a crash on shutdown (with debugger attached)."));
#else
	UnloadDLLs();
#endif
}

bool FVTKPluginModule::SupportsDynamicReloading()
{
#if PLATFORM_WINDOWS
	UE_LOG(LogTemp, Error, TEXT("[VTKPlugin] VTK currently does NOT support dynamic reloading on Windows, because handles can't be freed."));
	return false;
#else
	return true;
#endif
}

FString FVTKPluginModule::GetVTKBinariesDir()
{
	auto Plugin = IPluginManager::Get().FindPlugin("VTKPlugin");
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
	const FString VTKBinariesDir = GetVTKBinariesDir();
	const FString ExtFilter = GetExtensionFilter();

	FPlatformProcess::AddDllDirectory(*VTKBinariesDir);

	TArray<FString> Files;						// Files with valid dll extension
	TArray<FString> FilesAdded;					// Delay-loaded dlls
	TArray<FString> FilesSkipped = { };			// Files excluded from delay-loading
	IFileManager::Get().FindFiles(Files, *VTKBinariesDir, *ExtFilter);
	// Since the order of DLLs are unknown,
	// try all possible combinations until all are loaded or none combinations are left.
	for (auto i = 0; i < Files.Num(); i++)
	{
		for (const auto& File : Files)
		{
			// DLL already loaded?
			if (FilesAdded.Contains(File))
				continue;

			auto Filename = FPaths::GetCleanFilename(File);

			if (FilesSkipped.Contains(Filename))
			{
				UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] Skipped %s because it is not delay-loaded."), *Filename);
				continue;
			}

			// Get DLL handle
			const FString Path = FPaths::Combine(VTKBinariesDir, File);
			void* Dll  = FPlatformProcess::GetDllHandle(*Path);
			
			// If DLL handle valid => keep
			if (Dll != NULL)
			{
				DynamicLinkLibraryHandles.Add(Dll);
				DynamicLinkLibraryNames.Add(Filename);
				FilesAdded.Add(File);
				UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] Delay-loaded %d/%d dlls: %s"), DynamicLinkLibraryHandles.Num(), Files.Num() - FilesSkipped.Num(), *Filename);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] Delay-loading didn't work, retrying again"));
			}
		}
		
		// All DLLs loaded
		if (DynamicLinkLibraryHandles.Num() == (Files.Num() - FilesSkipped.Num()))
			break;
	}
}

void FVTKPluginModule::UnloadDLLs()
{
	for (auto Idx = DynamicLinkLibraryHandles.Num() - 1; Idx >= 0; Idx--)
	{
		auto& LibraryHandle = DynamicLinkLibraryHandles[Idx];
		auto& LibraryName = DynamicLinkLibraryNames[Idx];
		
		UE_LOG(LogTemp, Warning, TEXT("[VTKPlugin] Freeing dll: %s"), *LibraryName);
		FPlatformProcess::FreeDllHandle(LibraryHandle);
	}
	
	DynamicLinkLibraryHandles.Empty();
	DynamicLinkLibraryNames.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVTKPluginModule, VTKPlugin)
