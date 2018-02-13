#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::edge_iterator		EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

typedef pair<int,int> Pair;

int min_tree_cost( int node, int n,
                   vector<bool> &visited,
                   priority_queue<Pair> &prio,
                   const Graph &g,
                   const WeightMap &wm,
                   bool mst)
{
    visited[node] = true;

    OutEdgeIt ei,ee;
    for( tie(ei,ee) = out_edges(node,g); ei != ee; ++ei ) {
        Edge e = *ei;
        int u = source(e,g);
        int v = target(e,g);
        if( visited[v] ) continue;
        int w = wm[e];

        prio.push(Pair(-w,v));
    }
    if( prio.empty() ) return 0;

    Pair p = prio.top();
    prio.pop();
    while( not prio.empty() and visited[p.second] ) {
        p = prio.top();
        prio.pop();
    }
    
    if( visited[p.second] ) return 0;

    //Normal way
    //return -p.first + min_tree_cost(p.second, n, visited, prio, g, wm);
    
    //Sub-optimal choice
    vector<bool> vb1 = visited;
    priority_queue<Pair> prio1 = prio;
    int ms_val = -p.first + min_tree_cost(p.second, n, vb1, prio1, g, wm, false);
    if( mst ) return ms_val;

    //Make sub-optimal choice
    //vector<bool> vb2 = visited;
    //priority_queue<Pair> prio2 = prio;
    
    int no_ms_val = min_tree_cost( node, n, visited, prio, g, wm, true );

    return min(ms_val,no_ms_val);
 

}




void testcases()
{
    int n,t;
    cin>>n>>t;
    t-=1;

	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
    for( int i = 0; i < n; i++ ) {
        for( int j = i+1; j < n; j++ ) {
            int w;
            cin>>w;
            Edge e; bool ok;
            tie(e,ok) = add_edge(i,j,G);
            weightmap[e] = w;
            //cout<<"("<<i<<","<<j<<") "<<w<<" ";
        }
        //cout<<endl;
    }

    vector<bool> vb(n,false);
    //priority_queue<Pair,vector<Pair>,greater<Pair>> prio;
    priority_queue<Pair> prio;

    cout<<min_tree_cost(t,n,vb,prio,G,weightmap,false)<<endl;




}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) testcases();
	return 0;
}

