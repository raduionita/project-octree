#ifndef __math_cbox_hpp__
#define __math_cbox_hpp__

#include <iostream>
#include <array>

namespace math
{
  class CBox : public CCube
  {
    public:
    math::real mHeight;
    math::real mDepth;
    
    public:
    CBox(math::real fValue = 0.0f) : CCube(fValue), mHeight(fValue), mDepth(fValue) { }
    
    CBox(const CBox& that)
    {
      mPosition.x = that.mPosition.x;
      mPosition.y = that.mPosition.y;
      mPosition.z = that.mPosition.z;
      mWidth      = that.mWidth;
      mHeight     = that.mHeight;
      mDepth      = that.mDepth;
    }
    
    CBox& operator = (const CBox& that)
    {
      if(this != &that)
      {
        mPosition.x = that.mPosition.x;
        mPosition.y = that.mPosition.y;
        mPosition.z = that.mPosition.z;
        mWidth      = that.mWidth;
        mHeight     = that.mHeight;
        mDepth      = that.mDepth;
      }
      return *this;
    }
    
    CBox& operator = (math::real fValue)
    {
      mPosition.x = fValue;
      mPosition.y = fValue;
      mPosition.z = fValue;
      mWidth  = fValue;
      mHeight = fValue;
      mDepth  = fValue;
      return *this;
    }
    
    CBox& operator += (const CBox& that)
    {
      if(this != &that)
      {
        if(that.getVolume() == 0.0f)
        {
          // do nothing
        }
        else if(getVolume() == 0.0f)
        {
          *this = that;
        }
        else
        {
          math::real wmin = std::min(mPosition.x - mWidth/2.0f,  that.mPosition.x - that.mWidth/2.0f);  // x
          math::real wmax = std::max(mPosition.x + mWidth/2.0f,  that.mPosition.x + that.mWidth/2.0f);  // x
          math::real hmin = std::min(mPosition.y - mHeight/2.0f, that.mPosition.y - that.mHeight/2.0f); // y
          math::real hmax = std::max(mPosition.y + mHeight/2.0f, that.mPosition.y + that.mHeight/2.0f); // y
          math::real dmin = std::min(mPosition.z - mDepth/2.0f,  that.mPosition.z - that.mDepth/2.0f);  // z
          math::real dmax = std::max(mPosition.z + mDepth/2.0f,  that.mPosition.z + that.mDepth/2.0f);  // z
          
          mWidth  = wmax - wmin;
          mHeight = hmax - hmin;
          mDepth  = dmax - dmin;
          mPosition.x = wmin + mWidth/2.0f; 
          mPosition.y = hmin + mHeight/2.0f; 
          mPosition.z = dmin + mDepth/2.0f; 
        }
      }
      return *this;
    }
    
    friend CBox operator + (const CBox& lhs, const CBox& rhs);
    
    friend bool operator == (const CBox& lhs, const CBox& rhs);
    
    friend bool operator != (const CBox& lhs, const CBox& rhs);
    
    friend std::ostream& operator << (std::ostream& out, const CBox& oBox);
    
    public:
    std::array<math::vec3, 8> getVertices() const
    {
      std::array<math::vec3, 8> aVertices;
      aVertices.fill(math::vec3(0.0f));
      
      math::real fHalfWidth  = mWidth/2.0f;
      math::real fHalfHeight = mHeight/2.0f;
      math::real fHalfDepth  = mDepth/2.0f;
      
      for(size_t i = 0; i < 2; ++i) // x
      for(size_t j = 0; j < 2; ++j) // y
      for(size_t k = 0; k < 2; ++k) // z
      {
        size_t key = i * 2 * 2 + j * 2 + k;
        math::vec3& p = aVertices[key];
        p.x = mPosition.x + (i == 0 ? (-fHalfWidth)  : (+fHalfWidth));
        p.y = mPosition.y + (j == 0 ? (-fHalfHeight) : (+fHalfHeight));
        p.z = mPosition.z + (k == 0 ? (-fHalfDepth)  : (+fHalfDepth));
      }
      
      return aVertices;
    }

    std::array<CPlane, 6> getPlanes() const
    {
      std::array<CPlane, 6> aPlanes;
      return aPlanes;
    }

    math::real getVolume() const
    {
      return mWidth * mHeight * mDepth;
    }
  };

  std::ostream& operator << (std::ostream& out, const CBox& oBox)
  {
    out << oBox.mPosition << " (" << oBox.mWidth << ", " << oBox.mHeight << ", " << oBox.mDepth << ")";
    return out;
  }

  bool operator == (const CBox& lhs, const CBox& rhs)
  {
    return lhs.mPosition.x == rhs.mPosition.x &&
           lhs.mPosition.y == rhs.mPosition.y &&
           lhs.mPosition.z == rhs.mPosition.z &&
           lhs.mWidth  == rhs.mWidth &&
           lhs.mHeight == rhs.mHeight&&
           lhs.mDepth  == rhs.mDepth;
  }

  bool operator != (const CBox& lhs, const CBox& rhs)
  {
    return !(lhs == rhs);
  }

  CBox operator + (const CBox& lhs, const CBox& rhs)
  {
    CBox res;
    
    if(rhs.getVolume() == 0.0f)
    {
      res = lhs;
    }
    else if(lhs.getVolume() == 0.0f)
    {
      res = rhs;
    }
    else
    {
      math::real wmin = std::min(lhs.mPosition.x - lhs.mWidth/2.0f,  rhs.mPosition.x - rhs.mWidth/2.0f);  // x
      math::real wmax = std::max(lhs.mPosition.x + lhs.mWidth/2.0f,  rhs.mPosition.x + rhs.mWidth/2.0f);  // x
      math::real hmin = std::min(lhs.mPosition.y - lhs.mHeight/2.0f, rhs.mPosition.y - rhs.mHeight/2.0f); // y
      math::real hmax = std::max(lhs.mPosition.y + lhs.mHeight/2.0f, rhs.mPosition.y + rhs.mHeight/2.0f); // y
      math::real dmin = std::min(lhs.mPosition.z - lhs.mDepth/2.0f,  rhs.mPosition.z - rhs.mDepth/2.0f);  // z
      math::real dmax = std::max(lhs.mPosition.z + lhs.mDepth/2.0f,  rhs.mPosition.z + rhs.mDepth/2.0f);  // z
      
      res.mWidth      = wmax - wmin;
      res.mHeight     = hmax - hmin;
      res.mDepth      = dmax - dmin;
      res.mPosition.x = wmin + res.mWidth/2.0f; 
      res.mPosition.y = hmin + res.mHeight/2.0f; 
      res.mPosition.z = dmin + res.mDepth/2.0f; 
    }
    
    return res;
  }

}

#endif // __math_cbox_hpp__
