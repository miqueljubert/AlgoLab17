// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Main
int get_satellites() {

    int n_ground,n_satellites,n_links;
    std::cin>>n_ground>>n_satellites>>n_links;

	// build Graph
	const int N = n_ground+n_satellites+2;
	Graph G(N);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    
	Vertex src = n_ground+n_satellites;
	Vertex sink = n_ground+n_satellites+1;
    
    for( int i = 0; i < n_links; i++ ) {
        int g,s;
        std::cin>>g>>s;
        s += n_ground;
        eaG.addEdge(g,s,1);
    }

    for( int i = 0; i < n_ground; i++ ) {
        eaG.addEdge(src, i,1);
    }
    for( int i = n_ground; i < n_ground+n_satellites; i++ ) {
        eaG.addEdge(i,sink,1);
    }

	// add edges
    /*
	eaG.addEdge(src, 1, 5);
	eaG.addEdge(1, 2, 3);
	eaG.addEdge(1, 3, 2);
	eaG.addEdge(2, 3, 1);
	eaG.addEdge(2, 4, 2);
	eaG.addEdge(3, 4, 2);
	eaG.addEdge(4, sink, 4);
    */

	// Find a min cut via maxflow
	int flow = boost::push_relabel_max_flow(G, src, sink);
	//std::cout << "maximum flow = minimum cut = " << flow << std::endl;

	// BFS to find vertex set S
	std::vector<int> vis(N, false); // visited flags
	std::vector<int> set(N, -1); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[src] = true; // Mark the source as visited
    int group = 0;
	Q.push(src);
    //Q.push(-1);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
        /*
        if( u == -1 ){ 
            group = (group+1)%2;
            continue;
        }
        */
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
            set[v] = group;
			Q.push(v);
		}
        //Q.push(-1);
	}
    /*
	// Output S
    std::cout<<"groud"<<std::endl;
	for (int i = 0; i < n_ground; ++i) {
		//if (vis[i]) std::cout << i;
        std::cout<<i<<" "<<vis[i]<<std::endl;
	}
    std::cout<<std::endl;
    std::cout<<"air"<<std::endl;
	for (int i = n_ground; i < n_ground+n_satellites; ++i) {
		//if (vis[i]) std::cout << i-n_ground;
        std::cout<<i-n_ground<<" "<<not vis[i]<<std::endl;
	}
	std::cout << std::endl;
    */
    int g1 = 0;
    int g2 = 0;
    for (int i = 0; i < n_ground; ++i) {
		//if (vis[i]) std::cout << i;
        if( vis[i] )    g1++;
        else            g2++;
        //std::cout<<i<<" "<<vis[i]<<std::endl;
	}
    int s1 = 0;
    int s2 = 0;
	for (int i = n_ground; i < n_ground+n_satellites; ++i) {
		//if (vis[i]) std::cout << i-n_ground;
        //std::cout<<i-n_ground<<" "<<not vis[i]<<std::endl;
        if( not vis[i] ) s1++;
        else             s2++;
	}

    if( g1+s1 <= g2+s2 ) {
        std::cout<<g1<<" "<<s1<<std::endl;
        for( int i = 0; i < n_ground; ++i ) {
            if (vis[i]) std::cout << i <<" ";
        }
        for (int i = n_ground; i < n_ground+n_satellites; ++i) {
            if (not vis[i]) std::cout << i - n_ground<<" ";
        }
        std::cout<<std::endl;
    } else {
        std::cout<<g2<<" "<<s2<<std::endl;
        for( int i = 0; i < n_ground; ++i ) {
            if (not vis[i]) std::cout << i<<" ";
        }
        for (int i = n_ground; i < n_ground+n_satellites; ++i) {
            if (vis[i]) std::cout << i - n_ground<<" ";
        }
        std::cout<<std::endl;
    }

	return 0;
}

int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ){
        get_satellites();
    }
}
