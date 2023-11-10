// Fill out your copyright notice in the Description page of Project Settings.


#include "VtkFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START
// DistanceBetweenTwoPoins example
#include "vtkMath.h"

#if PLATFORM_WINDOWS
// StructuredGridReader example
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkStructuredGrid.h"
#endif
THIRD_PARTY_INCLUDES_END

float UVtkFunctionLibrary::DistanceBetweenTwoPoints(const FVector3f& P0, const FVector3f& P1)
{
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] VTK Example: Distance Between Two Points"));
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] P0: (%f, %f, %f), P1: (%f, %f, %f)"),
		P0.X, P0.Y, P0.Z, P1.X, P1.Y, P1.Z);
	
	// Create two vtk compatible points.
	const double vtk_p0[3] = {P0.X, P0.Y, P0.Z};
	const double vtk_p1[3] = {P1.X, P1.Y, P1.Z};

	// Find the squared distance between the points using vtk.
	const double squaredDistance = vtkMath::Distance2BetweenPoints(vtk_p0, vtk_p1);

	// Take the square root to get the Euclidean distance between the points.
	const double distance = std::sqrt(squaredDistance);
	
	// Output the results.
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] SquaredDistance = %f"), squaredDistance);
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Distance = %f"), distance);

	// Return the Euclidean distance.
	return distance;
}

bool UVtkFunctionLibrary::ReadStructuredGridTest(const FString& Filepath)
{
#if PLATFORM_WINDOWS
	const FString CleanFilepath = Filepath.TrimQuotes();
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] VTK Example: Read Structured Grid"));
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Reading structured grid: %s"), *CleanFilepath);
	
	if (!FPaths::FileExists(CleanFilepath))
	{
		UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] File does not exist: %s"), *CleanFilepath);
		return false;
	}
	
	// Read the file.
	vtkNew<vtkXMLStructuredGridReader> reader;
	reader->SetFileName(TCHAR_TO_ANSI(*CleanFilepath));
	reader->Update();

	if (reader->CheckAbort())
	{
		UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] VTK encountered a problem"));
		return false;
	}

	auto structuredGrid = reader->GetOutput();
	int dataDim = structuredGrid->GetDataDimension();
	int numPoints = structuredGrid->GetNumberOfPoints();
	int numCells = structuredGrid->GetNumberOfCells();
	int gridDims[3];
	int cellDims[3];
	structuredGrid->GetDimensions(gridDims);
	structuredGrid->GetCellDims(cellDims);

	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Data dimension: %d"), dataDim);
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Grid Extents: (%d, %d, %d)"), gridDims[0], gridDims[1], gridDims[2]);
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Cell Extents: (%d, %d, %d)"), cellDims[0], cellDims[1], cellDims[2]);
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Number Points: %d"), numPoints);
	UE_LOG(LogTemp, Warning, TEXT("[VtkPlugin] Data Cells: %d"), numCells);
	
	return true;
#else
	UE_LOG(LogTemp, Error, TEXT("[VtkPlugin] The ReadStructuredGridTest example is not yet implemented on Unix-based systems because of missing RTTI support."));
	return false;
#endif
}
