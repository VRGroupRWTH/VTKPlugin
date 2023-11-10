#pragma once

#include "CoreTypes.h"

// NOTE
// This is just am exemplary implementation, a "proof of concept" for cross-compatibility.
// I expect that better ways exist to do the stuff below.

class vtkStructuredGrid;
class vtkXMLStructuredGridReader;

class VTKWRAPPER_API FvtkMath
{
public:
    static double Distance2BetweenPoints(const double p1[3], const double p2[3]);
};

class VTKWRAPPER_API FvtkStructuredGrid
{    
public:
    FvtkStructuredGrid();
    FvtkStructuredGrid(vtkStructuredGrid* arg);
    ~FvtkStructuredGrid();
    
    int GetDataDimension();
    int GetNumberOfPoints();
    int GetNumberOfCells();
    void GetDimensions(int dims[3]);
    void GetCellDims(int cellDims[3]);

private:
    vtkStructuredGrid* mStructuredGrid;
};

class VTKWRAPPER_API FvtkXMLStructuredGridReader
{
public:
    FvtkXMLStructuredGridReader();
    ~FvtkXMLStructuredGridReader();
    
    void SetFileName(const char* arg);
    void Update();
    bool CheckAbort();
    FvtkStructuredGrid* GetOutput();

private:
    vtkXMLStructuredGridReader* mXMLStructuredGridReader;
};
