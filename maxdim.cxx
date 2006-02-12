#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkMaximumProjectionImageFilter.h"
#include "itkExtractImageFilter.h"


int main(int, char * argv[])
{
  int dim = atoi(argv[1]);

  typedef unsigned char PType;
  typedef itk::Image< PType, 3 > IType;
  typedef itk::Image< PType, 2 > IType2;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );

  typedef itk::MaximumProjectionImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetProjectionDimension( dim );
  // to be sure that the result is ok with several threads, even on a single
  // proc computer
  filter->SetNumberOfThreads( 2 );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  filter->Update();

  IType::SizeType inputSize = filter->GetOutput()->GetLargestPossibleRegion().GetSize();

  typedef itk::ExtractImageFilter< IType, IType2 > ExtractType;
  ExtractType::Pointer extract = ExtractType::New();
  extract->SetInput( filter->GetOutput() );

  IType::SizeType size;
  for(int i=0; i<=3; i++) {
    if(i == dim) {
      size[i] = 0;
    } else {
      size[i] = inputSize[i];
     }
  }
  IType::IndexType idx;
  idx.Fill(0);

  IType::RegionType region;
  region.SetSize( size );
  region.SetIndex( idx );
  extract->SetExtractionRegion( region );

  std::cout << "size: " << size << std::endl;
  std::cout << "index: " << idx << std::endl;

  extract->Update();
  std::cout << "output size: " << extract->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;

  typedef itk::ImageFileWriter< IType2 > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( extract->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  return 0;
}

