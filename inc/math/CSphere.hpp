#ifndef __math_csphere_hpp__
#define __math_csphere_hpp__

namespace math
{
  class CSphere : public CShape
  { 
    public:
    math::real mRadius;
    
    public:
    CSphere(math::real fRadius) : CShape(), mRadius(fRadius) { }
  };
}

#endif // __math_csphere_hpp__
