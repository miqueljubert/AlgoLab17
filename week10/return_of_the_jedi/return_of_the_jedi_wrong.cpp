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
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, long>
		> Graph;

//typedef boost::adjacency_matrix< boost::undirectedS,
//		boost::no_property,
//		boost::property<boost::edge_weight_t, long>
//		> Graph;


typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;


void DFS_mark( const Graph &g, Vertex start, std::vector<bool> &visited ) {
    OutEdgeIt ei,ee;
    for( boost::tie(ei,ee) = boost::out_edges(start,g); ei != ee; ++ei ) {
        Vertex next = boost::target(*ei,g);
        if( not visited[next] ) {
            visited[next] = true;
            DFS_mark(g, next, visited );
        }
    }
}

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

	std::vector<Vertex> primpredmap(V);

    Graph G_min_tree(V);
	Vertex start = origin;
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)), boost::root_vertex(start));
    
    long total_weight = 0;
    //Build min-tree graph!
    for( int j = 0; j < n_vertexs; j++ ) {
        if( primpredmap[j] == j ) {
            continue;
        }

        Vertex u,v;
        u = j;
        v = primpredmap[u];

        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(u,v,G_min_tree);
        
        boost::tie(e, success) = boost::edge(u,v,G);
        total_weight += weightmap[e];

    }

    long opt_weight = LONG_MAX;
    for( int j = 0; j < n_vertexs; j++ ) {
        if( primpredmap[j] == j ) {
            continue;
        }

        Vertex u,v;
        u = j;
        v = primpredmap[u];

        Edge e; bool success;
        boost::tie(e,success) = boost::edge(u,v,G);
        long edge_weight = weightmap[e];
        boost::tie(e,success) = boost::edge(u,v,G_min_tree);

        boost::remove_edge(u,v,G_min_tree);

        std::vector<bool> visited(V,false);
        DFS_mark(G_min_tree,u,visited);
        
        //std::cout<<"Edge: "<<u<<","<<v<<std::endl;
        OutEdgeIt ei,ee;
        long min_weight = LONG_MAX;
        for( boost::tie(ei,ee) = boost::out_edges(u,G); ei != ee; ++ei ) {
            Vertex t = boost::target(*ei,G);
            //std::cout<<"trying dest edge: "<<u<<","<<t<<" visited:"<<visited[t]<<std::endl;

            if( visited[t] or t == v ) {
                //std::cout<<"ignored"<<std::endl;
                continue; //If visited, it is in the same connected component. If t == v it is the edge we want to avoid!
            }

            //if( min_weight == LONG_MAX ) min_weight = weightmap[*ei];
            min_weight = std::min(min_weight,weightmap[*ei]);
        }
        
        boost::tie(e,success) = boost::add_edge(u,v,G_min_tree);
        if(min_weight == LONG_MAX) continue;
        long resulting_weight = total_weight - edge_weight + min_weight;
        
        //if( opt_weight == LONG_MAX ) opt_weight = resulting_weight;
        opt_weight = std::min(opt_weight,resulting_weight);
    }
    std::cout<<opt_weight<<std::endl;

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;
    std::cin>>T;
	while(T--)	testcases();
	return 0;
}

