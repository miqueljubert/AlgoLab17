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
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <queue>

#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#include <boost/property_map/property_map.hpp>



typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!


// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator
typedef boost::graph_traits<Graph>::edge_iterator  EdgeIt; // Iterator



// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    Edge addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
        return e;
    }
};











// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
    int n_vertexs, n_edges, start_vertex, end_vertex;
    std::cin>>n_vertexs>>n_edges>>start_vertex>>end_vertex;
    
    long max_capacity = 0;

	int V = n_vertexs;
    //std::cout<<"n_vertexs:"<<n_vertexs<<std::endl;
	Graph G(V+1);
	Graph G_distance(V);
	EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
	EdgeCapacityMap capmap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revmap = boost::get(boost::edge_reverse, G);
    EdgeAdder eaG(G,capmap,weightmap,revmap);

	EdgeWeightMap distance_weightmap = boost::get(boost::edge_weight, G_distance);

	for (int i = 0; i < n_edges; ++i) {
        int a,b,c,d;
        std::cin>>a>>b>>c>>d;
        eaG.addEdge(a,b,c,d);
        eaG.addEdge(b,a,c,d);

        max_capacity += c;

		Edge e;	bool success;
        boost::tie(e,success) = boost::add_edge(a,b,G_distance);
        distance_weightmap[e] = d;
        boost::tie(e,success) = boost::add_edge(b,a,G_distance);
        distance_weightmap[e] = d;
	}

    
    std::vector<long> d(n_vertexs);
    
    boost::dijkstra_shortest_paths(G_distance,start_vertex,
           distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, G_distance)))
            );

    long min_dist = d[end_vertex];

    //start
    int src = V+1;
    Edge e_src, e_src_rev;
    e_src = eaG.addEdge(src,start_vertex,1,0);
    e_src_rev = revmap[e_src];



	ResidualCapacityMap rescapmap = boost::get(boost::edge_residual_capacity, G);
    EdgeIt ei,ee;
    boost::push_relabel_max_flow(G,src,end_vertex);
    max_capacity = boost::find_flow_cost(G);
    for( boost::tie(ei,ee) = boost::edges(G); ei != ee; ++ei ) {
        rescapmap[*ei] = 0;
    }


    //std::cout<<"distance:"<<min_dist<<std::endl;

    long lower = 0;
    long upper = max_capacity;

    if( d[end_vertex] == INT_MAX ) {
        std::cout<<0<<std::endl;
        return;
    }

    //Binary search
    long cost_low = 0;
    long prev_cost = 0;
    int middle = 0;
    while( true ) {

        if( lower + 1 >= upper ) {
            capmap[e_src] = upper;

            boost::push_relabel_max_flow(G,src,end_vertex);
            boost::cycle_canceling(G);
            long cost_upper = boost::find_flow_cost(G);
            //std::cout<<"cu:"<<cost_upper<<std::endl;
            if( cost_upper != upper*min_dist ) middle = lower;
            else middle = upper;
            break;
        }

        middle = (lower+upper)/2;

        capmap[e_src] = middle;

        boost::push_relabel_max_flow(G,src,end_vertex);
        boost::cycle_canceling(G);
        cost_low = boost::find_flow_cost(G);

        std::cout<<"lower: "<<lower<<" upper:"<<upper<<" middle:"<<middle<<" cost:"<<cost_low<<" target:"<<middle*min_dist<<std::endl;

        //std::cout<<"cost_low:"<<cost_low<<std::endl;
        for( boost::tie(ei,ee) = boost::edges(G); ei != ee; ++ei ) {
            rescapmap[*ei] = 0;
        }
        if( cost_low != middle*min_dist ) upper = middle-1;
        else lower = middle;
        //std::cout<<"new lower:"<<lower<<" upper:"<<upper<<std::endl;

        //std::cout<<cost_low<<std::endl;
    }

    capmap[e_src] = middle;
    //std::cout<<middle<<std::endl;
    boost::push_relabel_max_flow(G,src,end_vertex);
    boost::cycle_canceling(G);
    prev_cost = cost_low;
    cost_low = boost::find_flow_cost(G);



    std::cout<<cost_low/min_dist<<std::endl;
    

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;
    std::cin>>T;
	while(T--)	testcases();
	return 0;
}

