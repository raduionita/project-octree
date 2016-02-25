#ifndef __math_cmatrix_hpp__
#define __math_cmatrix_hpp__

namespace math
{
  class CMatrix4x4
  {
    private: 
    CVector4 data[4];
    
    public:
    CVector4&      operator [] (const ushort j)
    {
      return data[j];
    }
    const CVector4& operator [] (const ushort j) const
    {
      return data[j];
    }
  };
}

#endif // __math_cmatrix_hpp__
