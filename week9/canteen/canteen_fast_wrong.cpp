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
#include <limits>
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

void do_menus()
{
    int n_days;
    std::cin>>n_days;

    const int N=n_days*2 + 2;
    const int v_source = N-2;
    const int v_target = N-1;

    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    long min_value = 0;

    std::vector<long> n_menus(n_days);
    std::vector<long> menu_cost(n_days);
    //Menu production per day
    for( int i = 0; i < n_days; i++ ) {
        std::cin>>n_menus[i];
        std::cin>>menu_cost[i];
        //long n_menus;
        //long menu_cost;
        //std::cin>>n_menus>>menu_cost;
        //eaG.addEdge(i, i+n_days, n_menus, menu_cost );
    }
    int student_total = 0;
    std::vector<long> n_students(n_days);
    std::vector<long> menu_price(n_days);
    for( int i = 0; i < n_days; i++ ) {
        std::cin>>n_students[i];
        std::cin>>menu_price[i];
        min_value = std::max(min_value,menu_price[i]);
        //long n_students;
        //long menu_price;
        //std::cin>>n_students>>menu_price;
        //eaG.addEdge(i+n_days, v_target, n_students, -menu_price );

        student_total += n_students[i];
    }
    std::vector<long> freezer_volume(n_days-1);
    std::vector<long> electricity_price(n_days-1);
    for( int i = 0; i < n_days-1; i++ ) {
        std::cin>>freezer_volume[i];
        std::cin>>electricity_price[i];
        //long freezer_volume;
        //long electricity_price;
        //std::cin>>freezer_volume>>electricity_price;
        //eaG.addEdge(i+n_days,i+1+n_days,freezer_volume,electricity_price);
    }
    long maxCapacity = std::numeric_limits<long>::max();
    for( int i = 0; i < n_days; i++ ) {
        eaG.addEdge(v_source,i,maxCapacity,0);
    }
    
    //Fem les arestes de veritat ara
    for( int i = 0; i < n_days; i++ ) {
        eaG.addEdge(i, i+n_days, n_menus[i], menu_cost[i] + min_value );
    }
    for( int i = 0; i < n_days; i++ ) {
        eaG.addEdge(i+n_days, v_target, n_students[i], -menu_price[i] + min_value );
    }
    for( int i = 0; i < n_days-1; i++ ) {
        eaG.addEdge(i+n_days,i+1+n_days,freezer_volume[i],electricity_price[i] + min_value);
    }


    /*
    for( int i = 0; i < n_days; i++ ) {
        eaG.addEdge(2*n_days+i,v_target,maxCapacity,0);
    }
    */

    //Source-edge
    
    // Add the edges
    /*
    eaG.addEdge(v_source, v_farm1, 3, 0);
    eaG.addEdge(v_source, v_farm2, 1, 0);
    eaG.addEdge(v_source, v_farm3, 2, 0);

    eaG.addEdge(v_farm1, v_shop1, 1, 1);
    eaG.addEdge(v_farm1, v_shop2, 1, 5);
    eaG.addEdge(v_farm2, v_shop1, 1, 2);
    eaG.addEdge(v_farm2, v_shop2, 1, 2);
    eaG.addEdge(v_farm3, v_shop1, 1, 3);
    eaG.addEdge(v_farm3, v_shop2, 2, 2);

    eaG.addEdge(v_shop1, v_target, 3, 0);
    eaG.addEdge(v_shop2, v_target, 3, 0);
*/
    // Run the algorithm

    // Option 1: Min Cost Max Flow with cycle_canceling
    /*
    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    if( flow1 == student_total ) {
        std::cout<<"possible";
    } else {
        std::cout<<"impossible";
    }
    std::cout<<" "<<flow1<<" "<<-cost1<<std::endl;
    */
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    //std::cout << "flow " << flow1 << std::endl; // 5
    //std::cout << "cost " << cost1 << std::endl; // 12
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
    //std::cout << "cost " << cost2 << std::endl; // 12
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        //std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            //<< " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    /*
    std::cout << "s-out flow " << s_flow << std::endl; // 5
    // Or equivalently, you can do the summation at the sink, but with reversed sign.
    int t_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        t_flow += rescapacitymap[*e] - capacitymap[*e];
    }
    std::cout << "t-in flow " << t_flow << std::endl; // 5
    std::cout << "-----------------------" << std::endl;
    */
    int flow1 = s_flow;
    int cost1 = cost2;
    if( flow1 == student_total ) {
        std::cout<<"possible";
    } else {
        std::cout<<"impossible";
    }
    std::cout<<" "<<flow1<<" "<<-cost1<<std::endl;

}
int main() {
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        do_menus();
    }
}
