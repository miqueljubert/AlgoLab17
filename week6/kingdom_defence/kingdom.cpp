#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <utility>
#include <limits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
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









void defend_kingdom()
{
    int nlocations,npaths;
    std::cin>>nlocations>>npaths;
    
    std::vector<int> soldier_supply(nlocations);
    std::vector<int> initial_supply(nlocations);
    //std::vector<int> soldier_demand(nlocations);
    for( int i = 0; i < nlocations; i++ ) {
        int supply,demand;
        std::cin>>supply>>demand;
        soldier_supply[i] = supply-demand;
        initial_supply[i] = supply;
        //soldier_demand[i] = demand;
    }
    std::vector<std::pair<int,int>> paths(npaths);
    std::vector<std::pair<int,int>> capacities(npaths);
    for( int i = 0; i < npaths; i++ ) {
        int u,v,min_cap,max_cap;
        std::cin>>u>>v>>min_cap>>max_cap;
        paths[i] = std::pair<int,int>(u,v);
        capacities[i] = std::pair<int,int>(min_cap,max_cap);
    }

    Graph G(2*nlocations+2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    int s = 2*nlocations;
    int t = 2*nlocations+1;
   

    long reqflow = 0;
    for( int i = 0; i < npaths; i++ ) {
        int u,v,min_cap,max_cap;
        tie(u,v) = paths[i];
        tie(min_cap,max_cap) = capacities[i];
        if( u == v ) {
            int supply = initial_supply[u];
            if( min_cap > abs(supply) ) {
                eaG.addEdge(2*u, t, min_cap-supply);
                eaG.addEdge(s,2*u+1,min_cap-supply);
                reqflow += min_cap-supply;
            }
            //std::cout<<max_cap<<" "<<min_cap<<std::endl;
        }
    }





    //Push demand forward on edges
    for( int i = 0; i < npaths; i++ ) {
        int u,v,min_cap,max_cap;
        tie(u,v) = paths[i];
        tie(min_cap,max_cap) = capacities[i];
        if( u != v ) {
            soldier_supply[u]-=min_cap;
            soldier_supply[v]+=min_cap;
        }
    }
    
    //build the edges between nodes
    for( int i = 0; i < npaths; i++ ) {
        int u,v,min_cap,max_cap;
        tie(u,v) = paths[i];
        tie(min_cap,max_cap) = capacities[i];
        if( u != v ) {
            eaG.addEdge(2*u+1, 2*v, max_cap-min_cap);
            //std::cout<<max_cap<<" "<<min_cap<<std::endl;
        }
    }
    
    //Node between the two edges
    for( int i = 0; i < nlocations; i++ ) {
        eaG.addEdge(2*i, 2*i+1, std::numeric_limits<long>::max());
    }
    //Supply and demand edges
    for( int i = 0; i < nlocations; i++ ) {
        int supply = soldier_supply[i];
        if( supply == 0 ) continue;
        if( supply < 0 ) { //Need soldiers
            eaG.addEdge(2*i+1, t, abs(supply) );
            reqflow+=abs(supply);
        }
        if( supply > 0 ) { //Can supply soldiers
            eaG.addEdge(s, 2*i, abs(supply) );
        }
    }
    
    long flow1 = boost::push_relabel_max_flow(G, s, t);
    //std::cout<<flow1<<"-"<<reqflow<<std::endl;
    if( flow1 == reqflow ) std::cout<<"yes"<<std::endl;
    else std::cout<<"no"<<std::endl;

}






int main() {
	std::ios_base::sync_with_stdio(false);
	int ntests;
    std::cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        defend_kingdom();
    }
}
