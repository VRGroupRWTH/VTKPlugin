#pragma once

#include "Modules/ModuleManager.h"
#include "Runtime/Core/Public/Containers/Array.h"

class FVTKPluginModule : public IModuleInterface
{
public:
  virtual void StartupModule () override;
  virtual void ShutdownModule() override;

private:
  TArray<void*> DynamicLinkLibraries;
};