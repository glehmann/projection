/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkUpperDimensionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/12/09 13:27:12 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkUpperDimensionImageFilter_txx
#define _itkUpperDimensionImageFilter_txx

#include "itkUpperDimensionImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkProgressReporter.h"
#include "itkImageLinearIteratorWithIndex.h"


namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
UpperDimensionImageFilter<TInputImage,TOutputImage>
::UpperDimensionImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  m_NewDimension = OutputImageDimension-1;
  m_NewDimensionSapcing = 1.0;
  m_NewDimensionOrig = 0.0;
  m_NewDimensionIndex = 0;
  m_NewDimensionSize = 1;
}


template <class TInputImage, class TOutputImage>
void
UpperDimensionImageFilter<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  itkDebugMacro("GenerateOutputInformation Start");

  typename TOutputImage::RegionType outputRegion;
  typename TInputImage::IndexType inputIndex;
  typename TInputImage::SizeType  inputSize;
  typename TOutputImage::SizeType  outputSize;
  typename TOutputImage::IndexType outputIndex;
  typename TInputImage::SpacingType inSpacing;
  typename TInputImage::PointType inOrigin;
  typename TOutputImage::SpacingType outSpacing;
  typename TOutputImage::PointType outOrigin;

  // Get pointers to the input and output
  typename Superclass::OutputImagePointer output = this->GetOutput();
  typename Superclass::InputImagePointer input = const_cast< TInputImage * >( this->GetInput() );

  inputIndex = input->GetLargestPossibleRegion().GetIndex();
  inputSize = input->GetLargestPossibleRegion().GetSize();
  inSpacing = input->GetSpacing();
  inOrigin = input->GetOrigin();

  for(unsigned int i = 0; i<OutputImageDimension; i++)
    {
    if (i == m_NewDimension)
      {
      outputSize[i]  = m_NewDimensionSize;
      outputIndex[i] = m_NewDimensionIndex;
      outSpacing[i] = m_NewDimensionSapcing;
      outOrigin[i]  = m_NewDimensionOrig;
      }
    else if (i == OutputImageDimension-1)
      {
      outputSize[i]  = inputSize[m_NewDimension];
      outputIndex[i] = inputIndex[m_NewDimension];
      outSpacing[i] = inSpacing[m_NewDimension];
      outOrigin[i]  = inOrigin[m_NewDimension];
      }
    else
      {
      outputSize[i]  = inputSize[i];
      outputIndex[i] = inputIndex[i];
      outSpacing[i] = inSpacing[i];
      outOrigin[i]  = inOrigin[i];
      }
    }

  outputRegion.SetSize(outputSize);
  outputRegion.SetIndex(outputIndex);
  output->SetOrigin(outOrigin);
  output->SetSpacing(outSpacing);
  output->SetLargestPossibleRegion(outputRegion);

  itkDebugMacro("GenerateOutputInformation End");
std::cout <<  "outputSize" << outputSize <<std::endl;
std::cout <<  "outputIndex" << outputIndex <<std::endl;
}


template <class TInputImage, class  TOutputImage>
void
UpperDimensionImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  itkDebugMacro("GenerateInputRequestedRegion Start");
  Superclass::GenerateInputRequestedRegion();

  if ( this->GetInput() )
    {
    typename TInputImage::RegionType RequestedRegion;
    typename TInputImage::SizeType  inputSize;
    typename TInputImage::IndexType inputIndex;
    typename TInputImage::SizeType  inputLargSize;
    typename TInputImage::IndexType inputLargIndex;
    typename TOutputImage::SizeType  outputSize;
    typename TOutputImage::IndexType outputIndex;

    outputIndex = this->GetOutput()->GetRequestedRegion().GetIndex();
    outputSize = this->GetOutput()->GetRequestedRegion().GetSize();
    inputLargSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
    inputLargIndex = this->GetInput()->GetLargestPossibleRegion().GetIndex();

    for(unsigned int i=0; i<InputImageDimension; i++)
      {
      if( i != m_NewDimension)
        {
        inputSize[i] = outputSize[i];
        inputIndex[i] = outputIndex[i];
        }
      else
        {
        inputSize[i] = outputSize[InputImageDimension-1];
        inputIndex[i] = outputIndex[InputImageDimension-1];
        }
      }

    RequestedRegion.SetSize(inputSize);
    RequestedRegion.SetIndex(inputIndex);
    InputImagePointer input = const_cast< TInputImage * > ( this->GetInput() );
    input->SetRequestedRegion(RequestedRegion);
std::cout <<  "inputSize" << inputSize <<std::endl;
std::cout <<  "inputIndex" << inputIndex <<std::endl;
    }

  itkDebugMacro("GenerateInputRequestedRegion End");
}


template <class TInputImage, class TOutputImage>
void
UpperDimensionImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId )
{
  // use the output image to report the progress: there is no need to call CompletedPixel()
  // for all input pixel
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  typedef typename TOutputImage::PixelType OutputPixelType;
  
  // get some values, just to be easier to manipulate
  typename Superclass::InputImageConstPointer  inputImage = this->GetInput();
  typename TInputImage::RegionType inputRegion = inputImage->GetLargestPossibleRegion();
  typename TInputImage::SizeType inputSize = inputRegion.GetSize();
  typename TInputImage::IndexType inputIndex = inputRegion.GetIndex();

  typename TOutputImage::Pointer outputImage = this->GetOutput();
  typename TOutputImage::RegionType outputRegion = outputImage->GetLargestPossibleRegion();
  typename TOutputImage::SizeType outputSize = outputRegion.GetSize();
  typename TOutputImage::IndexType outputIndex = outputRegion.GetIndex();

  typename TOutputImage::SizeType outputSizeForThread = outputRegionForThread.GetSize();
  typename TOutputImage::IndexType outputIndexForThread = outputRegionForThread.GetIndex();

  // create the iterators for input and output image
  typedef ImageLinearIteratorWithIndex<TOutputImage> OutputIteratorType;
  OutputIteratorType oIt( outputImage, outputRegionForThread );
  oIt.SetDirection( m_NewDimension );
  oIt.GoToBegin();

  // ok, everything is ready... lets the linear iterator do its job !
  while( !oIt.IsAtEnd() )
    {
    // get the value from the input image to put to the output image
    typename TOutputImage::IndexType oIdx = oIt.GetIndex();
    typename TInputImage::IndexType iIdx;
    for( unsigned int i=0; i< InputImageDimension; i++ )
      {
      if( i != m_NewDimension )
        { iIdx[i] = oIdx[i]; }
      else
        { iIdx[i] = oIdx[OutputImageDimension-1]; }
      }
    InputImagePixelType p = inputImage->GetPixel( iIdx );

    while( !oIt.IsAtEndOfLine() )
      {
      oIt.Set( p );
      ++oIt;
      progress.CompletedPixel();
      }

    // continue with the next one
    oIt.NextLine();
    }

}


template <class TInputImage, class TOutputImage>
void
UpperDimensionImageFilter<TInputImage,TOutputImage>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "NewDimension: " << m_NewDimension << std::endl;
}


} // end namespace itk


#endif
