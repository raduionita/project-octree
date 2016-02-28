#ifndef __core_hpp__
#define __core_hpp__

#include <exception>
#include <iostream>

namespace core
{
  // @todo
  // create a core module from where all other(ogl, tree, math) modules start
  
  class CApp
  {
    
  };
  
  class CException : public std::exception
  {
    protected:
    std::string mMessage;
    
    public:
    CException(const std::string& message) : mMessage(message) { }
    
    public:
    virtual const char* what()
    {
      return mMessage.c_str();
    }
  };
}

#endif // __core_hpp__
