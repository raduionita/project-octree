#ifndef __math_ctriangle_hpp__
#define __math_ctriangle_hpp__

#include <array>
#include <cassert>

namespace math
{
  class CTriangle : public IShape
  {
    protected:
    std::array<vec3, 3> mVertices;
    
    public:
    vec3& operator [] (ushort i)
    {
      assert(i < 3);
      return mVertices[i];
    }
    
    const vec3& operator [] (ushort i) const
    {
      assert(i < 3);
      return mVertices[i];
    }
  };
}

#endif // __math_ctriangle_hpp__
