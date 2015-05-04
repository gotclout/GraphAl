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
 * Explores adjacent verticies tracking discovery times to detect bccs
 * if a bridge is detected it will be added to a container
 *
 * @param Vertex* u is the vertex to visit
 * @param vector<Edge> is the collection of bccs
 */
int BCC_VISIT(Vertex* & u, vector<Edge> & bccs, list < vector<Edge> >& abccs)
{
  u->color    = eGray;
  u->d = u->l = ++t;
  Vertex* v   = 0;
  int minDu   = t, minDv, children = 0;

  cout << "BCC-VISIT: time:"
       << t << ", Visiting u: " << u->id << endl;

  for(AdjListIt uit = u->adj->begin(); uit != u->adj->end(); ++uit)
  {
    v = (*uit);
    cout << "BCC-VISIT: time:" << t << ", Exploring Adjacent Vertex"
         << " v: " << v->id << endl;

    if(v->color == eWhite)
    {
      children++;
      v->pi = &(*u);
      Edge edg(u, v);
      bccs.push_back(edg);
      minDv = BCC_VISIT(v, bccs, abccs);

      u->l = min(u->l, v->l);

      if( (u->d == 1 && children > 1) || (u->d > 1 && v->l >= u->d))
      {
        vector<Edge> ve;
        while(bccs.back().v1 != u || bccs.back().v2 != v)
        {
          ve.push_back(bccs.back());
          bccs.pop_back();
        }
        ve.push_back(bccs.back());
        bccs.pop_back();
        abccs.push_back(ve);
        c++;
      }
    }
    else if(v != u->pi && v->d < u->l)
    {
      minDu = v->d;
      u->l = min(u->l, v->d);
      Edge bridge(u, v);
      bccs.push_back(bridge);
    }
  }

  u->color = eBlack;
  u->f = ++t;
  cout << "BCC-VISIT: time:"
       << t << ", Finished u:" << u->id << endl << ln;

  return minDu;
}

/**
 * Finds bccs in a graph
 *
 * @param Graph G is the graph for which bccs should be detected
 * @return vector<Edge> is the collection of edges that are bccs
 */
list < vector<Edge> > BICONNECTED_COMP(Graph & G)
{
  vector<Edge> bccs;
  list< vector<Edge> > abccs;
  VertexMapIt vit = G.VE.begin();
  Vertex* u = 0;

  t = 0;
  for(; vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    u->color  = eWhite;
    u->pi     = NIL;
    cout << "BCC      : time:" << t << ", "
         << "Initialized u:" << u->id << endl;
  }

  cout << ln;
  t = 0;

  for(vit = G.VE.begin(); vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    cout << "BCC      : time:" << t << ", ";
    if(u->color == eWhite)
    {
      cout << "Visiting u:" << u->id << endl;
      BCC_VISIT(u, bccs, abccs);
    }
    else
      cout << "Not Visiting u:" <<  u->id << endl;

    size_t sz = bccs.size(), j = 0;
    vector<Edge> ev;
    while(bccs.size() > 0)
    {
      j = 1;
      ev.push_back(bccs.back());
      bccs.pop_back();
    }
    if(j)
    {
      abccs.push_back(ev);
      c++;
    }
  }

  cout << ln;

  return abccs;
}

/**
 * Renders a vector of edges
 *
 * @param vector<Edge> is the collection of edges to be rendered
 */
void PRINT_BCC(const list < vector<Edge> > & abccs)
{
  size_t i = 0, sz = abccs.size();

  if(sz == 0)
    cout << "No BCC Found In Graph\n";
  else
  {
    cout << "Rendering " << sz << " BCC...\n\n";
    list < vector<Edge> >::const_iterator ait = abccs.begin();
    for(; ait != abccs.end(); ++ait)
    {
      vector<Edge> ev = *ait;
      vector<Edge>::iterator it = ev.begin();
      cout << "BCC: " << i;
      size_t ssz = ev.size();
      for(size_t ss = 0; it != ev.end(); ++it, ++ss)
      {
        cout << (*it).v1->id << "|" << (*it).v2->id;
        if(ss + 1 == ssz) cout << endl;
        else cout << " ";
      }
    }
  }

  cout << ln;
}

/**
 * Main: Finds bccs in a graph using DFS and DFS-VISIT patterns
 * Default graph g1 constructed in increasing edge order
 */
int main(int argc, char** argv)
{
  Graph g1;
  g1.add_edge("0","1");
  g1.add_edge("1","2");
  g1.add_edge("1","3");
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

  list < vector<Edge> > abccs = BICONNECTED_COMP(g1);

  PRINT_BCC(abccs);

  cout << "count: " << c << endl;

  return 0;
}
