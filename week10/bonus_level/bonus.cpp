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


void bonus()
{
    int n;
    std::cin>>n;
    int N = 2*n*n+2;

    int v_source = N-2;
    int v_target = N-1;

    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ ) {
            int index = 2*(i*n+j);
            int index_next = index+1;
            int neighbor_down = 2*((i+1)*n+j);
            int neighbor_right = 2*(i*n+(j+1));

            int a;
            std::cin>>a;
            //std::cout<<a<<" ";

            eaG.addEdge(index,index_next,1,-a + 101);//First time a money
            eaG.addEdge(index,index_next,1, 0 + 101);//Second time 0

            if( j != n-1 ) eaG.addEdge(index_next,neighbor_right,2,0);//No money for jumping
            if( i != n-1 ) eaG.addEdge(index_next,neighbor_down,2,0); //No money for jumping

        }
        //std::cout<<std::endl;
    }
    eaG.addEdge(v_source, 0,2,0);
    eaG.addEdge(2*((n-1)*n+(n-1))+1,v_target,2,0);

    //int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    //boost::cycle_canceling(G);
    //int cost1 = boost::find_flow_cost(G);


    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost1 = boost::find_flow_cost(G);
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ ) {
            int index = 2*(i*n+j);
            int index_next = index+1;
            OutEdgeIt ei,ee;
            boost::tie(ei,ee) = boost::out_edges(index,G);
            for( ; ei != ee; ++ei ) {
                //if( boost::target(*ei,G) != index_next ) std::cout<<"error: "<<boost::target(*ei,G)<<" "<<index_next<<" - "<<index<<std::endl;
                //std::cout<<index<<" edge:"<<boost::source(*ei,G)<<","<<boost::target(*ei,G)<< " weight "<<weightmap[*ei]<<std::endl;

                if( boost::target(*ei,G) == index_next and rescapacitymap[*ei] == 0 )
                    cost1-=101;

                //if( rescapacitymap[*ei] == 0 )
                    //cost1 -= 101;
            }
        }
    }
 
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    //std::cout << "flow " << flow1 << std::endl; // 5
    std::cout <<  -cost1 << std::endl; // 12

    /*
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ ) {
            int index = 2*(i*n+j);
            int index_next = index+1;
            int neighbor_down = 2*((i+1)*n+j);
            int neighbor_right = 2*(i*n+(j+1));
            
            Edge er,ed,ee; bool b;
            boost::tie(ee,b) = boost::edge(index,index_next,G);
            if( j != n-1 ) boost::tie(er,b) = boost::edge(index_next,neighbor_right,G);
            if( i != n-1 ) boost::tie(ed,b) = boost::edge(index_next,neighbor_down,G);
            //std::cout<<"pos:"<<i<<","<<j<<" down:"<<capacitymap[ed]-rescapacitymap[ed]<<"\tright:"<<capacitymap[er]-rescapacitymap[er]<<"\tvalue:"<<weightmap[ee]<<std::endl;
            //std::cout<<"selfjump: "<<capacitymap[ee]-rescapacitymap[ee]<<std::endl;

            //std::cout<<weightmap[e]<<" "<<rescapacitymap[e]<<"/"<<capacitymap[e]<<"\t";
            //int a;
            //std::cin>>a;
            ////std::cout<<a<<" ";

            //eaG.addEdge(index,index_next,1,-a);//First time a money
            //eaG.addEdge(index,index_next,1,0);//Second time 0

            //eaG.addEdge(index_next,neighbor_right,2,0);//No money for jumping
            //eaG.addEdge(index_next,neighbor_down,2,0); //No money for jumping
            //std::cout<<"index:"<<index<<"  "<< 2*((n-1)*n+(n-1))<<std::endl;
            //std::cout<<"vs: "<<v_source<<" "<<v_target<<std::endl;
        }
        std::cout<<std::endl;
    }
    */
    
    /*
    const int N=7;
    const int v_source = 0;
    const int v_farm1 = 1;
    const int v_farm2 = 2;
    const int v_farm3 = 3;
    const int v_shop1 = 4;
    const int v_shop2 = 5;
    const int v_target = 6;

    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    // Add the edges
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

    // Run the algorithm

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    std::cout << "flow " << flow1 << std::endl; // 5
    std::cout << "cost " << cost1 << std::endl; // 12

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
    std::cout << "cost " << cost2 << std::endl; // 12
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
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

    return 0;
    */
}

int main() {
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ )  bonus();
}
