#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkUpperDimensionImageFilter.h"
#include "itkExtractImageFilter.h"


int main(int, char * argv[])
{
  int dim = atoi(argv[3]);
  int size = atoi(argv[4]);

  typedef unsigned char PType;
  typedef itk::Image< PType, 3 > IType;
  typedef itk::Image< PType, 2 > IType2;

  typedef itk::ImageFileReader< IType2 > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::UpperDimensionImageFilter< IType2, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetNewDimension( dim );
  filter->SetNewDimensionSize( size );
  // to be sure that the result is ok with several threads, even on a single
  // proc computer
  filter->SetNumberOfThreads( 2 );

//   itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

