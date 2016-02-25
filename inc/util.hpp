#ifndef __util_hpp__
#define __util_hpp__

#include <vector>

namespace util
{
  /**
   * Merge 2 vectors into one(the first)
   * @param  std::vector<T>& lhs
   * @param  std::vector<T>& rhs
   * @return std::vector<T>& lhs
   * @todo   Need a better algorithm - a memcpy type one
   */
  template<typename T>
  std::vector<T>& merge(std::vector<T>& lhs, const std::vector<T>& rhs)
  {
    for(auto it = rhs.begin(); it != rhs.end(); ++it)
      lhs.push_back(*it);
    return lhs;
  }
}

#endif // __util_hpp__
