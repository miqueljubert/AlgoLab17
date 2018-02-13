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


int go_shopping()
{

    Graph G(0);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    
    int nvertices,nedges,nstores;
    std::cin>>nvertices>>nedges>>nstores;
    std::vector<int> stores(nstores);
    for( int i = 0; i < nstores; i++ ) {
        std::cin>>stores[i];
    }

    for( int i = 0; i < nedges; i++ ) {
        int i1,i2;
        std::cin>>i1>>i2;
        eaG.addEdge(i1,i2,1);
        eaG.addEdge(i2,i1,1);//Will it work correctly?
    }
    Vertex s = 0;
    Vertex t = nvertices;
    for( int i = 0; i < nstores; i++ ) {
        eaG.addEdge(stores[i],t,1);
    }

    long flow1 = boost::push_relabel_max_flow(G, s, t);
    //std::cout<<flow1<<"-"<<nstores<<std::endl;
    //std::cout<<(flow1==nstores)<<std::endl;
    if(flow1==nstores) std::cout<<"yes"<<std::endl;
    else std::cout<<"no"<<std::endl;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    int ntests;
    std::cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        go_shopping();
    }
}












