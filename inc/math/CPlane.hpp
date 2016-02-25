#ifndef __math_cplane_hpp__
#define __math_cplane_hpp__

namespace math
{
  class CPlane
  {
    public:
    math::vec3 mNormal;   // shows plane pozitive side
    math::real mDistance; // from origin
  };
}

#endif // __math_cplane_hpp__
