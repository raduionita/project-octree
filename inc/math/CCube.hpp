#ifndef __math_ccube_hpp__
#define __math_ccube_hpp__

namespace math
{
  class CCube : public CShape 
  {
    public:
    math::real mWidth;
    
    public:
    CCube(math::real fValue = 0.0f) : CShape(), mWidth(fValue) { }
  };
}

#endif // __math_ccube_hpp__
