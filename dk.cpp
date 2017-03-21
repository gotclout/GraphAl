#include <iostream>
#include <string>
#include <set>
#include <queue>
#include <map>
#include <sstream>
#include <string>
#include <fstream>

#include "Graph.h"

using namespace std;

class VertexEntry
{
  public:

  Vertex* v;
  //static int idc;

  VertexEntry(const Vertex* pV)
  {
    v = (Vertex*) &(*pV);
  }
/*
  VertexEntry& operator=(const VertexEntry & rhs)
  {
    if(this != &rhs)
    {
      if(rhs.v){ v = new Vertex(*rhs.v); }
      //v = (Vertex*) &(*rhs.v);
    }
    return *this;
  }
*/
  bool operator==(const VertexEntry & rhs)
  {
    return *v == *(rhs.v);
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

//int VertexEntry::idc = 0;
bool operator==(VertexEntry lhs, VertexEntry rhs) { return *lhs.v == *rhs.v;    };
bool operator>(VertexEntry lhs, VertexEntry rhs)  { return lhs.v->d > rhs.v->d; };
bool operator<(VertexEntry lhs, VertexEntry rhs)  { return lhs.v->d < rhs.v->d; };

/*
void set_input()
{
  src = 0; tgt = 5;
  cout << "Input source city index: " << src << endl;
  cout << "Input destination city index: " << tgt << endl;
  cout << "Computing shortest path from: " << src << " to "
       << tgt << endl;
}
*/

string srcstr, tgtstr;
Graph g(false, true);
map<int, Point> cities;
int NV, NE;

/**
 *
 */
void get_input()
{
  cout << "Input source city index: ";
  cin >> srcstr;
  cout << "Input destination city index: ";
  cin >> tgtstr;
  cout << "Computing shortest path from: " << srcstr << " to "
       << tgtstr << endl;
}

/**
 *
 */
void make_graph(string input = "./doc/sample.txt")
{
  int i, u, v;

  ifstream in(input.c_str(), ios::in);

  stringstream ss;

  cout << "Generating graph from file:  " << input << endl;
  if(in.is_open())
  {
    in >> NV >> NE;

    cout << "Map contains " << NV << " verticies (cities) and "
         << NE << " edges" << endl;

    cout << "Adding cities to graph..." << endl;
    Point p;
    for(i = 0; i < NV; ++i)
    {
      in >> p.idx;
      in >> p.x;
      in >> p.y;

      cities[p.idx] = p;
    }

    cout << "Cities added to graph" << endl;
    cout << "Adding edges to graph..." << endl;
    u = v = 0;
    for(i = 0; i < NE; ++i)
    { 
      in >> u;
      in >> v;
      
      ss << u;
      Vertex vu(ss.str());
      ss.str(""); ss.clear();
      ss << v;
      Vertex vv(ss.str());
      ss.str(""); ss.clear();
      vu.p = cities[u];
      vv.p = cities[v]; 
      
      g.add_edge(vu, vv, cities[u].distance(cities[v]));
    }

    cout << "Edges added to graph" << endl;
    in.close();
  }
  else
  {
    cout << "Could not open: " << input << " for reading" << endl;
  }
}

/**
 *
 */
map< pair<string, string>, double> DIJKSTRA(Graph & g, Vertex* & src, Vertex* & tgt, map< pair<string, string>, double> pd)
{
  Vertex *v, *u;
  Edge* e;
  set<VertexEntry> s;
  float w;

  g.init(src);
  priority_queue<VertexEntry, vector<VertexEntry>, std::greater<VertexEntry> > pq;

  cout << "Dijkstra Initialized Using Min Priority Queue..." << endl;
  for(VertexMapIt i = g.VE.begin(); i != g.VE.end(); ++i)
  {
    Vertex* v = (Vertex*) &i->first;
    if(*v != *src)
    {
      v->d = INT_MAX - 10000; //TODOO FIXME INF
    }
    VertexEntry ve(v);
    pq.push(ve);
  }

  while(!pq.empty())
  {
    VertexEntry* ve = (VertexEntry*) &pq.top();
    u = ve->v;
    s.insert(*ve);
    pq.pop();
    cout << "extractmin: " << u->id << " : " << u->d << endl;
    set<VertexEntry>::iterator o = s.find(*ve);
    if(o != s.end()){
    VertexEntry ove = *o;
    float oe = o->v->d;}
    AdjListIt ait = u->adj->begin();
    for( ; ait != u->adj->end(); ++ait)
    {
      v = *ait;
      e = g.get_edge(u, v);
      w = e->cap;
      g.relax(u, v, w);/*
      if(g.relax(*u, *v, e->cap))
      {
        pair<string, string> k = make_pair(u->id, v->id);
        pd[k] = v->d;
      }*/
    }
  }

  cout << "All pairs shortest path" << endl;

  for(set<VertexEntry>::iterator vei = s.begin(); vei != s.end(); ++vei)
  {
    VertexEntry ve = *vei;
    cout << "v[" << ve.v->id << "]: " << ve.v->d << endl;
  }

  cout << "All pairs shortest path complete...rendering path" << endl;

  Vertex* vtgt = g.get_vertex(*tgt);

  if(vtgt)
  {
    cout << vtgt->id << " -> ";
    Vertex* rent = vtgt->pi;
    while(rent)
    {
      cout << rent->id ;
      rent = rent->pi;
      if(rent) cout << " -> ";
    }
    cout << endl;

    VertexEntry ve(tgt);
    set<VertexEntry>::iterator sit = s.find(ve);
    if(sit != s.end())
    {
      VertexEntry ve = *sit;
      cout << "Total Distance: " << ve.v->d << endl;
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

int main(int argc, char* argv[])
{
  string mapfile;

  if(argc == 2)
  { 
   stringstream s;

    s << argv[1];
    s >> mapfile;
    make_graph(mapfile);
  }
  else
  {
    make_graph();
  }
  get_input();
/*
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
*/
  //cout << g << endl;
  Vertex* src = g.get_vertex(srcstr);
  Vertex* tgt = g.get_vertex(tgtstr);
  map< pair<string, string>, double> r;
  DIJKSTRA(g, src, tgt, r);

  return 0;
}
