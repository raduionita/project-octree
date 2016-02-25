#ifndef __math_cvector_hpp__
#define __math_cvector_hpp__

#include <iostream>
#include <initializer_list>
#include <cassert>

namespace math
{
  class CVector3
  {
    public:
    union
    {
      real data[3];
      struct { real x; real y; real z; };
      struct { real s; real t; real p; };
      struct { real r; real g; real b; };
    };
    
    public:
    CVector3(real fValue = 0.0f) : x(fValue), y(fValue), z(fValue) { }
    
    CVector3(real x, real y, real z) : x(x), y(y), z(z) { }
    
    CVector3(std::initializer_list<real> list)
    {
      if(list.size() == 1) 
      {
        x = y = z = *(list.begin());
      }
      else if(list.size() == 3)
      {
        std::initializer_list<real>::iterator it;
        size_t i = 0;
        for(it = list.begin(); it != list.end(); ++it, ++i)
          data[i] = *it;
      }
      else
      {
        // error
      }
    }
    
    CVector3& operator = (std::initializer_list<real> list)
    {
      if(list.size() == 1) 
      {
        x = y = z = *(list.begin());
      }
      else if(list.size() == 3)
      {
        std::initializer_list<real>::iterator it;
        size_t i = 0;
        for(it = list.begin(); it != list.end(); ++it, ++i)
          data[i] = *it;
      }
      else
      {
        // error
      }
      
      return *this;
    }
    
    real operator [] (ushort i)
    {
      assert(i < 3);
      return data[i];
    }
    
    friend CVector3 operator * (const CVector3& lhs, real rhs);
    
    friend CVector3 operator + (const CVector3& lhs, const CVector3& rhs);
    
    friend CVector3 operator - (const CVector3& lhs, const CVector3& rhs);
    
    friend std::ostream& operator << (std::ostream& out, const CVector3& v);
    
    friend bool operator == (const CVector3& lhs, const CVector3& rhs);
  };
  
  std::ostream& operator << (std::ostream& out, const CVector3& v)
  {
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
  }

  inline bool operator == (const CVector3& lhs, const CVector3& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }

  CVector3 operator * (const CVector3& lhs, real rhs)
  {
    return CVector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
  }
  
  CVector3 operator + (const CVector3& lhs, const CVector3& rhs)
  {
    return CVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }
  
  CVector3 operator - (const CVector3& lhs, const CVector3& rhs)
  {
    return CVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }
  
  class CVector4
  {
    public:
    union
    {
      real data[3];
      struct { real x; real y; real z; real w; };
      struct { real s; real t; real p; real q; };
      struct { real r; real g; real b; real a; };
    };
    
    real operator [] (ushort i)
    {
      assert(i < 4);
      return data[i];
    }
  };
}

#endif // __math_cvector_hpp__
