// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> distance_Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                boost::property <boost::edge_weight_t, long> > distance_Graph; // new!


typedef boost::property_map<distance_Graph, boost::edge_weight_t >::type       distance_EdgeWeightMap; // new!
typedef boost::graph_traits<Graph>::edge_descriptor            distance_Edge;

// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

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

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void avalanche()
{
    int n_positions,n_edges,n_agents,n_shelters,shelter_cap,shelter_time;
    std::cin>>n_positions>>n_edges>>n_agents>>n_shelters>>shelter_cap>>shelter_time;

    // Create Graph and Maps
    distance_Graph G_distances(n_positions);
    distance_EdgeWeightMap distance_weightmap = boost::get(boost::edge_weight, G_distances);

    Graph G(n_agents+n_shelters+2);
    int src = n_agents+n_shelters;
    int snk = n_agents+n_shelters+1;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    for( int i = 0; i < n_edges; i++ ) {
        int u,v,z;
        char c;
        std::cin>>c>>u>>v>>z;
        if( c == 'S' ) {
            distance_Edge e; bool success;
            boost::tie(e,success) = boost::add_edge(u,v,G_distances);
            distance_weightmap[e] = z;
            //eaG.addEdge(u,v,max_capacity,z);
        } else {
            distance_Edge e; bool success;
            boost::tie(e,success) = boost::add_edge(u,v,G_distances);
            distance_weightmap[e] = z;
            boost::tie(e,success) = boost::add_edge(v,u,G_distances);
            distance_weightmap[e] = z;
            //eaG.addEdge(u,v,max_capacity,z);
            //eaG.addEdge(v,u,max_capacity,z);
        }
    }
    
    std::vector<int> agent_pos(n_agents);
    for( int i = 0; i < n_agents; i++ ) {
        int pos;
        std::cin>>pos;
        agent_pos[i] = pos;
    }

    std::vector<int> shelter_pos(n_shelters);
    for( int i = 0; i < n_shelters; i++ ) {
        int pos;
        std::cin>>pos;
        shelter_pos[i] = pos;
    }



	//std::vector<Vertex> predmap(V);
	std::vector<std::vector<int>> distmaps(n_agents,std::vector<int>(n_positions));//Distance agent->shelter
    for( int i = 0; i < n_agents; i++ ) {
        Vertex start = agent_pos[i];
        boost::dijkstra_shortest_paths(G_distances, start,
            boost::distance_map(boost::make_iterator_property_map(distmaps[i].begin(), boost::get(boost::vertex_index, G_distances))));	// distance map as Named Parameter
    }
    
    long max_time = -1;
    for( int i = 0; i < n_agents; i++ ) {
        eaG.addEdge(src,i,1,0);//Add an edge to the agents, with 1 capacity and zero cost
        for( int j = 0; j < n_shelters; j++ ) {
            int shelter_position = shelter_pos[j];
            long distance = distmaps[i][shelter_position];

            if( distance < INT_MAX) max_time = std::max(max_time,distance); // If reachable
            
            //if( distance <= 9-shelter_time )
                //eaG.addEdge(i,j+n_agents,1,0);//Always add the edge, but don't give it a capacity
            eaG.addEdge(i,j+n_agents,0,0);//Always add the edge, but don't give it a capacity
        }
    }
    for( int i = 0; i < n_shelters; i++ ) {
            eaG.addEdge(n_agents+i,snk,1,0);//Capacity, weight
    }
    max_time += shelter_time;
    ///std::cout<<"mx:"<<max_time<<std::endl;

    //Do the computations
    int opt_time = INT_MAX;
    //for( int current_time = shelter_time; current_time <= max_time; current_time++ ) {
    int lower = shelter_time;
    int upper = max_time;
    while( lower <= upper ) {

        int middle = (lower+upper)/2;

        for( int i = 0; i < n_agents; i++ ) {
            for( int j = 0; j < n_shelters; j++ ) {
                int shelter_position = shelter_pos[j];
                int distance = distmaps[i][shelter_position];

                Edge e; bool success;
                boost::tie(e,success) = boost::edge(i,j+n_agents,G);
                if(  distance <= middle-shelter_time ) {
                    //eaG.addEdge(i,j+n_agents,0,0);//Always add the edge, but don't give it a capacity
                    capacitymap[e] = 1;
                } else {
                    capacitymap[e] = 0;
                }
            }
        }

        long flow1 = boost::push_relabel_max_flow(G, src, snk);

        bool possible = (flow1 == n_agents);
        if( possible ) {
            opt_time = std::min(opt_time,middle);
        }

        if(possible) { //If possible, we can remove all the upper part
            upper = middle-1;
        } else {//If not possible, need to look further up
            lower = middle+1;
        }
    }
    std::cout<<opt_time<<std::endl;


}
int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        avalanche();
    }
}
