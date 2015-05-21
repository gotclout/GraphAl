#ifndef __UTILS__
#define __UTILS__

#include <map>
#include <list>
#include <vector>

using namespace std;

#define value_type typename Container::value_type

/**
 * Quicksort Implementation
 */
template <class Container>
void aqsort(Container & c, size_t left, size_t right)
{
  size_t i = left, j = right;

  if(j - i <= 1)
    return;
  else
  {
    value_type pivot = c.at((left + right)/2);

    while(i < j)
    {
      while(c.at(i) < pivot)i++;
      while(c.at(j) > pivot)j--;

      if(i < j)
      {
        value_type tmp = c.at(i);
        c.at(i++) = c.at(j);
        c.at(j--) = tmp;
      }
      if(left < j)
        aqsort(c, left, j);
      if(i < right)
        aqsort(c, i, right);
    }
  }
}

class Utils
{
  private:

  public:

  protected:
};

#endif//__UTILS__
