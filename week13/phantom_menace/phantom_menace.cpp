#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
using namespace std;
using namespace boost;


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


void testcases()
{
    int n,m,s,d;
    cin>>n>>m>>s>>d;

    Graph G(2*n+2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);


    int max_c = n+1;
    int src = 2*n;
    int tgt = 2*n+1;
    for( int i = 0; i < m; i++ ) {
        int u,v;
        cin>>u>>v;
        eaG.addEdge(n+u,v,max_c);
    }
    for( int i = 0; i < n; i++ ) {
        eaG.addEdge(i,n+i,1);
    }
    vector<int> origins(s);
    for( int i = 0; i < s; i++ ) {
        cin>>origins[i];
        eaG.addEdge(src,origins[i],max_c);
    }
    vector<int> destinies(d);
    for( int i = 0; i < d; i++ ) {
        cin>>destinies[i];
        eaG.addEdge(n+destinies[i],tgt,max_c);
    }


	long flow1 = boost::push_relabel_max_flow(G, src, tgt);
	//long flow2 = boost::edmonds_karp_max_flow(G, src, tgt);
	//std::cout << flow1 << " == " << flow2  << std::endl;
    cout<<flow1<<endl;

    /*
	Graph G(4);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	eaG.addEdge(0, 1, 1);
	eaG.addEdge(0, 3, 1);
	eaG.addEdge(2, 1, 1);
	eaG.addEdge(2, 3, 1);

	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	eaG.addEdge(source, 0, 2);
	eaG.addEdge(source, 2, 1);
	eaG.addEdge(1, target, 2);
	eaG.addEdge(3, target, 1);

	long flow1 = boost::push_relabel_max_flow(G, source, target);
	long flow2 = boost::edmonds_karp_max_flow(G, source, target);
	std::cout << "(push relabel max flow) " << flow1 << " == " << flow2 << " (Edmonds Karp max flow)" << std::endl;

	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
    std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)       << " runs " << capacitymap[*ebeg] - rescapacitymap[*ebeg]
       << " units of flow (negative for reverse direction)." << std::endl;
	}
    */

}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) testcases();
	return 0;
}
