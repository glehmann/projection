#include "itkImageFileReader.h"

#include "itkMaximumProjectionImageFilter.h"
#include "itkMinimumProjectionImageFilter.h"
#include "itkSumProjectionImageFilter.h"
#include "itkMeanProjectionImageFilter.h"
#include "itkMedianProjectionImageFilter.h"
#include "itkSigmaProjectionImageFilter.h"
#include "itkBinaryProjectionImageFilter.h"
#include "itkAccumulateImageFilter.h"

#include "itkTimeProbe.h"
#include <vector>
#include "itkMultiThreader.h"
#include <iomanip>

int main(int, char * argv[])
{
  const int dim = 3;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  typedef unsigned short LPType;
  typedef itk::Image< LPType, dim > LIType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::MaximumProjectionImageFilter< IType, LIType > MaxType;
  MaxType::Pointer max = MaxType::New();
  max->SetInput( reader->GetOutput() );
  
  reader->Update();
  
  
  std::cout << "#th" << "\t" 
            << "max" << "\t" 
            << std::endl;

  const int rep1 = 50;
  const int rep2 = 100;
  const int maxNbOfThreads = 10;

  std::vector< itk::TimeProbe > timeProbes;
  timeProbes.resize( maxNbOfThreads );

  for( int r1=0; r1<=rep1; r1++ )
    {

    for( int th=0; th<maxNbOfThreads; th++ )
      {
      max->SetNumberOfThreads( th+1 );

      timeProbes[th].Start();
      for( int r2=0; r2<rep2; r2++ )
        {
        max->Update();
        max->Modified();
        }
      timeProbes[th].Stop();

      }
    }

  for( int th=0; th<maxNbOfThreads; th++ )
    {
    std::cout << std::setprecision(3)
              << th+1 << "\t" 
              << timeProbes[th].GetMeanTime() / rep2 << "\t" 
              <<std::endl;
    }
  
  return 0;
}

