#ifndef __Vertex__
#define __Vertex__

#define NIL NULL

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <climits>

using std::map;
using std::vector;
using std::list;
using std::string;
using std::iostream;
using std::ostream;
using std::endl;

/** Vertex Color Enumeration **/
enum EColor
{
  eWhite = 0,
  eGray,
  eBlack,
  eUndefined
};

/*******************************************************************************
 * Structure for expressing graph vertex
 ******************************************************************************/
struct Vertex
{

  typedef list<Vertex*>     AdjList;
  typedef AdjList::iterator AdjListIt;

  AdjList* adj;   //adjacent verticies
  Vertex*  pi;    //parent
  EColor   color; //state
  int      d,     //discovery time
           f,     //finish time
           l,     //lowest
           mcap;  //min capacity of path to vertex
  string   id;    //uid

  /**
   * Default construct
   */
  Vertex()
  {
    adj   = 0;
    pi    = 0;
    color = eUndefined;
    d     = l = f = -1;
    id    = "DEFAULT";
    mcap  = INT_MAX;
  };

  /**
   * Construct from id
   */
  Vertex(string pId)
  {
    adj   = 0;
    pi    = 0;
    color = eUndefined;
    id    = pId;
    d     = l = f = -1;
    mcap  = INT_MAX;
  };

  /**
   * Adds a vertex to this vertexes adjaceny list
   */
  bool add_adj(Vertex* & v)
  {
    bool ret = true;
    if(v)
      adj->push_back(v);
    else
      ret = false;

    return ret;
  };

  /**
   * TODO: Copy/Assignment
   */
  /*
  Vertex& operator=(const Vertex & rhs)
  {
    if(this != &rhs)
    {

    }

    return *this;
  }

  Vertex(const Vertex & pV)
  {


  }
  */
  /**
   * Equivalence operator overload
   * 
   * @param Vertex rhs is a const reference to the Vertex to be compared to 
   *  this vertex
   * @return bool true if the id of this Vertex is equivalent to the id
   *  of the rhs Vertex id
   */
  bool operator==(const Vertex & rhs) const
  {
    return id == rhs.id;
  };

  /**
   * Inequivalence operator overload
   * 
   * @param Vertex rhs is a const reference to the Vertex to be compared to
   *  this vertex
   * @return bool true if the id of this Vertex is not equivalent to the id
   *  of the rhs Vertex id
   */
  bool operator !=(const Vertex & rhs) const {return !(*this == rhs); };

  /**
   * Less than operator overload for container comparators
   *
   * @param Vertex is the right hand side vertex
   */
  bool operator<(const Vertex & rhs) const { return id < rhs.id; };

  /**
   * Converts EColour enum to string
   */
  string get_color(const EColor & c)
  {
    string ret = "UNDEFINED";
    switch(c)
    {
      case eWhite: ret = "WHITE";
      case eGray:  ret = "GRAY";
      case eBlack: ret = "BLACK";
      default    :              ; //Undefined
    }

    return ret;
  };

  /**
   * Renders a vertex
   *
   * @param ostream outputstream for rendering
   * @param Vertex is the vertex to be rendered
   */
  friend ostream& operator<< (ostream & o, const Vertex & pV)
  {
    Vertex v = (Vertex) pV; //TODO
    return operator<<(o, v);
  };
  friend ostream& operator<< (ostream & o, Vertex & v)
  {
    size_t sz = v.adj->size();

    if(sz > 0)
    {
      o << v.id << " Adjacent to Verticies: ";
      AdjListIt it = v.adj->begin();
      for(size_t i =0; it != v.adj->end(); ++it, ++i)
      {
        o << (*it)->id;
        if(i+1 != sz) o << ", ";
      }
    }
    else o << "Vertex: " << v.id << " Has No Adjacent Verticies";

    o << endl;

    if(v.pi)    o << "                  pi[" << v.id << "]:" << v.pi->id;
    else        o << "                  pi[" << v.id << "]:" << "NIL";
    if(v.d < 0) o << ", d[" << v.id << "]:"  << "UNDEFINED";
    else        o << ", d[" << v.id << "]:"  << v.d;
    if(v.f < 0) o << ", f[" << v.id << "]:"  << "UNDEFINED";
    else        o << ", f[" << v.id << "]:"  << v.f;
                o << ", Color:"     << v.get_color(v.color);

    return o;
  };
};

#endif//__Vertex__