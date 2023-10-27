// Fill out your copyright notice in the Description page of Project Settings.

#include "Test.h"
#include <vtkMath.h>

Test::Test()
{
	DistanceBetweenTwoPoints();
	IOLegacy();
}

Test::~Test()
{

}

void Test::DistanceBetweenTwoPoints()
{
	UE_LOG(LogTemp, Warning, TEXT("VTK Test: Distance Between Two Points"));
	// Create two points.
	double p0[3] = {0.0, 0.0, 0.0};
	double p1[3] = {1.0, 1.0, 1.0};

	// Find the squared distance between the points.
	double squaredDistance = vtkMath::Distance2BetweenPoints(p0, p1);

	// Take the square root to get the Euclidean distance between the points.
	double distance = std::sqrt(squaredDistance);
	
	// Output the results.
	UE_LOG(LogTemp, Warning, TEXT("SquaredDistance = %f"), squaredDistance);
	UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), distance);
}

void Test::IOLegacy()
{
	UE_LOG(LogTemp, Warning, TEXT("VTK Test: IOLegacy (not implemented yet)"));
}
