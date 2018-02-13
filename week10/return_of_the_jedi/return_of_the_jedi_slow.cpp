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
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
//		boost::no_property,
//		boost::property<boost::edge_weight_t, long>
//		> Graph;

typedef boost::adjacency_matrix< boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, long>
		> Graph;


typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;


void testcases() {

    int n_vertexs, origin;
    std::cin>>n_vertexs>>origin;
    origin = origin-1;

	int V = n_vertexs;
	Graph G(V);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
    
    int n_edges = 0;
	for (int i = 0; i < V; ++i) {
        for( int j = i+1; j < V; ++j ) {
            n_edges++;
            Edge e;	bool success;
            Vertex u = i;
            Vertex v = j;
            long w;
            std::cin>>w;
            //std::cout<<w<<std::endl;
            boost::tie(e, success) = boost::add_edge(u, v, G);
            weightmap[e] = w;
            assert(boost::source(e, G) == u && boost::target(e, G) == v);
        }
	}
	//std::cout << std::endl;

    //std::cout<<n_vertexs<<","<<n_edges<<std::endl;


	
	// Prim minimum spanning tree
	// ==========================
	std::vector<Vertex> primpredmap(V);
	//boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));
	Vertex start = origin;
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)), boost::root_vertex(start));
	//std::cout << "Prim builds a minimum spanning tree (of node " << start << "'s component) of total weight: ";
    
    long min_weight = -1;
    for( int j = 0; j < n_vertexs; j++ ) {
        if( primpredmap[j] == j ) {
            //std::cout<<"root"<<std::endl;
            continue;
        }

        Vertex u,v;
        u = j;
        v = primpredmap[u];

        Edge e; bool success;
        boost::tie(e, success) = boost::edge(u,v,G);
        //std::cout<<success<<std::endl;
        long removed_weight = weightmap[e];
        boost::remove_edge(u,v,G);



        //n cubed is too slow
	    std::vector<Vertex> not_primpredmap(V);
	    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(not_primpredmap.begin(), boost::get(boost::vertex_index, G)), boost::root_vertex(start));
        long totalweight = 0;
        for (int i = 0; i < V; ++i) {
            if (not_primpredmap[i] != i) {
                Edge et; bool success;
                boost::tie(et, success) = boost::edge(i, not_primpredmap[i], G);
                totalweight += weightmap[et];
            }
        }
        boost::tie(e, success) = boost::add_edge(u, v, G);
        //std::cout<<success<<std::endl;
        weightmap[e] = removed_weight;

        if( min_weight == -1 ) {
            min_weight = totalweight;
        } else {
            min_weight = std::min(min_weight,totalweight);
        }

        //std::cout << totalweight << "\n";
    }
    std::cout<<min_weight<<std::endl;;

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;
    std::cin>>T;
	while(T--)	testcases();
	return 0;
}

