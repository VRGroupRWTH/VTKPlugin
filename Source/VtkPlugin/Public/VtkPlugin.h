// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FVtkPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;

private:
	FString GetVTKBinariesDir();
	FString GetExtensionFilter();
	void LoadDLLs();
	void UnloadDLLs();
	
private:
	/** Array which holds handles to the loaded VTK dlls */
	TArray<void*> DynamicLinkLibraryHandles;
	TArray<FString> DynamicLinkLibraryNames;
};
