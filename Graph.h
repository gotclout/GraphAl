#ifndef __Graph__
#define __Graph__

#include "Edge.h"

/*******************************************************************************
 * Structure for representing a graph G = (V, E)
 ******************************************************************************/
struct Graph
{
  EdgeVector E;   //Collection of Edges
  VertexMap V;   //Maps a vertex to a list of adjacent verticies
  bool directed,  //indicates whether G is a directed or undirected graph
       weighted;  //
  static int gc;

  /**
   * Retrieves the number of verticies |V|
   */
  size_t vsize() { return V.size(); };

  /**
   * Retrieves the number of edges |E|
   */
  size_t esize() { return E.size(); };

  /**
   * Default construct
   *
   * @param bool pDirected is true if G is directed, false otherwise
   */
  Graph(bool pDirected = false, bool pWeighted = false)
  {
    directed = pDirected;
    weighted = pWeighted;
  };

  /**
   * Sets the parent value of all verticies to nil
   */
  void nilpi()
  {
    for(VertexMapIt i = V.begin(); i != V.end(); ++i)
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
    VertexMapIt uit, vit, beg = V.begin();
    Vertex* uvt, //u vertex ptr
           *vvt; //v vertex ptr

    uit = V.find(Vertex(u, gc));
    if(uit == V.end())
    {
      uit = V.insert(beg, VertexMapType(Vertex(u, gc), AdjList()));
      gc++;
    }
    uvt = (Vertex*)&(uit->first);

    if(!uvt->adj) uvt->adj = (AdjList*)& uit->second;

    vit = V.find(Vertex(v, gc));
    if(vit == V.end())
    {
      vit = V.insert(beg, VertexMapType(Vertex(v, gc), AdjList()));
      gc++;

    }
    vvt = (Vertex*)&(vit->first);

    if(!vvt->adj) vvt->adj = (AdjList*)& vit->second;

    uvt->add_adj(vvt);                      //add uv by default
    E.push_back(Edge(&(*uvt), &(*vvt), w));

    if(!directed)                           //add vu if undirected
    {
      vvt->add_adj(uvt);
      E.push_back(Edge(&(*vvt), &(*uvt), w));
    }
  };

  /**
   * Updates an edge flow and its reverse edge by m
   */
  void update_edge(Vertex u, Vertex v, int m)
  {
    for(size_t i = 0; i < E.size(); ++i)
    {
      if(E[i].u->id == u.id && E[i].v->id == v.id)E[i].flow += m;
      else if(E[i].u->id == v.id &&  E[i].v->id == u.id) E[i].cap -= m;
    }
  };

  /**
   * Relaxes vertex
   *
   * @param Vertex u
   * @param Vertex v
   * @param int w
   * @return bool
   */
  bool relax(Vertex & u, Vertex & v, int w)
  {
    bool ret = false;

    if(weighted)
    {
      if(u.d + w < v.d)
      {
        v.d = u.d + w;
        v.pi = &u;
        ret = true;
      }
    }

    return ret;
  }

  /**
   * Retrieves the edges connecting v to e in G
   */
  EdgePtrVector adjacent_edges(Vertex v)
  {
    EdgePtrVector ev;
    for(size_t i = 0; i < E.size(); ++i)
      if(E[i].u->id == v.id) ev.push_back(&E[i]);
    return ev;
  };

  /**
   * Retrieves the vertex adjacent to v on edge e
   *
   * @param Vertex v is the specified vertex
   * @param Edge e is the specified edge
   * @return Vertex* is the vertex adjacent to v on edge e
   */
  Vertex* adjacent_vertex(Vertex v, Edge e) {return *e.u == v ? e.v : e.u;};

  /**
   * Removes the specified edge from E in G = V
   */
  bool remove_edge(Vertex* u, Vertex* v)
  {
    Edge e(u, v);
    return remove_edge(e);
  }

  /**
   * Removes the specified edge from E in G = V
   */
  bool remove_edge(Edge & e)
  {
    bool ret = false;

    EdgeVectorIt it = E.begin();
    for( ; it != E.end() && !ret; ++it)
    {
      if(e == *it)
      {
        //remove adjacent vertices
        VertexMapIt vit = V.find(*(e.u));
        Vertex* v = (Vertex*) &vit->first;
        if(vit != V.end()) v->remove_adj(e.v);
        if(!directed)
        {
          vit = V.find(*(e.v));
          v = (Vertex*) &vit->first;
          if(vit != V.end()) v->remove_adj(e.u);
        }
        E.erase(it);
        ret = true;
      }
    }

    return ret;
  };

  /**
   * Construct and retreives G^T of G
   */
  Graph* get_transpose()
  {
    Graph* gt = new Graph(directed, weighted);

    size_t sz = E.size(), i = 0;

    for( ; i < sz; ++i)
    {

    }
    return gt;
  };

  /**
   * Retrieves edge at index i
   */
  Edge* get_edge(int i)
  {
    return i < E.size() && i > -1 ? &E[i] : 0;
  }

  Edge* get_edge(int u, int v)
  {
     for(size_t i = 0; i < E.size(); ++i)
     {
       if(E[i].u->aid == u && E[i].v->aid == v)
         return &E[i];
     }
     return 0;
  }


  /**
   * Retrieve vertex with the specified id
   */
  Vertex* get_vertex(string id)
  {
    VertexMapIt vit = V.find(Vertex(id));
    return vit != V.end() ? (Vertex*)&vit->first : 0;
  }

  Vertex* get_vertex(int i)
  {
    Vertex* ret = 0;
    VertexMapIt vmi = V.begin();
    for(; vmi != V.end() && !ret; ++vmi)
    {
      if(vmi->first.aid == i) ret = (Vertex*)&vmi->first;
    }
    return ret;
  }
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

    VertexMapIt vmi = G.V.begin();

    for(; vmi != G.V.end(); ++vmi) o << vmi->first << endl;
    o << "Rendering Edges\nNum Edges: " << G.E.size() << endl;
    for(i = 0; i < G.E.size(); ++i) o << G.E[i];
    o << "\nRendering Complete\n";

    return o;
  };
};

int Graph::gc = 0;
#endif//__Graph__

