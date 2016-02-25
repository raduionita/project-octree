#ifndef __math_cplane_hpp__
#define __math_cplane_hpp__

#include <cassert>

namespace math
{
  class CPlane
  {
    public:
    union 
    {
      real data[4];
      struct { 
        real x; // shows plane pozitive side
        real y; 
        real z; 
        real d; // from origin
      };
    };
    
    public:
    CPlane(real fValue = 0.0f) : x(fValue), y(fValue), z(fValue), d(fValue) { }
    
    CPlane(real x, real y, real z, real d) : x(x), y(y), z(z), d(d) { }
    
    CPlane(vec3 n, real d) : x(n.x), y(n.y), z(n.z), d(d) { }
    
    vec3 getNormal() const
    {
      return vec3(x, y, z);
    }
    
    real getDistance() const
    {
      return d;
    }
    
    real operator [] (ushort i) const
    {
      assert(i < 4);
      return data[i];
    }
  };
}

#endif // __math_cplane_hpp__
