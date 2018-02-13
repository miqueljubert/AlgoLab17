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
#include <unordered_set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::allow_parallel_edge_tag,
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
	Edge addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
        return e;
	}
};


// Main
int cut_sculptures() {
   int n_sculptures, n_limbs; 
   std::cin>>n_sculptures>>n_limbs;

	// Create Graph and Maps
	Graph G(n_sculptures + 2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    for( int i = 0; i < n_limbs; i++ ) {
        long a,b,c;
        std::cin>>a>>b>>c;
        eaG.addEdge(a,b,c);
    }
    int src = n_sculptures;
    int snk = n_sculptures+1;

    long max_cap = std::numeric_limits<long>::max();

    Edge e1,e2,re1,re2;
    e1 = eaG.addEdge(src,0,max_cap);
    re1 = revedgemap[e1];
    
    long optimal_cost = max_cap;
    long optimal_sink = -1;
    long optimal_source = -1;
    
    for( int i = 1; i < n_sculptures; i++ ) {

        e2 = eaG.addEdge(i,snk,max_cap);
        re2 = revedgemap[e2];

	    long flow = boost::push_relabel_max_flow(G, src, snk);

        //Get the visited flag
        std::vector<int> vis(n_sculptures+2, false); // visited flags
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[src] = true; // Mark the source as visited
        Q.push(src);
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            OutEdgeIt ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
                const int v = boost::target(*ebeg, G);
                // Only follow edges with spare capacity
                if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
                vis[v] = true;
                Q.push(v);
            }
        }
        /*
        for( int i = 0; i < n_sculptures; i++ ) {
            if( not vis[i] ) tried[i] = true;
        }
        */
        if( flow < optimal_cost ) {
            optimal_cost = flow;
            optimal_sink = i;
            optimal_source = 0;
        }
        remove_edge(e2,G);
        remove_edge(re2,G);

    }

    remove_edge(e1,G);
    remove_edge(re1,G);
    e2 = eaG.addEdge(0,snk,max_cap);
    re2 = revedgemap[e2];
    
    for( int i = 1; i < n_sculptures; i++ ) {

        e1 = eaG.addEdge(src,i,max_cap);
        re1 = revedgemap[e1];

	    long flow = boost::push_relabel_max_flow(G, src, snk);

        //Get the visited flag
        std::vector<int> vis(n_sculptures+2, false); // visited flags
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[src] = true; // Mark the source as visited
        Q.push(src);
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            OutEdgeIt ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
                const int v = boost::target(*ebeg, G);
                // Only follow edges with spare capacity
                if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
                vis[v] = true;
                Q.push(v);
            }
        } /*
        for( int i = 0; i < n_sculptures; i++ ) {
            if( not vis[i] ) tried[i] = true;
        }
        */
        if( flow < optimal_cost ) {
            optimal_cost = flow;
            optimal_sink = 0;
            optimal_source = i;
        }
        remove_edge(e1,G);
        remove_edge(re1,G);

    }
    remove_edge(e2,G);
    remove_edge(re2,G);


    //std::cout<<optimal_cost<<std::endl;
    //std::cout<<"src:"<<src<<" optimal_source:"<<optimal_source;
    //std::cout<<"sink:"<<snk<<" optimal_sink:"<<optimal_sink<<std::endl;
    e1 = eaG.addEdge(src,optimal_source,max_cap);
    e2 = eaG.addEdge(optimal_sink,snk,max_cap);
    re1 = revedgemap[e1];
    re2 = revedgemap[e1];

    long flow = boost::push_relabel_max_flow(G, src, snk);

    //Get the visited flag
    std::vector<int> vis(n_sculptures+2, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[src] = true; // Mark the source as visited
    Q.push(src);

    int ntaken = 0;
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            Q.push(v);
            ntaken++;
        }
    } 

    std::cout<<flow<<std::endl;
    std::cout<<ntaken<<" ";
    for( int i = 0; i < n_sculptures; i++ ) {
        if( vis[i] ) std::cout<<i<<" ";
    }
    std::cout<<std::endl;

    /*
	// BFS to find vertex set S
	std::vector<int> vis(N, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[src] = true; // Mark the source as visited
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	// Output S
	for (int i = 0; i < N; ++i) {
		if (vis[i]) std::cout << i << " ";
	}
	std::cout << std::endl;
    */
	return 0;
}

int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        cut_sculptures();
    }
}









