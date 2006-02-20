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
  
  std::cout << "#dim" << "\t"
            << "size" << "\t"
            << "max" << "\t" 
            << "min" << "\t" 
            << "sum" << "\t" 
            << "accSum" << "\t" 
            << "mean" << "\t" 
            << "accMean" << "\t" 
            << "median" << "\t" 
            << "sigma" << "\t" 
            << "binary" << "\t" 
            << std::endl;

    for( int d=0; d<dim; d++ )
      {
      itk::TimeProbe maxtime;
      itk::TimeProbe mintime;
      itk::TimeProbe sumtime;
      itk::TimeProbe accSumtime;
      itk::TimeProbe meantime;
      itk::TimeProbe sigmatime;
      itk::TimeProbe accMeantime;
      itk::TimeProbe mediantime;
      itk::TimeProbe binarytime;

      max->SetProjectionDimension( d );
      min->SetProjectionDimension( d );
      sum->SetProjectionDimension( d );
      mean->SetProjectionDimension( d );
      accSum->SetAccumulateDimension( d );
      accMean->SetAccumulateDimension( d );
      median->SetProjectionDimension( d );
      sigma->SetProjectionDimension( d );
      binary->SetProjectionDimension( d );

      max->UpdateLargestPossibleRegion();
      min->UpdateLargestPossibleRegion();
      sum->UpdateLargestPossibleRegion();
      accSum->UpdateLargestPossibleRegion();
      mean->UpdateLargestPossibleRegion();
      accMean->UpdateLargestPossibleRegion();
      median->UpdateLargestPossibleRegion();
      sigma->UpdateLargestPossibleRegion();
      binary->UpdateLargestPossibleRegion();
	 
      for( int i=0; i<10; i++ )
        {
        maxtime.Start();
        for( int j=0; j<10; j++ )
          {
          max->Update();
          max->Modified();
          }
        maxtime.Stop();

        mintime.Start();
        for( int j=0; j<10; j++ )
          {
          min->Update();
          min->Modified();
          }
        mintime.Stop();
  
        sumtime.Start();
        for( int j=0; j<10; j++ )
          {
          sum->Update();
          sum->Modified();
          }
        sumtime.Stop();
  
        accSumtime.Start();
        for( int j=0; j<10; j++ )
          {
          accSum->Update();
          accSum->Modified();
          }
        accSumtime.Stop();
  
        meantime.Start();
        for( int j=0; j<10; j++ )
          {
          mean->Update();
          mean->Modified();
          }
        meantime.Stop();
  
        accMeantime.Start();
        for( int j=0; j<10; j++ )
          {
          accMean->Update();
          accMean->Modified();
          }
        accMeantime.Stop();
  
        mediantime.Start();
        for( int j=0; j<10; j++ )
          {
          median->Update();
          median->Modified();
          }
        mediantime.Stop();
  
        sigmatime.Start();
        for( int j=0; j<10; j++ )
          {
          sigma->Update();
          sigma->Modified();
          }
        sigmatime.Stop();
  
        binarytime.Start();
        for( int j=0; j<10; j++ )
          {
          binary->Update();
          binary->Modified();
          }
        binarytime.Stop();
  
        }
        
      std::cout << std::setprecision(3)
                << d << "\t" 
	        << reader->GetOutput()->GetLargestPossibleRegion().GetSize()[d] << "\t"
                << maxtime.GetMeanTime() / 10 << "\t" 
                << mintime.GetMeanTime() / 10 << "\t" 
                << sumtime.GetMeanTime() / 10 << "\t" 
                << accSumtime.GetMeanTime() / 10 << "\t" 
                << meantime.GetMeanTime() / 10 << "\t" 
                << accMeantime.GetMeanTime() / 10 << "\t" 
                << mediantime.GetMeanTime() / 10 << "\t" 
                << sigmatime.GetMeanTime() / 10 << "\t" 
                << binarytime.GetMeanTime() / 10 << "\t" 
                <<std::endl;
      }
  
  return 0;
}

