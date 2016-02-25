#ifndef __math_cshape_hpp__
#define __math_cshape_hpp__

namespace math
{
  class CShape
  {
    public:
    math::vec3 mPosition;
    
    public:
    CShape()
    {
      mPosition = { 0.0f, 0.0f, 0.0f };
    }
  };
}

#endif // __math_cshape_hpp__
