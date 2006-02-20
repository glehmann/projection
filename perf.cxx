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
  
  reader->Update();
  
  
  std::cout << "#th" << "\t" 
            << "max" << "\t" 
            << "min" << "\t" 
            << "sum" << "\t" 
            << "mean" << "\t" 
            << "median" << "\t" 
            << "sigma" << "\t" 
            << "binary" << "\t" 
            << std::endl;

    for( int th=1; th<=10; th++ )
      {
      itk::TimeProbe maxtime;
      itk::TimeProbe mintime;
      itk::TimeProbe sumtime;
      itk::TimeProbe meantime;
      itk::TimeProbe sigmatime;
      itk::TimeProbe mediantime;
      itk::TimeProbe binarytime;

      max->SetNumberOfThreads( th );
      min->SetNumberOfThreads( th );
      sum->SetNumberOfThreads( th );
      mean->SetNumberOfThreads( th );
      median->SetNumberOfThreads( th );
      sigma->SetNumberOfThreads( th );
      binary->SetNumberOfThreads( th );

      for( int i=0; i<10; i++ )
        {
        maxtime.Start();
        for( int i=0; i<10; i++ )
          {
          max->Update();
          max->Modified();
          }
        maxtime.Stop();
  
        mintime.Start();
        for( int i=0; i<10; i++ )
          {
          min->Update();
          min->Modified();
          }
        mintime.Stop();
  
        sumtime.Start();
        for( int i=0; i<10; i++ )
          {
          sum->Update();
          sum->Modified();
          }
        sumtime.Stop();
  
        meantime.Start();
        for( int i=0; i<10; i++ )
          {
          mean->Update();
          mean->Modified();
          }
        meantime.Stop();
  
        mediantime.Start();
        for( int i=0; i<10; i++ )
          {
          median->Update();
          median->Modified();
          }
        mediantime.Stop();
  
        sigmatime.Start();
        for( int i=0; i<10; i++ )
          {
          sigma->Update();
          sigma->Modified();
          }
        sigmatime.Stop();
  
        binarytime.Start();
        for( int i=0; i<10; i++ )
          {
          binary->Update();
          binary->Modified();
          }
        binarytime.Stop();
  
        }
        
      std::cout << std::setprecision(3)
                << th << "\t" 
                << maxtime.GetMeanTime() / 10 << "\t" 
                << mintime.GetMeanTime() / 10 << "\t" 
                << sumtime.GetMeanTime() / 10 << "\t" 
                << meantime.GetMeanTime() / 10 << "\t" 
                << mediantime.GetMeanTime() / 10 << "\t" 
                << sigmatime.GetMeanTime() / 10 << "\t" 
                << binarytime.GetMeanTime() / 10 << "\t" 
                <<std::endl;
      }
  
  return 0;
}

