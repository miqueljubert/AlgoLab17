#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,                 // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
                            no_property,                        // interior properties of vertices  
                            property<edge_weight_t, int>        // interior properties of edges
                            >                       Graph;
typedef graph_traits<Graph>::edge_descriptor        Edge;       // Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor      Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef graph_traits<Graph>::edge_iterator          EdgeIt;     // to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type    WeightMap;  // property map to access the interior property edge_weight_t




void ant_challenge()
{
    int n,e,s,a,b;
    cin>>n>>e>>s>>a>>b;
    vector<Graph> forest_networks(s,n);//The private network of each species
    vector<WeightMap> edge_weights(s);

    for( int i = 0; i < s; i++ ) {
        edge_weights[i] = get(edge_weight, forest_networks[i]);
    }

    int u,v,w;
    for( int i = 0; i < e; i++ ) {
        cin>>u>>v;
        for( int j = 0; j < s; j++ ) {
            cin>>w;
            Edge e; bool success;
            tie(e,success) = add_edge(u,v,forest_networks[j]);

            if( not success ) cout<<"Error adding edge"<<endl;
            edge_weights[j][e] = w;
        }
    }
    vector<Vertex> hives(s);
    int h;
    for( int i = 0; i < s; i++ ) {
        cin>>h;
        hives[i] = h;
    }
    vector<vector<Vertex>> predecessors(s,vector<Vertex>(n));
    for( int i = 0; i < s; i++ ) {
        dijkstra_shortest_paths( forest_networks[i], hives[i],
                                    predecessor_map( make_iterator_property_map( 
                                                                predecessors[i].begin(),
                                                                get(vertex_index, forest_networks[i])) 
                                                    ) );
    }
    
    //We build the new weights
    for( 
}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        ant_challenge();
    }
}

