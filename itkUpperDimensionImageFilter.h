/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkUpperDimensionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/09/23 03:06:06 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkUpperDimensionImageFilter_h
#define __itkUpperDimensionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConceptChecking.h"

namespace itk
{
  
/** \class UpperDimensionImageFilter
 * \brief Implements an accumulation of an image along a selected direction.
 *
 *    This class accumulates an image along a dimension and reduce the size 
 * of this dimension to 1. The dimension being accumulated is set by 
 * NewDimension. 
 *
 *   Each pixel is the cumulative sum of the pixels along the collapsed
 * dimension and reduce the size of the accumulated dimension to 1 (only 
 * on the accumulated). 
 *
 *   The dimensions of the InputImage and the OutputImage must be the same.
 *
 * This class is parameterized over the type of the input image and
 * the type of the output image.
 *
 * \author Emiliano Beronich
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup   IntensityImageFilters     Singlethreaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT UpperDimensionImageFilter : public ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef UpperDimensionImageFilter  Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UpperDimensionImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename    InputImageType::Pointer    InputImagePointer;
  typedef typename    InputImageType::RegionType InputImageRegionType;
  typedef typename    InputImageType::PixelType  InputImagePixelType;
  typedef TOutputImage OutputImageType;
  typedef typename     OutputImageType::Pointer    OutputImagePointer;
  typedef typename     OutputImageType::RegionType OutputImageRegionType;
  typedef typename     OutputImageType::PixelType  OutputImagePixelType;

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Input and output images must be the same dimension. */
//   itkConceptMacro(ImageDimensionCheck,
//       (Concept::SameDimension<itkGetStaticConstMacro(InputImageDimension),
//                               itkGetStaticConstMacro(OutputImageDimension)>));

  /** Set the direction in which to accumulate the data.  It must be
   * set before the update of the filter. Defaults to the last
   * dimension. */
  itkGetMacro( NewDimension, unsigned int );
  itkSetMacro( NewDimension, unsigned int );

  itkGetMacro( NewDimensionSapcing, double );
  itkSetMacro( NewDimensionSapcing, double );

  itkGetMacro( NewDimensionOrig, double );
  itkSetMacro( NewDimensionOrig, double );

  itkGetMacro( NewDimensionIndex, long );
  itkSetMacro( NewDimensionIndex, long );

  itkGetMacro( NewDimensionSize, unsigned long );
  itkSetMacro( NewDimensionSize, unsigned long );

protected:
  UpperDimensionImageFilter();
  virtual ~UpperDimensionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Apply changes to the output image information. */
  virtual void GenerateOutputInformation();

  /** Apply changes to the input image requested region. */
  virtual void GenerateInputRequestedRegion();

  virtual void ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, int threadId );

private:
  UpperDimensionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_NewDimension;

  double m_NewDimensionSapcing;

  double m_NewDimensionOrig;

  long m_NewDimensionIndex;

  unsigned long m_NewDimensionSize;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkUpperDimensionImageFilter.txx"
#endif

#endif


