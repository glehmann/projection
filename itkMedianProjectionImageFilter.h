#ifndef __itkMedianProjectionImageFilter_h
#define __itkMedianProjectionImageFilter_h

#include "itkProjectionImageFilter.h"
#include "itkNumericTraits.h"
#include <vector>
#include <algorithm>

namespace itk {
/** \class MedianProjectionImageFilter
 * \brief Median projection
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ProjectionImageFilter
 */


namespace Function {
template <class TInputPixel, class TIndexType>
class MedianAccumulator
{
public:
  MedianAccumulator( unsigned long size )
    {
    m_Values.reserve( size );
    }
  ~MedianAccumulator(){}

  inline TInputPixel operator()( const TInputPixel &input, const TIndexType &index )
    {
    m_Values.push_back( input );
    }

  inline TInputPixel GetValue()
    {
    const typename std::vector<TInputPixel>::iterator medianIterator = m_Values.begin() +  m_Values.size() / 2;
    std::nth_element(m_Values.begin(), medianIterator, m_Values.end() );
    return *medianIterator;
    }

  std::vector<TInputPixel> m_Values;
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT MedianProjectionImageFilter :
    public
    ProjectionImageFilter<TInputImage, TOutputImage, Function::MedianAccumulator< typename TInputImage::PixelType, typename TInputImage::IndexType > >
{
public:
  typedef MedianProjectionImageFilter Self;
  typedef ProjectionImageFilter<TInputImage, TOutputImage, Function::MedianAccumulator< typename TInputImage::PixelType, typename TInputImage::IndexType > > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(MedianProjectionImageFilter, ProjectionImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  MedianProjectionImageFilter() {}
  virtual ~MedianProjectionImageFilter() {}

private:
  MedianProjectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end MedianProjectionImageFilter

} //end namespace itk
#endif
