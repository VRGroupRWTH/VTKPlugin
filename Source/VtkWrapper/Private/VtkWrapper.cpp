#include "VtkWrapper.h"

#include "Modules/ModuleManager.h"

PRAGMA_DEFAULT_VISIBILITY_START
THIRD_PARTY_INCLUDES_START
// DistanceBetweenTwoPoins example
#include "vtkMath.h"

// StructuredGridReader example
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkPolyDataMapper.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkStructuredGrid.h"
THIRD_PARTY_INCLUDES_END
PRAGMA_DEFAULT_VISIBILITY_END

// NOTE
// This is just am exemplary implementation, a "proof of concept" for cross-compatibility.
// I expect that better ways exist to do the stuff below.

// FvtkMath

double FvtkMath::Distance2BetweenPoints(const double p1[3], const double p2[3])
{
    return vtkMath::Distance2BetweenPoints(p1, p2);
}

// FvtkStructuredGrid

FvtkStructuredGrid::FvtkStructuredGrid()
{
    mStructuredGrid = vtkStructuredGrid::New();
}

FvtkStructuredGrid::FvtkStructuredGrid(vtkStructuredGrid* arg)
{
    mStructuredGrid = arg;
}

FvtkStructuredGrid::~FvtkStructuredGrid()
{
    mStructuredGrid->Delete();
}

int FvtkStructuredGrid::GetDataDimension()
{
    return ((vtkStructuredGrid*)mStructuredGrid)->GetDataDimension();
}
int FvtkStructuredGrid::GetNumberOfPoints()
{
    return ((vtkStructuredGrid*)mStructuredGrid)->GetNumberOfPoints();
}
int FvtkStructuredGrid::GetNumberOfCells()
{
    return ((vtkStructuredGrid*)mStructuredGrid)->GetNumberOfCells();
}
void FvtkStructuredGrid::GetDimensions(int dims[3])
{
    ((vtkStructuredGrid*)mStructuredGrid)->GetDimensions(dims);
}
void FvtkStructuredGrid::GetCellDims(int cellDims[3])
{
    ((vtkStructuredGrid*)mStructuredGrid)->GetCellDims(cellDims);
}

// FvtkXMLStructuredGridReader

FvtkXMLStructuredGridReader::FvtkXMLStructuredGridReader()
{
    mXMLStructuredGridReader = vtkXMLStructuredGridReader::New();
}

FvtkXMLStructuredGridReader::~FvtkXMLStructuredGridReader()
{
    mXMLStructuredGridReader->Delete();
}

void FvtkXMLStructuredGridReader::SetFileName(const char* arg)
{
    ((vtkXMLStructuredGridReader*)mXMLStructuredGridReader)->SetFileName(arg);
}
void FvtkXMLStructuredGridReader::Update()
{
    ((vtkXMLStructuredGridReader*)mXMLStructuredGridReader)->Update();
}
bool FvtkXMLStructuredGridReader::CheckAbort()
{
    return ((vtkXMLStructuredGridReader*)mXMLStructuredGridReader)->CheckAbort();
}

FvtkStructuredGrid* FvtkXMLStructuredGridReader::GetOutput()
{
    auto out = ((vtkXMLStructuredGridReader*)mXMLStructuredGridReader)->GetOutput();
    return new FvtkStructuredGrid(out);
}

IMPLEMENT_MODULE(FDefaultModuleImpl, VtkWrapper);
//IMPLEMENT_MODULE(FVtkWrapperModule, VtkWrapper);