#include <iostream>
#include <sstream>
#include <climits>
#include "Graph.h"

using namespace std;

/** logging string **/
static const char* ln =
"--------------------------------------------------------------------------------\n";

void PRINT_APSP(int** & dv, Graph & g, stringstream & ss)
{

  size_t n = g.vsize();
  for(size_t i = 0, k = 2; i < n; ++i, k+=2)
  {
//    cout << g.get_edge(i)->u->id << " ";
    if(i == 0)
    cout << ss.str().c_str()[2] << " ";
    else
    cout << ss.str().c_str()[k] << " ";
    for(size_t j = 0; j < n; ++j)
    {
      cout << dv[i][j] << " ";
    }
    cout << endl;
  }
}

void FLOYD_WARSHALL(Graph & g, int** & dv)
{
  int n = g.vsize();
  stringstream ss, sconv;
  ss << "  ";
  for(int i = 0; i < n; ++i)
  {
    for(int j = 0; j < n; ++j)
    {
      dv[i][j] = INT_MAX / 2 - 1;
    }
  }

  VertexMapIt it = g.VE.begin();
  for(size_t i = 0; it != g.VE.end(); ++it, ++i)
  {
    dv[it->first.aid][it->first.aid] = 0;
    ss << it->first.id << " ";
    ss << it->first.aid << " ";
  }

  ss << endl;
  for(size_t k = 0; k < g.esize(); ++k)
  {
    Edge* e =  g.get_edge(k);
    e->v->l = 0;
    Vertex* u = e->u, *v = e->v;

    dv[u->aid][v->aid] = e->cap;
  }

  int nv;
  for(int k = 0; k < n; ++k)
  {
    for(int i = 0; i < n; ++i)
    {
      string si,
             sk;
      sconv << i; sconv >> si;
      sconv.str(""); sconv.clear();
      sconv << k; sconv >> sk;
      sconv.str(""); sconv.clear();
      Edge* e = g.get_edge(si, sk);
      if(e) e->v->l = dv[i][k];
      for(int j = 0; j < n; ++j)
      {
        nv = dv[i][k]+ dv[k][j];
        if( nv < dv[i][j])
        {
          dv[i][j] = nv;
          string sj;
          sconv << j; sconv >> sj;
          sconv.str(""); sconv.clear();
          Edge* ee = g.get_edge(sk, sj);
          if(e && ee && j != i)
          {
            e->v->l = nv;
            if(e->v->pi) e->v->pi->l += dv[i][k];
            ee->v->pi = e->v; //ee->v->->l = nv ;
          }
        }
      }
    }
  }
  cout << "Rendering APSP...\n" << ss.str();
  PRINT_APSP(dv, g, ss);
  it = g.VE.begin();
  for(size_t i = 0; it != g.VE.end(); ++it, ++i)
  {
    bool f = 1;
    Vertex* tpi = it->first.pi;
    while(tpi)
    {
      if(f)
      {
        cout << it->first.id << " -> " << tpi->id;
        f = 0;
      }
      else
      {

         cout << " -> " << tpi->id;
        if(tpi->pi->pi->id ==
            tpi->id)
        {
          cout << " : " << tpi->pi->l;
          break;
        }
      }
      tpi = tpi->pi;
    }
    if(!f) cout << endl;
  }
  cout << g;
}

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
  int** dv = new int*[g.vsize()];
  for(int i = 0; i < g.vsize(); ++i)
    dv[i] = new int[g.vsize()];


  FLOYD_WARSHALL(g, dv);
  return 0;
}
