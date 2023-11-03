// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FVTKPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FString GetBinariesDir();
	FString GetExtensionFilter();
	void LoadDLLs();
	void UnloadDLLs();
	
private:
	/** Array which holds handles to the loaded VTK dlls */
	TArray<void*> DynamicLinkLibraries;
};
