#include "Graph.h"

using namespace std;

/** Time **/
static int t = 0;

/** **/
static int c = 0;

/** logging string **/
static const char* ln =
  "--------------------------------------------------------------------------------\n";
/**
 * Explores adjacent verticies tracking discovery times to detect bridges
 * if a bridge is detected it will be added to a container
 *
 * @param Vertex* u is the vertex to visit
 * @param vector<Edge> is the collection of bridges
 */
int BICONNECTED_COMP_VISIT(Vertex* & u, vector<Edge> & bridges)
{
  u->color  = eGray;
  u->d      = ++t;
  Vertex* v = 0;
  int minDu = t, minDv, children = 0;

  cout << "DFS-BRIDGE-VISIT: time:" << t << ", Visiting u: " << *u << endl;

  for(AdjListIt uit = u->adj->begin(); uit != u->adj->end(); ++uit)
  {
    v = (*uit);
    v->pi = &(*u);
    cout << "DFS-BRIDGE-VISIT: time:" << t << ", Exploring Adjacent Vertex\n"
         << "                  v: " << *v << endl;

    if(v->color == eWhite)
    {
      children++;
      minDv = BICONNECTED_COMP_VISIT(v, bridges);
      Edge bridge(u, v);
      bridges.push_back(bridge);
      //cout << "DFS-BRIDGE-VISIT: time:" << t << ", minDV:" << minDv << " ";
      //if(minDv < minDu) cout << "<";
      //else if(minDv == minDu) cout << "=";
      //else cout << ">";
      //cout << " minDU:" << minDu;

      u->l = min(u->l, v->l);
      //minDu = min(minDu, minDv);

      if( (u->d == 1 && children > 1) ||
          (u->d > 1 && v->l >= u->d))
      {
        while(bridges.back() != bridge)
        {
          cout << bridges.back() << endl;
          bridges.pop_back();
        }
        cout << bridges.back() << endl;
        bridges.pop_back();
        c++;
      }

      /*if(minDv > minDu)
      {
        cout << ", Discovered Bridge:(" << u->id << ", " << v->id << ")\n";
        Edge bridge(u, v);
        bridges.push_back(bridge);
      }
      else cout << endl;*/
    }
    else if(v != u->pi && v->d < minDu)
    {
      minDu = v->d;
      u->l = min(u->l, v->d);
      Edge bridge(u, v);
      bridges.push_back(bridge);
    }
  }

  u->color = eBlack;
  u->f = ++t;
  cout << "DFS-BRIDGE-VISIT: time:" << t << ", Finished u:" << *u << endl << ln;

  return minDu;
}

/**
 * Finds bridges in a graph
 *
 * @param Graph G is the graph for which bridges should be detected
 * @return vector<Edge> is the collection of edges that are bridges
 */
vector<Edge> BICONNECTED_COMP(Graph & G)
{
  vector<Edge> bridges;
  VertexMapIt vit = G.VE.begin();
  Vertex* u = 0;

  t = 0;
  for(; vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    u->color  = eWhite;
    u->pi     = NIL;
    cout << "DFS-BRIDGE      : time:" << t << ", "
         << "Initialized u:" << *u << endl;
  }

  cout << ln;
  t = 0;

  for(vit = G.VE.begin(); vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    cout << "DFS-BRIDGE      : time:" << t << ", ";
    if(u->color == eWhite)
    {
      cout << "Visiting u:" << *u << endl;
      BICONNECTED_COMP_VISIT(u, bridges);
    }
    else
      cout << "Not Visiting u:" <<  *u << endl;

    size_t sz = bridges.size(), j = 0;
    while(sz > 0)
    {
      j = 1;
      cout << bridges.back() << endl;
      bridges.pop_back();
      --sz;
    }
    if(j)
    {
      cout << endl;
      c++;
    }
  }

  cout << ln;

  return bridges;
}

/**
 * Renders a vector of edges
 *
 * @param vector<Edge> is the collection of edges to be rendered
 */
void PRINT_BRIDGES(const vector<Edge> & bridges)
{
  size_t i = 0, sz = bridges.size();

  if(sz == 0)
    cout << "No Bridges Found In Graph\n";
  else
  {
    cout << "Rendering " << sz << " Bridges...\n\n";
    for(; i < sz; ++i)
    {
      Edge e = bridges[i];
      cout << "Bridge " << e;
    }
  }

  cout << ln;
}

/**
 * Main: Finds bridges in a graph using DFS and DFS-VISIT patterns
 * Default graph g1 constructed in increasing edge order
 */
int main(int argc, char** argv)
{
  Graph g1;
  g1.add_edge("0","1");
  g1.add_edge("1","2");
  g1.add_edge("2","3");
  g1.add_edge("2","4");
  g1.add_edge("3","4");
  g1.add_edge("1","5");
  g1.add_edge("0","6");
  g1.add_edge("5","6");
  g1.add_edge("5","7");
  g1.add_edge("5","8");
  g1.add_edge("7","8");
  g1.add_edge("8","9");
  g1.add_edge("10","11");

  cout << g1 << ln;
  vector<Edge> bridges1 = BICONNECTED_COMP(g1);
  PRINT_BRIDGES(bridges1);

  return 0;
}
