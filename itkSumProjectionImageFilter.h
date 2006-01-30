#ifndef __itkSumProjectionImageFilter_h
#define __itkSumProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"

namespace itk {
/** \class SumProjectionImageFilter
 * \brief Sum projection
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ProjectionImageFilter
 */


namespace Function {
template <class TInputPixel, class TOuputPixel>
class SumAccumulator
{
public:
  SumAccumulator( unsigned long size )
    {
    m_Sum = NumericTraits< TInputPixel >::Zero;
    }
  ~SumAccumulator(){}

  inline TInputPixel operator()( const TInputPixel &input )
    {
    m_Sum = m_Sum + input;
    }

  inline TOuputPixel GetValue()
    {
    return m_Sum;
    }

  TOuputPixel m_Sum;
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT SumProjectionImageFilter :
    public
    ProjectionImageFilter<TInputImage, TOutputImage,
      Function::SumAccumulator< typename TInputImage::PixelType, typename TOutputImage::PixelType > >
{
public:
  typedef SumProjectionImageFilter Self;
  typedef ProjectionImageFilter<TInputImage, TOutputImage, 
    Function::SumAccumulator< typename TInputImage::PixelType, typename TOutputImage::PixelType > > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(SumProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  SumProjectionImageFilter() {}
  virtual ~SumProjectionImageFilter() {}

private:
  SumProjectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end SumProjectionImageFilter

} //end namespace itk
#endif
