#include "VTKPlugin.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "vtkPolyData.h"

#define LOCTEXT_NAMESPACE "FVTKPluginModule"

void FVTKPluginModule::StartupModule ()
{
#if PLATFORM_WINDOWS
  const FString BaseDir         = IPluginManager::Get().FindPlugin("VTKPlugin")->GetBaseDir();
  const FString AbsoluteBaseDir = IFileManager  ::Get().ConvertToAbsolutePathForExternalAppForRead(*BaseDir);
  const FString VTKDir          = FPaths::Combine(*AbsoluteBaseDir, TEXT("/Binaries/ThirdParty/VTK/Win64/"));
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
#endif
}
void FVTKPluginModule::ShutdownModule()
{
  for (const auto& Library : DynamicLinkLibraries)
    FPlatformProcess::FreeDllHandle(Library);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVTKPluginModule, VTKPlugin)