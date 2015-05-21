#ifndef __Edge__
#define __Edge__

#include "Vertex.h"
/*******************************************************************************
 * Structure expressing an Adjacency Matrix for a graph G TODO:
 ******************************************************************************/
struct AdjMatrix
{
  int n;      //
  Vertex** A; //

  /**
   * Default Constructor
   */
  AdjMatrix(int n)
  {
    A = new Vertex*[n];
    for(int i = 0; i < n; ++i)
      A[i] = new Vertex[n];
  };

  /**
   * Retrieves the vertex at A[i][j]
   */
  Vertex* get(int i, int j){ return &A[i][j];};

  void put(int i, int j, Vertex* v)
  {
    A[i][j] = *v;
  };

  /**
   * Retrieves the verticies at A[i]
   */
  Vertex* allfrom(int i)
  {
    Vertex* r = new Vertex[n];
    for(int idx = 0; idx < n; i++)
      r[idx] = A[i][idx];

    return r;
  };

  /**
   * Destructor
   */
  ~AdjMatrix()
  {
    if(A)
    {
      for(int i = 0; i < n; ++i)
      {
        if(A[i]) delete [] A[i];
      }
      delete [] A;
    }
  }
};

/** Typedef for Adjacency containers **/
typedef list<Vertex*>     AdjList;
typedef AdjList::iterator AdjListIt;

/*******************************************************************************
 * Structure expressing graph edges
 ******************************************************************************/
class Edge
{
  private:

  protected:

  public:

  Vertex* u,       // start vertex
        * v;       // end vertex
  string  id;       // uid
  int     flow,     // flow of the current edge
          cap;      // capacity of the edge

  /**
   * Default construct
   */
  Edge() { u = v = 0; flow = 0; cap = 0; id = ""; };

  /**
   * Construct from verticies
   *
   * @param Vertex* pV1 start vertex, pV2 end vertex
   * @param Vertex* pV2 end vertex
   */
  Edge(Vertex* & pV1, Vertex* & pV2)
  {
    u = &(*pV1);
    v = &(*pV2);
    id = u->id + v->id;
    flow = cap = 0;
  };

  /**
   * Construct from vertex pointer and value
   *
   * @param Vertex* pV1 is the source vertex of edge e
   * @param Vertex* pV2 is the destination vertex of edge e
   * @param int pCap is the capacity of edge e
   */
  Edge(Vertex* pV1, Vertex* pV2, int pCap)
  {
    u = &(*pV1);
    v = &(*pV2);
    cap = pCap;
    flow = 0;
    id = u->id + v->id;
  };

  /**
   * Less than operator overload
   *
   * @param rhs is the edge to be compared
   * @return true if the id of this edge < rhs false otherwise
   */
  bool operator<(const Edge & rhs) const
  {
    if(rhs.cap < 0 || this->cap > 0 ) return cap < rhs.cap;
    return id < rhs.id;
  };

  /**
   *
   */
  bool operator>(const Edge & rhs) const
  {
    if(rhs.cap > 0 || this->cap > 0) return cap > rhs.cap;
    return id < rhs.id;
  };

  /**
   * Equivalence operator
   */
  bool operator==(const Edge & rhs) const
  {
    return id == rhs.id;
  }

  /**
   * Inequality operator
   */
  bool operator!=(const Edge & rhs) const
  {
    return id != rhs.id;
  }

  /**
   * Retrieves the residual value of the edge
   *
   * @return int is the value of capacity - flow
   */
  int residual() { return cap - flow; };

  /**
   * Renders an edge
   */
  friend ostream& operator <<(ostream & o, Edge & e)
  {
    if(e.u && e.v)
      o << "Edge: [" << e.u->id << ", " << e.v->id << "]\n";
    return o;
  };
};

class WeightedEdge : Edge
{

};

class NetworkEdge : Edge
{

};

/** Typedefs for Vertex Containers **/
typedef map<Vertex, AdjList>  VertexMap;
typedef VertexMap::iterator   VertexMapIt;
typedef VertexMap::value_type VertexMapType;

/** Typedefs for Edge Containers **/
typedef vector<Edge>  EdgeVector;
typedef EdgeVector::iterator EdgeVectorIt;
typedef vector<Edge*> EdgePtrVector;
typedef EdgePtrVector::iterator EdgePtrVectorIt;

#endif//__Edge__
