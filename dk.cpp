#include <iostream>
#include <string>
#include <set>
#include <queue>
#include "Graph.h"

using namespace std;

class VertexEntry
{
  public:
  Vertex* v;

  VertexEntry(const Vertex* pV)
  {
    v = (Vertex*) &(*pV);
  }

  bool operator<(const VertexEntry & rhs)
  {
    return v->d < rhs.v->d;
  }

  bool operator>(const VertexEntry & rhs)
  {
    //return !(*this < rhs);
    return v->d > rhs.v->d;
  }

};

bool operator>(VertexEntry lhs, VertexEntry rhs) {return lhs.v->d > rhs.v->d;};
bool operator<(VertexEntry lhs, VertexEntry rhs) {return lhs.v->d < rhs.v->d;};

map< pair<string, string>, double> DIJKSTRA(Graph & g, Vertex* & src, map< pair<string, string>, double> pd)
{
  Vertex *v, *u;
  Edge* e;
  set<VertexEntry> s;

  g.init(src);

  //priority_queue<VertexEntry, vector<VertexEntry>, std::greater<VertexEntry> > pq;
  priority_queue<VertexEntry> pq;

  for(VertexMapIt i = g.VE.begin(); i != g.VE.end(); ++i)
  {
    Vertex* v = (Vertex*) &i->first;
    if(*v == *src)
    {
      continue;
    }
    else
    {
      v->d = INT_MAX - 1000;
    }
    VertexEntry ve(v);
    pq.push(ve);
    cout << *(ve.v) << endl;
  }

  while(!pq.empty())
  {
    //extract-min
    VertexEntry ve = pq.top();
    u = ve.v;
    //u = &(*pq.top().v);
    pq.pop();
    s.insert(u);
    EdgePtrVector E = g.adjacent_edges(*u);
    for(size_t i = 0; i < E.size(); ++i)
    {
      e = E[i];
      v = g.adjacent_vertex(*u, *e);
      g.relax(*u, *v, e->cap);
    }
  }

  for(VertexMapIt i = g.VE.begin(); i != g.VE.end(); ++i)
  {
     Vertex *v = (Vertex*) &i->first;
     cout << src->id << " : " << v->id << endl;
     if(*v != *src && v->pi)
     {
       pair<string, string> k = make_pair(v->id, v->pi->id);
       pd[k] = v->d;
     }
  }

  return pd;
}

void PRINT_APSP(map< pair<string, string>, double> & apsp)
{
  cout << "Rendering APSP...\n";

  if(apsp.size() < 1)
  {
    cout << "None\n";
  }
  else
  {
    map< pair<string, string>, double>::iterator i = apsp.begin(),
                                        e = apsp.end();

    while(i != e)
    {
      cout << i->first.first << " -> " << i->first.second << " "
           << i->second << endl;
      ++i;
    }
  }
}

int main()
{

  Graph g(false, true);

  g.add_edge("0", "1", 4);
  g.add_edge("0", "7", 8);
  g.add_edge("1", "2", 8);
  g.add_edge("1", "7", 11);
  g.add_edge("2", "3", 7);
  g.add_edge("2", "8", 2);
  g.add_edge("2", "5", 4);
  g.add_edge("3", "4", 9);
  g.add_edge("3", "5", 14);
  g.add_edge("4", "5", 10);
  g.add_edge("5", "6", 2);
  g.add_edge("6", "7", 1);
  g.add_edge("6", "8", 6);
  g.add_edge("7", "8", 7);

  cout << g << endl;
  Vertex* src = g.get_vertex("0");
  map< pair<string, string>, double> r;
  DIJKSTRA(g, src, r);
  PRINT_APSP(r);

  return 0;
}
