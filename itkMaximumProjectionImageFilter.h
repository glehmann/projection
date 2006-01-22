#ifndef __itkMaximumProjectionImageFilter_h
#define __itkMaximumProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"

namespace itk {
/** \class MaximumProjectionImageFilter
 * \brief TODO
 *
 * \sa ProjectionImageFilter
 */


namespace Function {
template <class TInputPixel>
class MaximumAccumulator
{
public:
  MaximumAccumulator()
    {
    m_Maximum = NumericTraits< TInputPixel >::NonpositiveMin();
    }
  ~MaximumAccumulator(){}

  inline TInputPixel operator()( const TInputPixel &input )
    {
    m_Maximum = std::max( m_Maximum, input );
    }

  inline TInputPixel GetValue()
    {
    return m_Maximum;
    }

  TInputPixel m_Maximum;
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT MaximumProjectionImageFilter :
    public
    ProjectionImageFilter<TInputImage, TOutputImage, Function::MaximumAccumulator< typename TInputImage::PixelType > >
{
public:
  typedef MaximumProjectionImageFilter Self;
  typedef ProjectionImageFilter<TInputImage, TOutputImage, Function::MaximumAccumulator< typename TInputImage::PixelType > > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(MaximumProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  MaximumProjectionImageFilter() {}
  virtual ~MaximumProjectionImageFilter() {}

private:
  MaximumProjectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end MaximumProjectionImageFilter

} //end namespace itk
#endif
