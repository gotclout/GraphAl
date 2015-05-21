#include <iostream>

#include "DisjointSet.h"
#include "Graph.h"
#include "utils.h"

/**
 *
 */
void KRUSKAL_MST(Graph & G)
{
  DisjointSet<Vertex*> ds;
  Vertex *u = 0, *v = 0;
  vector<Edge> A;
  EdgeVector ev = G.E;                   //TODO: rm copy
  size_t i = 0, sz = G.V.size(), sum = 0;

  for(i = 0; i < sz; ++i)
    ds.MakeSet(G.get_vertex(i));

  sz = ev.size();
  aqsort(ev, 0, sz - 1);

  for(i = 0; i < sz; ++i)
  {
    u = *ds.FindSet(ev[i].u);
    v = *ds.FindSet(ev[i].v);
    if(u && v && !(*u == *v))
    {
      A.push_back(ev[i]);
      ds.Union(u, v);
    }
  }

  sz = A.size();
  for(i = 0, sum = 0; i < sz; ++i)
  {
    sum += A[i].cap;
    cout << A[i].id << ": " << A[i].cap
         << " sum: " << sum << endl;
  }
}

/**
 *
 */
int main(int argc, char** argv)
{
  /*
       B----E----G
       |\  /|    |
   G = |  C |    |
       | / \|    |
       A----D----F
  */

  Graph G(false, true);
  G.add_edge("A", "B", 2);
  G.add_edge("A", "C", 4);
  G.add_edge("A", "D", 6);
  G.add_edge("B", "C", 2);
  G.add_edge("B", "E", 6);
  G.add_edge("C", "D", 1);
  G.add_edge("C", "E", 3);
  G.add_edge("D", "F", 3);
  G.add_edge("D", "E", 2);
  G.add_edge("E", "G", 5);
  G.add_edge("F", "G", 4);

  KRUSKAL_MST(G);

  return 0;
}
