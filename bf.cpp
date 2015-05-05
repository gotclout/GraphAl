#include <iostream>
#include <climits>
#include "Graph.h"

using namespace std;

/** logging string **/
static const char* ln =
  "--------------------------------------------------------------------------------\n";

/**
 *
 */
map< pair<string, string>, int> BELLMAN_FORD(Graph & G, Vertex* src, map< pair<string, string>, int> & pd)
{
  for(VertexMapIt i = G.VE.begin(); i != G.VE.end(); ++i)
  {
    Vertex *v = (Vertex*) &i->first;
    if(*v == *src)
    {
      v->d = 0;
    }
    else
    {
      v->d = INT_MAX - 1000;
    }
    cout << v->id << " " << v->d << " " << v << endl;
    //v->p = NIL;
  }

  cout << "sz: " << G.esize() << endl;
  for(size_t j = 0; j < G.vsize(); ++j)
  {
    for(size_t k = 0; k < G.esize(); ++k)
    {
      Edge* e =  G.get_edge(k);
      Vertex* u = e->u, *v = e->v;
      G.relax(*u, *v, e->cap);
      cout << u->id << " " << u->d << " : "
           << v->id << " " << v->d << " " << v  << " " << u << endl;
    }

  }
  for(size_t k = 0; k < G.esize(); ++k)
  {
    Edge* e = G.get_edge(k);

    if(e)
    {
      if(e->u->d + e->cap < e->v->d)
      {
        cerr << "ERROR: Graph contains a negative weight cycle" << endl;
      }
    }
  }

  for(VertexMapIt i = G.VE.begin(); i != G.VE.end(); ++i)
  {
     Vertex *v = (Vertex*) &i->first;
     if(*v != *src && v->pi)
     {
       pair<string, string> k = make_pair(v->id, v->pi->id);
       pd[k] = v->d;
       //pd.insert(make_pair(v->id, v->pi->id), v->d);
     }
  }

  return pd;
}

void PRINT_APSP(map< pair<string, string>, int> & apsp)
{
  cout << "Rendering APSP...\n";

  if(apsp.size() < 1)
  {
    cout << "None\n";
  }
  else
  {
    map< pair<string, string>, int>::iterator i = apsp.begin(),
                                        e = apsp.end();

    while(i != e)
    {
      cout << i->first.first << " -> " << i->first.second << " "
           << i->second << endl;
      ++i;
    }
  }
}

/**
 *
 */
int main(int argc, char** argv)
{
  Graph g(false, true);
  g.add_edge("A", "B", 2);
  g.add_edge("A", "F", 3);
  g.add_edge("B", "C", 5);
  g.add_edge("B", "E", 4);
  g.add_edge("B", "G", 3);
  g.add_edge("C", "E", 2);
  g.add_edge("C", "H", 4);
  g.add_edge("D", "F", 4);
  g.add_edge("D", "G", 2);
  g.add_edge("D", "E", 2);
  g.add_edge("E", "H", 3);
  g.add_edge("F", "G", 1);
  g.add_edge("G", "H", 1);

  map< pair<string, string>, int> pd;
  //for(VertexMapIt i = g.VE.begin(); i != g.VE.end(); ++i)
  //{
    Vertex* src = g.get_vertex("A");//(i->first.id);
    BELLMAN_FORD(g, src, pd);
  //}
    PRINT_APSP(pd);
  //cout << "A\n" << g << ln;

  return 0;
}

