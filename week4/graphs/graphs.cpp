#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS,         // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
                        no_property,                    // interior properties of vertices  
                        property<edge_weight_t, int>    // interior properties of edges
                        >                           Graph;
typedef graph_traits<Graph>::edge_descriptor        Edge;       // Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor      Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef graph_traits<Graph>::edge_iterator          EdgeIt;     // to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type    WeightMap;  // property map to access the interior property edge_weight_t






void do_computation()
{
    int nvertices,nedges;
    cin>>nvertices>>nedges;
    
    Graph G(nvertices);
    WeightMap weightmap = get(edge_weight, G);

    int u,v,w;
    for( int i = 0; i < nedges; i++ ) {
        cin>>u>>v>>w;
        Edge e; bool success;
        tie(e, success) = add_edge(u,v,G);
        weightmap[e] = w;
    }

    Vertex start = 0;
    vector<Vertex> predmap(nvertices);
    prim_minimum_spanning_tree(G, make_iterator_property_map(predmap.begin(), get(vertex_index,G)), root_vertex(start));
    double total_weight = 0.;
    for( int i = 0; i < nvertices; i++ ) {
        Edge e; bool success;
        tie(e,success) = edge(i, predmap[i], G);
        if(success) total_weight += weightmap[e];
    }
    cout<<total_weight<<" ";

    vector<int> distmap(nvertices);
    dijkstra_shortest_paths(G, start, distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))) );
    int maxpath = -1;
    for( int i = 0; i < nvertices; i++  ) {
        maxpath = max(maxpath,distmap[i]);
    }
    cout<<maxpath<<endl;


}


int main()
{
   int ntests;
   cin>>ntests;
   for( int i = 0; i < ntests; i++ ) {
        do_computation();
   }
}
