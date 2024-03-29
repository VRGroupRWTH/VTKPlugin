// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VtkFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VTKPLUGIN_API UVtkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="VTK")
	static float DistanceBetweenTwoPoints(const FVector3f& P0, const FVector3f& P1);

	UFUNCTION(BlueprintCallable, Category="VTK")
	static bool ReadStructuredGridTest(const FString& Filepath);
};
