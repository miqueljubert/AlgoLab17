#include <iostream>
#include <cstdlib>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;
using namespace boost;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt;

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


void run()
{
    int n,m,l;
    cin>>n>>m>>l;
    Graph G(n+4);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    


    int src = n;
    int pre_src = n+1;
    int pre_tgt = n+2;
    int tgt = n+3;

    int max_val = 128*301;

    vector<int> start(m);
    vector<int> end(m);
    vector<int> value(m);
    for( int i = 0; i < m; i++ ) {
        int s,e,v;
        cin>>s>>e>>v;
        eaG.addEdge(s, e, 1, -v + max_val*(e-s));

        start[i] = s;
        end[i] = e;
        value[i] = v;
    }

    for( int i = 0; i < n-1; i++ ) {
        eaG.addEdge(i,i+1,l,max_val);
    }

    eaG.addEdge(pre_src,src,l,0);
    eaG.addEdge(src,0,l,0);

    eaG.addEdge(n-1,pre_tgt,l,0);
    eaG.addEdge(pre_tgt,tgt,l,0);

    boost::successive_shortest_path_nonnegative_weights(G, pre_src, tgt);
    int cost1 = boost::find_flow_cost(G);

    //int flow1 = boost::push_relabel_max_flow(G, pre_src, tgt);
    //boost::cycle_canceling(G);
    //int cost1 = boost::find_flow_cost(G);
    
    cout<<-(cost1-max_val*(n-1)*l)<<endl;

}


int main() {
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
