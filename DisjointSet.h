#ifndef __DisjointSet__
#define __DisjointSet__

#include <map>

using std::map;

/**
 *
 */
template <typename Thing>
class DisjointSet
{
  public:

  typedef Thing T;

  /**
   * TODO: Probably make struct Set
   *       Consider Path Compression
   *       Union & Union By Rank
   */
  struct Set
  {
    Thing value;  //
    Set*  parent; //
    int   rank;   //

    /**
     *
     */
    bool operator==(const Set & rhs)
    {
      return !(value < rhs.value || rhs.value < value);
    };
  };

  typedef map<T, Set*> Forest;
  typedef typename Forest::iterator ForestIt;

  private:

  Forest forest; // Map Typename to Set of O(1) lookups

  public:

  /**
   * Default constructor
   */
  DisjointSet()
  {
    ;
  };

  /**
   * Destructor
   */
  ~DisjointSet()
  {
    ForestIt i = forest.begin(),
             e = forest.end();
    Set*     s = 0;

    for( ; i != e; ++i)
    {
      if(i->second)
      {
        s = i->second;
        delete s;
        s = 0;
      }
    }

    forest.clear();
  };

  /**
   * Creates a singleton set
   */
  Set* MakeSet(const T & e)
  {
    Set* ret    = new Set;
    ret->value  = e;
    ret->rank   = 0;
    ret->parent = ret;
    forest[e]   = ret;

    return ret;
  };

  /**
   *
   */
  Set* FindSet(Set* & s)
  {
    Set* ret = 0;

    ret = *(s->parent) == *s ? s : s->parent = FindSet(s->parent);

    return ret;
  };

  /**
   * TODO: Rename Find
   */
  T* FindSet(const T & e)
  {
    return forest.find(e) == forest.end() ? 0 : &FindSet(forest[e])->value;
  };

  /**
   *
   */
  Set* Union(T & x, T & y)
  {
    Set *xroot = FindSet(forest[x]),
        *yroot = FindSet(forest[y]),
        *ret   = xroot;

    if(!(*xroot == *yroot))
    {
      if(xroot->rank < yroot->rank)
      {
        xroot->parent = yroot;
        ret = yroot;
      }
      else if(xroot->rank > yroot->rank)
      {
        yroot->parent = xroot;
      }
      else
      {
        yroot->parent = xroot;
        xroot->rank++;
      }
    }

    return ret;
  };

  /**
   * Testing
   */
  const Forest& Get()
  {
    return forest;
  };
};

#endif//__DisjointSet__
