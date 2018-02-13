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

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>


using namespace std;
using namespace boost;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
		boost::no_property,		
		boost::property<boost::edge_weight_t, int>
		>					GraphD;
typedef boost::graph_traits<GraphD>::edge_descriptor		EdgeD;
typedef boost::graph_traits<GraphD>::vertex_descriptor		VertexD;
typedef boost::graph_traits<GraphD>::edge_iterator		EdgeItD;	
typedef boost::graph_traits<GraphD>::out_edge_iterator		OutEdgeItD;
typedef boost::property_map<GraphD, boost::edge_weight_t>::type	WeightMapD;


typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>>	Graph;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;
//typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;



class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

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


void testcases() {

    int n,m,a,s,c,d;
    cin>>n>>m>>a>>s>>c>>d;

	GraphD GD(n);
	WeightMapD weightmap = boost::get(boost::edge_weight, GD);



    for( int i = 0; i < m; i++ ) {
        int x,y,z;
        char w;
        cin>>w>>x>>y>>z;
        //eaG.addEdge(x,y,z,1);
        if( w == 'S' ) {
            EdgeD e; bool b;
            tie(e,b) = add_edge(x,y,GD);
            weightmap[e] = z;
        } else {
            EdgeD e; bool b;
            tie(e,b) = add_edge(x,y,GD);
            weightmap[e] = z;
            tie(e,b) = add_edge(y,x,GD);
            weightmap[e] = z;

        }
    }
    //cerr<<"a"<<endl;

    vector<int> agents(a);
    for( int i = 0; i < a; i++ ) {
        cin>>agents[i];
    }
    vector<int> shelters(s);
    for( int i = 0; i < s; i++ ) {
        cin>>shelters[i];
    }
    //cerr<<"b"<<endl;


    vector<bool> checked(n,false);
    vector<vector<int>> agent_dist(a,vector<int>(n,-1));
    for( int i = 0; i < a; i++ ) {
        //if( checked[agents[i]] ) continue;
        //vector<int> pred(n);
	    boost::dijkstra_shortest_paths(GD, agents[i],
            distance_map(boost::make_iterator_property_map(agent_dist[i].begin(),
            boost::get(boost::vertex_index, GD))));
        //checked[agents[i]] = true;
        //agent_dist[i] = pred;
        
    }

    int max_time = 1000+5000*10000+1;
    
    long opt_time = 0;
    int start = d;
    int end = max_time;
    int valid = -1;
    while( start <= end ) {
    //for( int time = 0; time <= max_time; time++ ) {
        
        int current = (start+end)/2;
        Graph Gf(a+2*s+2);
        const int src = a+2*s;
        const int tgt = a+2*s+1;
        EdgeCapacityMap capacitymapf = boost::get(boost::edge_capacity, Gf);
        ReverseEdgeMap revedgemapf = boost::get(boost::edge_reverse, Gf);
        ResidualCapacityMap rescapacitymapf = boost::get(boost::edge_residual_capacity, Gf);
        EdgeAdder eaGf(Gf, capacitymapf, revedgemapf);

        for( int i = 0; i < a; i++ ) eaGf.addEdge(src,i,1);
        for( int j = 0; j < 2*s; j++ ) eaGf.addEdge(j+a,tgt,1);

        for( int i = 0; i < a; i++ ) {
            for( int j = 0; j < s; j++ ) {
                long dist = agent_dist[i][shelters[j]];
                if( dist <= current-d ) {
                    eaGf.addEdge(i,j+a,1);
                }
                if( c == 2 and dist <= current-2*d) {
                    eaGf.addEdge(i,j+a+s,1);
                }
            }
        }
	    long flow1 = boost::push_relabel_max_flow(Gf, src, tgt);
        //if( flow1 == a ) {
        //    cout<<time<<endl;
        //    return;
        //}
        if( flow1 == a ) {
            valid = current;
            end = current-1;
        } else {
            start = current+1;
        }

    }
    cout<<valid<<endl;
    //cout<<"n"<<endl;

	//cout<<endl;
    





    //cerr<<"c"<<endl;

	//Graph G(4);
	//EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	//ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	//ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	//EdgeAdder eaG(G, capacitymap, revedgemap);

	//// Add edges
	//eaG.addEdge(0, 1, 1); // from, to, capacity
	//eaG.addEdge(0, 3, 1);
	//eaG.addEdge(2, 1, 1);
	//eaG.addEdge(2, 3, 1);

	//// Add source and sink
	//// Careful: The names 'source' and 'target' are already used for BGL's 
	//// functions to get the two endpoints of an edge, use 'src' and 'sink'
	//// in case you remove the namespace (which we don't recommend).
	//Vertex source = boost::add_vertex(G);
	//Vertex target = boost::add_vertex(G);
	//eaG.addEdge(source, 0, 2);
	//eaG.addEdge(source, 2, 1);
	//eaG.addEdge(1, target, 2);
	//eaG.addEdge(3, target, 1);

	//// Calculate flow
	//// If not called otherwise, the flow algorithm uses the interior properties
	//// - edge_capacity, edge_reverse (read access),
	//// - edge_residual_capacity (read and write access).
	//long flow1 = boost::push_relabel_max_flow(G, source, target);
	//long flow2 = boost::edmonds_karp_max_flow(G, source, target);
	//std::cout << "(push relabel max flow) " << flow1 << " == " << flow2 << " (Edmonds Karp max flow)" << std::endl;

	//// Iterate over all the edges to print the flow along them
	//EdgeIt ebeg, eend;
	//for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
	//	std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G) 
	//			  << " runs " << capacitymap[*ebeg] - rescapacitymap[*ebeg]
    //                              << " units of flow (negative for reverse direction)." << std::endl;
	//}

}



    /*
	std::vector<std::vector<int> > graphedges = {{0,2,14},{0,3,12},{1,2,19},{1,3,15},{2,3,13},{4,5,17},{4,6,16},{5,6,18}};
	int E = graphedges.size();
	for (int i = 0; i < E; ++i) {
		Edge e;	bool success;
		Vertex u = graphedges[i][0];
		Vertex v = graphedges[i][1];
		int w = graphedges[i][2];
		boost::tie(e, success) = boost::add_edge(u, v, G);
		weightmap[e] = w;
		assert(boost::source(e, G) == u && boost::target(e, G) == v);	
	}
	std::cout << std::endl;



	std::vector<Vertex> predmap(V);
	std::vector<int> distmap(V);
	Vertex start = 0;
	boost::dijkstra_shortest_paths(G, start,
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
        */

int main() {
	std::ios_base::sync_with_stdio(false);
	int t; cin>>t;
    for( int i = 0; i < t; i++ ) testcases();
	return 0;
}

