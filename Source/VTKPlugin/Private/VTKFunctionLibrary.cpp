// Fill out your copyright notice in the Description page of Project Settings.


#include "VTKFunctionLibrary.h"
#include "vtkMath.h"

float UVTKFunctionLibrary::DistanceBetweenTwoPoints(const FVector3f& P0, const FVector3f& P1)
{
	UE_LOG(LogTemp, Warning, TEXT("VTK Example: Distance Between Two Points"));
	UE_LOG(LogTemp, Warning, TEXT("P0: (%f, %f, %f), P1: (%f, %f, %f)"),
		P0.X, P0.Y, P0.Z, P1.X, P1.Y, P1.Z);
	
	// Create two vtk compatible points.
	const double vtk_p0[3] = {P0.X, P0.Y, P0.Z};
	const double vtk_p1[3] = {P1.X, P1.Y, P1.Z};

	// Find the squared distance between the points using vtk.
	const double squaredDistance = vtkMath::Distance2BetweenPoints(vtk_p0, vtk_p1);

	// Take the square root to get the Euclidean distance between the points.
	const double distance = std::sqrt(squaredDistance);
	
	// Output the results.
	UE_LOG(LogTemp, Warning, TEXT("VTK SquaredDistance = %f"), squaredDistance);
	UE_LOG(LogTemp, Warning, TEXT("VTK Distance = %f"), distance);

	// Return the Euclidean distance.
	return distance;
}
