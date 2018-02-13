// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;
typedef	boost::graph_traits<Graph>::out_edge_iterator	    OutEdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};



void dfs( int i, const Graph &g, std::vector<bool> &visited, const EdgeCapacityMap &capacitymap )
{
    if( visited[i] ) return;
    visited[i] = true;
    OutEdgeIt ei,ee;
    for( boost::tie(ei,ee) = boost::out_edges(i,g); ei != ee; ++ei ) {
        if( capacitymap[*ei] == 0 ) continue;
        int next = boost::target(*ei,g);
        dfs(next,g,visited,capacitymap);
    }
}


void testcases() {
    int n,m,l,k;
    std::cin>>n>>m>>k>>l;
	Graph G(n+k+l+2);
    const int src = n+k+l;
    const int tgt = n+k+l+1;
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    
    std::vector<int> stations(k);
    for( int i = 0; i < k; i++ ) {
        std::cin>>stations[i];
	    //eaG.addEdge(stations[i], tgt, 1);//Flow from station to target
    }
    std::vector<int> photos(l);
    for( int i = 0; i < l; i++ ) {
        std::cin>>photos[i];
    }

    for( int i = 0; i < m; i++ ) {
       int a,b;
       std::cin>>a>>b;
	   eaG.addEdge(a, b, 1);
    }
    
    for( int i = 0; i < k; i++ ) {
        std::vector<bool> visited(n+k+l+2,false);
        dfs(stations[i],G,visited, capacitymap);
        for( int j = 0; j < l; j++ ) {
            if( visited[photos[j]] ) {
                eaG.addEdge(n+i,n+k+j,1);
            }
        }
    }
    for( int i = 0; i < l; i++ ) {
	    eaG.addEdge(n+k+i,photos[i], 1);
    }
    for( int i = 0; i < k; i++ ) {
	    eaG.addEdge(src,n+i, 1);
    }
    for( int i = 0; i < k; i++ ) {
	    eaG.addEdge(stations[i], tgt, 1);
    }
	long flow1 = boost::push_relabel_max_flow(G, src, tgt);
	//long flow2 = boost::edmonds_karp_max_flow(G, src, tgt);
    std::cout<<flow1<<std::endl;

    /*
	// Create Graph and Maps
	Graph G(4);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	// Add edges
	eaG.addEdge(0, 1, 1); // from, to, capacity
	eaG.addEdge(0, 3, 1);
	eaG.addEdge(2, 1, 1);
	eaG.addEdge(2, 3, 1);

	// Add source and sink
	// Careful: The names 'source' and 'target' are already used for BGL's 
	// functions to get the two endpoints of an edge, use 'src' and 'sink'
	// in case you remove the namespace (which we don't recommend).
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	eaG.addEdge(source, 0, 2);
	eaG.addEdge(source, 2, 1);
	eaG.addEdge(1, target, 2);
	eaG.addEdge(3, target, 1);

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	long flow1 = boost::push_relabel_max_flow(G, source, target);
	long flow2 = boost::edmonds_karp_max_flow(G, source, target);
	std::cout << "(push relabel max flow) " << flow1 << " == " << flow2 << " (Edmonds Karp max flow)" << std::endl;

	// Iterate over all the edges to print the flow along them
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G) 
				  << " runs " << capacitymap[*ebeg] - rescapacitymap[*ebeg]
                                  << " units of flow (negative for reverse direction)." << std::endl;
	}
    */

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) testcases();
	return 0;
}