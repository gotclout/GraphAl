#ifndef __DisjointSet__
#define __DisjointSet__

#include <map>

using std::map;

/**
 * Templated DisjointSet (Union Find) data structure
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
   *       configurable
   */
  struct Set
  {
    Thing  value;  // Template element
    Set*   parent; // Parent set (self if root)
    int    rank;   // Rank for union operation

    /**
     * Default Construct
     */
    Set() { parent = this; rank = 0; };

    /**
     * Construct from type
     */
    Set(const Thing & t) { value = t; parent = this; rank = 0; };

    /**
     * Less than operator overload for container compliance
     */
    bool operator<(const Set & rhs) { return value < rhs.value; };

    /**
     * Equivalence operator overload
     */
    bool operator==(const Set & rhs)
    {
      return !(value < rhs.value || rhs.value < value);
    };
  };

  /**
   * Wrapper for std::map comparator map<T*, Set*, Compare> force value
   * compare rather than pointer compare since T* is copied once in the
   * Set struct. Avoid copying it into the map since T could be large
   */
  struct ThingPtrCompare
  {
    bool operator()(const T* lhs, const T* rhs) { return *lhs < *rhs; };
  };

  typedef map<T*, Set*, ThingPtrCompare> Forest;
  typedef typename Forest::iterator      ForestIt;

  private:

  Forest forest; // Map Typename to Set for O(1) lookups

  public:

  /**
   * Default constructor
   */
  DisjointSet() { ; };

  /**
   * Destructor
   */
  ~DisjointSet()
  {
    ForestIt i = forest.begin(),
             e = forest.end();

    for(Set* s = 0; i != e; ++i)
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
    Set* ret            = new Set(e);
    forest[&ret->value] = ret;

    return ret;
  };

  /**
   * With path compression
   */
  Set* FindSet(Set* & s)
  {
    return *(s->parent) == *s ? s : s->parent = FindSet(s->parent);
  };

  /**
   * Find template element
   */
  T* Find(T & e)
  {
    return forest.find(&e) == forest.end() ? 0 : &FindSet(forest[&e])->value;
  };

  /**
   * By rank
   */
  Set* Union(T & x, T & y)
  {
    Set *xroot = FindSet(forest[&x]),
        *yroot = FindSet(forest[&y]),
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
   * TODO: This is for testing
   */
  const Forest& Get() { return forest; };

  /**
   * Retrieves the total number of unique elements in Forest
   */
  size_t size() { return forest.size(); };
};

#endif//__DisjointSet__
