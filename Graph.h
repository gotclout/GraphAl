#ifndef __Graph__
#define __Graph__

#include "Vertex.h"

/** Typedef for Vertex containers **/
typedef list<Vertex*>     AdjList;
typedef AdjList::iterator AdjListIt;

/**
 * TODO:
 */
struct AdjMatrix
{
  int n;

  Vertex** A;

  AdjMatrix(int n)
  {
    A = new Vertex*[n];
    for(int i = 0; i < n; ++i)
      A[i] = new Vertex[n];
  };

  Vertex* get(int i, int j){ return &A[i][j];};

  void put(int i, int j, Vertex* v)
  {
    A[i][j] = *v;
  };

  Vertex* allfrom(int i)
  {
    Vertex* r = new Vertex[n];
    for(int idx = 0; idx < n; i++)
      r[idx] = A[i][idx];

    return r;
  };
};

/*******************************************************************************
 * Structure expressing graph edges
 ******************************************************************************/
struct Edge
{
  Vertex* v1,       // start vertex
        * v2;       // end vertex
  string  id;       // uid
  int     flow,     // flow of the current edge
          cap;      // capacity of the edge

  /**
   * Default construct
   */
  Edge() { v1 = v2 = 0; flow = 0; cap = 0; id = ""; };

  /**
   * Construct from verticies
   *
   * @param Vertex* pV1 start vertex, pV2 end vertex
   * @param Vertex* pV2 end vertex
   */
  Edge(Vertex* pV1, Vertex* pV2)
  {
    v1 = &(*pV1);
    v2 = &(*pV2);
    id = v1->id + v2->id;
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
    v1 = &(*pV1);
    v2 = &(*pV2);
    cap = pCap;
    flow = 0;
    id = v1->id + v2->id;
  };

  /**
   * Less than operator overload
   *
   * @param rhs is the edge to be compared
   * @return true if the id of this edge < rhs false otherwise
   */
  bool operator<(const Edge & rhs) const
  {
    return id < rhs.id;
  };

  /**
   *
   */
  bool operator==(const Edge & rhs) const
  {
    return id == rhs.id;
  }

  /**
   *
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
    if(e.v1 && e.v2)
      o << "Edge: [" << e.v1->id << ", " << e.v2->id << "]\n";
    return o;
  };
};

/** Typedefs for Vertex Containers **/
typedef map<Vertex, AdjList>  VertexMap;
typedef VertexMap::iterator   VertexMapIt;
typedef VertexMap::value_type VertexMapType;

/** Typedefs for Edge Containers **/
typedef vector<Edge>  EdgeVector;
typedef vector<Edge*> EdgePtrVector;

/*******************************************************************************
 * Structure for representing a graph G = (V, E)
 ******************************************************************************/
struct Graph
{
  EdgeVector E; //Collection of Edges
  VertexMap VE; //Maps a vertex to a list of adjacent verticies
  bool direct;  //indicates whether G is a directed or undirected graph

  /**
   * Retrieves the number of verticies |V|
   */
  size_t vsize() { return VE.size(); };

  /**
   * Retrieves the number of edges |E|
   */
  size_t esize() { return E.size(); };

  /**
   * Default construct
   *
   * @param bool pDirected is true if G is directed, false otherwise
   */
  Graph(bool pDirected = false) { direct = pDirected; };

  /**
   * Sets the parent value of all verticies to nil
   */
  void nilpi()
  {
    for(VertexMapIt i = VE.begin(); i != VE.end(); ++i)
    {
      Vertex* vptr = (Vertex*) &i->first;
      vptr->pi = 0;
    }
  }

  /**
   * Adds an edge to this graph
   *
   * @param int u is the identifier for the first vertex
   * @param int v is the identifier for the second vertex
   */
  void add_edge(string u, string v, int w = -1)
  {
    VertexMapIt uit, vit, beg = VE.begin();
    Vertex* uvt, //u vertex ptr
           *vvt; //v vertex ptr

    uit = VE.find(Vertex(u));
    if(uit == VE.end())
      uit = VE.insert(beg, VertexMapType(Vertex(u), AdjList()));
    uvt = (Vertex*)&(uit->first);

    if(!uvt->adj) uvt->adj = (AdjList*)& uit->second;

    vit = VE.find(Vertex(v));
    if(vit == VE.end())
      vit = VE.insert(beg, VertexMapType(Vertex(v), AdjList()));
    vvt = (Vertex*)&(vit->first);

    if(!vvt->adj) vvt->adj = (AdjList*)& vit->second;

    uvt->add_adj(vvt);
    vvt->add_adj(uvt);
    E.push_back(Edge(&(*uvt), &(*vvt), w));
    E.push_back(Edge(&(*vvt), &(*uvt), 0));

    //TODO: Handle direction
  };

  /**
   * Updates an edge flow and its reverse edge by m
   */
  void update_edge(Vertex v1, Vertex v2, int m)
  {
    for(size_t i = 0; i < E.size(); ++i)
    {
      if(E[i].v1->id == v1.id && E[i].v2->id == v2.id)E[i].flow += m;
      else if(E[i].v1->id == v2.id &&  E[i].v2->id == v1.id) E[i].cap -= m;
    }
  };

   /**
    * Retrieves the edges connecting v to e in G
    */
   EdgePtrVector adjacent_edges(Vertex v)
   {
     EdgePtrVector ev;
     for(size_t i = 0; i < E.size(); ++i)
       if(E[i].v1->id == v.id) ev.push_back(&E[i]);
     return ev;
   };

  /**
   * Retrieves the vertex adjacent to v on edge e
   *
   * @param Vertex v is the specified vertex
   * @param Edge e is the specified edge
   * @return Vertex* is the vertex adjacent to v on edge e
   */
  Vertex* adjacent_vertex(Vertex v, Edge e) {return *e.v1 == v ? e.v2 : e.v1;};

  /**
   * TODO:
   */
  bool remove_edge(int u, int v)
  {
    bool ret = true;

    return ret;
  };

  /**
   * Construct and retreives G^T of G
   */
  Graph* get_transpose()
  {
    Graph* gt = new Graph(direct);

    size_t sz = E.size(), i = 0;

    for( ; i < sz; ++i)
    {

    }
    return gt;
  };

  /**
   * Renders a graph
   *
   * @param ostream is the output stream for rendering
   * @param Graph G is the graph to be rendered
   */
  friend ostream& operator << (ostream & o, Graph & G)
  {
    size_t i = 0, sz = G.vsize();

    o << "Rendering Graph...\n\n" << "Num Verticies: " << sz << endl;

    VertexMapIt vmi = G.VE.begin();

    for(; vmi != G.VE.end(); ++vmi) o << vmi->first << endl;
    o << "Rendering Edges\nNum Edges: " << G.E.size() << endl;
    for(i = 0; i < G.E.size(); ++i) o << G.E[i];
    o << "\nRendering Complete\n";

    return o;
  };
};

#endif//__Graph__
