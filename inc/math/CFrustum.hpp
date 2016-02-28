#ifndef __math_CFRUSTUM_HPP__
#define __math_CFRUSTUM_HPP__

#include <array>

namespace math
{
  class CFrustum : public IShape
  {
    typedef CPlane plane;
    
    //       +---+
    //      / _ / |
    //      |   |/
    //      +---+ 
    
    enum EPlanes 
    { 
      NEAR = 0, 
      FAR, 
      BOTTOM, 
      TOP, 
      LEFT, 
      RIGHT 
    };
    
    enum EVertices 
    {
      LNB = 0, // 0 left  near bottom
      RNB,     // 1 right near bottom
      RFB,     // 2 right far  bottom
      LFB,     // 3 left  far  bottom
      LNT,     // 4 left  near top
      RNT,     // 5 right near top
      RFT,     // 6 right far  top
      LFT      // 7 left  far  top
    };
    
    protected:
    std::array<vec3, 8> mVertices;
  
    public:
    CFrustum() 
    {
      mVertices.fill(vec3(0.0f));
    }
    
    CFrustum(vec3 position, vec3 direction, vec3 up, real fov, real ratio, real near, real far)
    {
      vec3 right = cross(direction, up);
    
      real fHeightNear = 2 * std::tan(fov/2.0f) * near;
      real fWidthNear  = fHeightNear * ratio;
      real fHeightFar  = 2 * std::tan(fov/2.0f) * far;
      real fWidthFar   = fHeightFar * ratio;
      vec3 vCenterNear = position + direction * near;
      vec3 vCenterFar  = position + direction * far;
      
      mVertices[LNT] = vCenterNear + (up * (fHeightNear/2.0f)) - (right * (fWidthNear/2.0f)); // 4
      mVertices[RNT] = vCenterNear + (up * (fHeightNear/2.0f)) + (right * (fWidthNear/2.0f)); // 5
      mVertices[LNB] = vCenterNear - (up * (fHeightNear/2.0f)) - (right * (fWidthNear/2.0f)); // 0
      mVertices[RNB] = vCenterNear - (up * (fHeightNear/2.0f)) + (right * (fWidthNear/2.0f)); // 1
      mVertices[LFT] = vCenterFar  + (up * (fHeightFar/2.0f))  - (right * (fWidthFar/2.0f));  // 7
      mVertices[RFT] = vCenterFar  + (up * (fHeightFar/2.0f))  + (right * (fWidthFar/2.0f));  // 6
      mVertices[LFB] = vCenterFar  - (up * (fHeightFar/2.0f))  - (right * (fWidthFar/2.0f));  // 3
      mVertices[RFB] = vCenterFar  - (up * (fHeightFar/2.0f))  + (right * (fWidthFar/2.0f));  // 2
    }
    
    CFrustum(const std::array<vec3, 8>& vertices) : mVertices(vertices) 
    {
      
    }
    
    CFrustum(const mat4& mvp)  
    {
      // @todo:
      // extract frustum planes from ModeViewProjection matrix
    }
    
    public:
    std::array<vec3, 8> getVertices() const
    {
      return mVertices;
    }
    
    std::array<plane, 6> getPlanes() const
    {
      std::array<plane, 6> planes;
      
      // derive plane from 3 points
      // e0 = p1 - p0;
      // e1 = p2 - p0;
      // n = cross(e0, e1)
      // n = normalize(n)
      // plane.n = n; // normalized
      // plane.d = -dot(n,p0) 
      
      return planes;
    }
    
    vec3 getCenter() const
    {
      return vec3(0.0f);
    }
  };
}

#endif // __math_CFRUSTUM_HPP__
