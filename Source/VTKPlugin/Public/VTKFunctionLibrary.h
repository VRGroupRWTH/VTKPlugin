// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VTKFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VTKPLUGIN_API UVTKFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="VTK")
	static float DistanceBetweenTwoPoints(const FVector3f& P0, const FVector3f& P1);
};
