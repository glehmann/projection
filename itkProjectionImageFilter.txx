/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkProjectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/12/09 13:27:12 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkProjectionImageFilter_txx
#define _itkProjectionImageFilter_txx

#include "itkProjectionImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkProgressReporter.h"


namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TAccumulator>
ProjectionImageFilter<TInputImage,TOutputImage,TAccumulator>
::ProjectionImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  m_Axe=InputImageDimension-1;
}


template <class TInputImage, class TOutputImage, class TAccumulator>
void
ProjectionImageFilter<TInputImage,TOutputImage,TAccumulator>
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

  // Set the LargestPossibleRegion of the output.
  // Reduce the size of the accumulated dimension.
  for(unsigned int i = 0; i<InputImageDimension; i++)
    {
    if (i != m_Axe)
      {
      outputSize[i]  = inputSize[i];
      outputIndex[i] = inputIndex[i];
      outSpacing[i] = inSpacing[i];
      outOrigin[i]  = inOrigin[i];
      }
    else
      {
      outputSize[i]  = 1;
      outputIndex[i] = 0;
      outSpacing[i] = inSpacing[i]*inputSize[i];
      outOrigin[i]  = inOrigin[i] + (i-1)*inSpacing[i]/2;
      }
    }

  outputRegion.SetSize(outputSize);
  outputRegion.SetIndex(outputIndex);
  output->SetOrigin(outOrigin);
  output->SetSpacing(outSpacing);
  output->SetLargestPossibleRegion(outputRegion);

  itkDebugMacro("GenerateOutputInformation End");
}


template <class TInputImage, class  TOutputImage, class TAccumulator>
void
ProjectionImageFilter<TInputImage,TOutputImage,TAccumulator>
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

    for(unsigned int i=0; i<TInputImage::ImageDimension; i++)
      {
      if(i!=m_Axe)
        {
        inputSize[i] = outputSize[i];
        inputIndex[i] = outputIndex[i];
        }
      else
        {
        inputSize[i]=inputLargSize[i];
        inputIndex[i]=inputLargIndex[i];
        }
      }

    RequestedRegion.SetSize(inputSize);
    RequestedRegion.SetIndex(inputIndex);
    InputImagePointer input = const_cast< TInputImage * > ( this->GetInput() );
    input->SetRequestedRegion (RequestedRegion);
    }

  itkDebugMacro("GenerateInputRequestedRegion End");
}


/**
 * GenerateData Performs the accumulation
 */
template <class TInputImage, class TOutputImage, class TAccumulator>
void
ProjectionImageFilter<TInputImage,TOutputImage,TAccumulator>
::GenerateData( void )
{
  if(m_Axe>=TInputImage::ImageDimension)
    {
    itkExceptionMacro(<<"ProjectionImageFilter: invalid dimension to accumulate. Axe = " << m_Axe);
    }

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename NumericTraits<OutputPixelType>::AccumulateType AccumulateType;
  
  typename Superclass::InputImageConstPointer  inputImage = this->GetInput();
  typename TOutputImage::Pointer outputImage = this->GetOutput();
  outputImage->SetBufferedRegion( outputImage->GetRequestedRegion() );
  outputImage->Allocate();

// Accumulate over the Nth dimension ( = m_Axe)
// and divide by the size of the accumulated dimension.
  typedef ImageRegionIterator<TOutputImage> outputIterType;
  outputIterType outputIter(outputImage, outputImage->GetBufferedRegion());
  typedef ImageRegionConstIteratorWithIndex<TInputImage> inputIterType;
  
  typename TInputImage::RegionType AccumulatedRegion;
  typename TInputImage::SizeType AccumulatedSize = inputImage->GetLargestPossibleRegion().GetSize();
  typename TInputImage::IndexType AccumulatedIndex = inputImage->GetLargestPossibleRegion().GetIndex();

  unsigned long SizeAxe = AccumulatedSize[m_Axe];
  double SizeAxeDouble = static_cast<double>(SizeAxe);
  long IndexAxe = AccumulatedIndex[m_Axe];
  for(unsigned int i=0; i< InputImageDimension; i++)
    {
    if (i != m_Axe )
      {
      AccumulatedSize[i] = 1;
      }
    }
  AccumulatorType accumulator = this->NewAccumulator( SizeAxe );
  AccumulatedRegion.SetSize(AccumulatedSize);
  outputIter.GoToBegin();
  while(!outputIter.IsAtEnd())
    {
    accumulator.Init();
    typename TOutputImage::IndexType OutputIndex = outputIter.GetIndex();
    for(unsigned int i=0; i<InputImageDimension; i++)
      {
      if (i != m_Axe)
        {
        AccumulatedIndex[i] = OutputIndex[i];
        }
      else
        {
        AccumulatedIndex[i] = IndexAxe;
        }
      }
    AccumulatedRegion.SetIndex(AccumulatedIndex);
    inputIterType inputIter(inputImage, AccumulatedRegion);
    inputIter.GoToBegin();

    while(!inputIter.IsAtEnd())
      {
      accumulator(inputIter.Get(), inputIter.GetIndex());
      ++inputIter;
      }
    outputIter.Set(static_cast<OutputPixelType>(accumulator.GetValue()));
    progress.CompletedPixel();
    ++outputIter;
    }
}


template <class TInputImage, class TOutputImage, class TAccumulator>
TAccumulator
ProjectionImageFilter<TInputImage,TOutputImage,TAccumulator>::
NewAccumulator( unsigned long size )
{
  return TAccumulator( size );
}


template <class TInputImage, class TOutputImage, class TAccumulator>
void
ProjectionImageFilter<TInputImage,TOutputImage,TAccumulator>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Axe: " << m_Axe << std::endl;
}


} // end namespace itk


#endif
