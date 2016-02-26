#ifndef __math_cplane_hpp__
#define __math_cplane_hpp__

#include <cassert>

namespace math
{
  class CPlane
  {
    // ax + by + cy + d = 0
    // (a, b, c) = normal
    // d = dist plane to origin
    public:
    union 
    {
      real data[4];
      struct { 
        real x; // shows plane pozitive side
        real y; 
        real z; 
        real w; // from origin
      };
      struct { 
        real a; // shows plane pozitive side
        real b; 
        real c; 
        real d; // from origin
      };
    };
    
    public:
    CPlane(real fValue = 0.0f) : x(fValue), y(fValue), z(fValue), w(fValue) { }
    
    CPlane(real x, real y, real z, real d) : x(x), y(y), z(z), w(d) { }
    
    CPlane(const vec3& n, real d) : x(n.x), y(n.y), z(n.z), w(d) { }
    
    CPlane(const vec4& p) : a(p.x), b(p.y), c(p.z), d(p.w) { }
    
    CPlane(const vec3& p0, const vec3& p1, const vec3& p2)
    {
      // derive plane from 3 points
      vec3 e0 = p1 - p0;
      vec3 e1 = p2 - p0;
      vec3 n = normalize(cross(e0, e1));
      x = n.x;
      y = n.y;
      z = n.z;
      d = -dot(n, p0);
    }
    
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
