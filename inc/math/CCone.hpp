#ifndef __math_ccone_hpp__
#define __math_ccone_hpp__

namespace math
{
  class CCone : public CShape
  {
    protected:
    real mRadius;
    real mHeight;
    
    public:
    CCone(real fValue = 0.0f) : CShape(), mRadius(fValue), mHeight(fValue)
    {
      
    }
  };
}

#endif // __math_ccone_hpp__
