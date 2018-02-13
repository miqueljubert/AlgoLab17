// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

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

#include <set>
#include <unordered_map>

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
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


void do_carsharing()
{
    long n_bookings,n_stations;
    std::cin>>n_bookings>>n_stations;
    
    long n_cars = 0; 
    std::vector<long> initial_capacity(n_stations);
    for( int i = 0; i < n_stations; i++ ) {
        std::cin>>initial_capacity[i];
        n_cars += initial_capacity[i];
    }

    std::vector<long> start_station(n_bookings); 
    std::vector<long> end_station(n_bookings); 
    std::vector<long> start_time(n_bookings); 
    std::vector<long> end_time(n_bookings);
    std::vector<long> profit(n_bookings);
    
    std::set<long> times;

    for( int i = 0; i < n_bookings; i++ ) {
        std::cin>>start_station[i]>>end_station[i]>>start_time[i]>>end_time[i]>>profit[i];
        start_station[i]--;
        end_station[i]--;
    }
    times.insert(start_time.begin(),start_time.end());
    //times.insert(end_time.begin(),end_time.end());


    long n_times = times.size();
    std::unordered_map<long,long> time_index;
    
    int current = 0;
    for( auto it = times.begin(); it != times.end(); ++it ) {
        time_index[*it] = current;
        current++;
    }

    long n_nodes = n_stations*(n_times+1) + 2;
    long src = n_stations*(n_times+1);
    long trg = n_stations*(n_times+1)+1;
    //std::cout<<n_nodes<<std::endl;

    

    Graph G(n_nodes);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    long maxcap = 100*100+1;

    long maxcost = 101;
    for( int i = 0; i < n_stations; i++ ) {
        eaG.addEdge(src,i,initial_capacity[i],0);
    }
    for( int i = 0; i < n_stations; i++ ) {
        eaG.addEdge(n_times*n_stations + i,trg,maxcap,0);
    }
    for( int i = 0; i < n_times; i++ ) {
        for( int j = 0; j < n_stations; j++ ) {
            eaG.addEdge(i*n_stations+j,(i+1)*n_stations+j,maxcap,maxcost);
        }
    }
    
    for( int i = 0; i < n_bookings; i++ ) {
        long s_node = start_station[i]; 
        long e_node = end_station[i]; 
        long s_time = start_time[i]; 
        long e_time = end_time[i];
        long p      = profit[i];
        
        //Get closer start to end
        auto lb = times.lower_bound(e_time);
        long time_e_index = -1;
        long e_index = -1;
        if( lb == times.end() ) {
            time_e_index = n_times;
            e_index = n_times*n_stations + e_node;
        } else {
            time_e_index = time_index[*lb];
            e_index = (time_index[*lb])*n_stations + e_node;
        }



        long s_index = n_stations*time_index[s_time] + s_node;
        //long e_index = n_stations*time_index[e_time] + e_node;
        //std::cout<<s_index<<"->"<<e_index<<std::endl;

        long diff = time_e_index-time_index[s_time];
        
        eaG.addEdge(s_index,e_index,1,diff*maxcost-p);
    }
 
    // Option 1: Min Cost Max Flow with cycle_canceling
    //int flow1 = boost::push_relabel_max_flow(G, src, trg);
    //boost::cycle_canceling(G);
    //int cost1 = boost::find_flow_cost(G);
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    //std::cout << "flow " << flow1 << std::endl; // 5
    //std::cout << "cost " << cost1 << std::endl; // 12



    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, src, trg);
    long cost2 = boost::find_flow_cost(G);
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
    //std::cout << "cost " << cost2 << std::endl; // 12
    long s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        //std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            //<< " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    //std::cout<<"s_flow:"<<s_flow<<std::endl;

    std::cout<<-(cost2-s_flow*(n_times)*maxcost)<<std::endl;
 

}











int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) do_carsharing();

}










