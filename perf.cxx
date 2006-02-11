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
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);

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
  
  typedef itk::MinimumProjectionImageFilter< IType, LIType > MinType;
  MinType::Pointer min = MinType::New();
  min->SetInput( reader->GetOutput() );
  
  typedef itk::SumProjectionImageFilter< IType, LIType > SumType;
  SumType::Pointer sum = SumType::New();
  sum->SetInput( reader->GetOutput() );
  
  typedef itk::MeanProjectionImageFilter< IType, LIType > MeanType;
  MeanType::Pointer mean = MeanType::New();
  mean->SetInput( reader->GetOutput() );
  
  typedef itk::MedianProjectionImageFilter< IType, LIType > MedianType;
  MedianType::Pointer median = MedianType::New();
  median->SetInput( reader->GetOutput() );
  
  typedef itk::SigmaProjectionImageFilter< IType, LIType > SigmaType;
  SigmaType::Pointer sigma = SigmaType::New();
  sigma->SetInput( reader->GetOutput() );
  
  typedef itk::BinaryProjectionImageFilter< IType, LIType > BinaryType;
  BinaryType::Pointer binary = BinaryType::New();
  binary->SetInput( reader->GetOutput() );
  
  typedef itk::AccumulateImageFilter< IType, LIType > AccType;
  AccType::Pointer accSum = AccType::New();
  accSum->SetInput( reader->GetOutput() );
  
  AccType::Pointer accMean = AccType::New();
  accMean->SetInput( reader->GetOutput() );
  accMean->SetAverage( true );
  
  reader->Update();
  
  
  std::cout << "#max" << "\t" 
            << "min" << "\t" 
            << "sum" << "\t" 
            << "accSum" << "\t" 
            << "mean" << "\t" 
            << "accMean" << "\t" 
            << "median" << "\t" 
            << "sigma" << "\t" 
            << "binary" << "\t" 
            << std::endl;

    itk::TimeProbe maxtime;
    itk::TimeProbe mintime;
    itk::TimeProbe sumtime;
    itk::TimeProbe accSumtime;
    itk::TimeProbe meantime;
    itk::TimeProbe sigmatime;
    itk::TimeProbe accMeantime;
    itk::TimeProbe mediantime;
    itk::TimeProbe binarytime;

    for( int i=0; i<10; i++ )
      {
      maxtime.Start();
      max->Update();
      maxtime.Stop();
      max->Modified();

      mintime.Start();
      min->Update();
      mintime.Stop();
      min->Modified();

      sumtime.Start();
      sum->Update();
      sumtime.Stop();
      sum->Modified();

      accSumtime.Start();
      accSum->Update();
      accSumtime.Stop();
      accSum->Modified();

      meantime.Start();
      mean->Update();
      meantime.Stop();
      mean->Modified();

      accMeantime.Start();
      accMean->Update();
      accMeantime.Stop();
      accMean->Modified();

      mediantime.Start();
      median->Update();
      mediantime.Stop();
      median->Modified();

      sigmatime.Start();
      sigma->Update();
      sigmatime.Stop();
      sigma->Modified();

      binarytime.Start();
      binary->Update();
      binarytime.Stop();
      binary->Modified();

      }
      
    std::cout << std::setprecision(3)
              << maxtime.GetMeanTime() << "\t" 
              << mintime.GetMeanTime() << "\t" 
              << sumtime.GetMeanTime() << "\t" 
              << accSumtime.GetMeanTime() << "\t" 
              << meantime.GetMeanTime() << "\t" 
              << accMeantime.GetMeanTime() << "\t" 
              << mediantime.GetMeanTime() << "\t" 
              << sigmatime.GetMeanTime() << "\t" 
              << binarytime.GetMeanTime() << "\t" 
              <<std::endl;
  
  return 0;
}

