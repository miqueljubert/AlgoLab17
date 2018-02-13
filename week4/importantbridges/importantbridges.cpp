// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
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
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_name_t, int> 
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_name_t>::type	ComponentMap;	// property map to access the interior property edge_weight_t


// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
    int V,E;
    std::cin>>V>>E;
	Graph G(V);

	for (int i = 0; i < E; ++i) {
		Edge e;	bool success;
        Vertex u,v;
        std::cin>>u>>v;
		boost::tie(e, success) = boost::add_edge(u, v, G);
	}

    ComponentMap componentmap = boost::get(boost::edge_name, G);
	int ncc = boost::biconnected_components(G, componentmap ); 
    
    std::vector<int> component_edges(ncc,0);
    EdgeIt ei,ee;
    for( boost::tie(ei,ee) = edges(G); ei != ee; ++ei ) {
        Edge e = *ei;
        int c = componentmap[e];
        //std::cout<<"Edge: "<<e<<" component: "<<c<<std::endl;
        component_edges[c]++;
    }
    
    std::vector<std::pair<int,int>> bridges;
    //EdgeIt ei,ee;
    for( boost::tie(ei,ee) = edges(G); ei != ee; ++ei ) {
        Edge e = *ei;
        int c = componentmap[e];
        if( component_edges[c] == 1 ) {
            int u,v;
            u = boost::source(e,G);
            v = boost::target(e,G);
            if( u > v ) {
                int temp = u;
                u = v;
                v = temp;
            }
            bridges.push_back(std::pair<int,int>(u,v));
        }
        
    }
    std::sort(bridges.begin(),bridges.end());
    std::cout<<bridges.size()<<std::endl;
    for( int i = 0; i < bridges.size(); i++ ) std::cout<<bridges[i].first<<" "<<bridges[i].second<<std::endl;

    //std::cout<<std::endl;
}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;
    std::cin>>T;
	while(T--)	testcases();
	return 0;
}

