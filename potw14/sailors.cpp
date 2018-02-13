#include <iostream>
#include <cstdlib>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

using namespace std;
using namespace boost;

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


int get_flow_cost( Graph &G, const Edge &e_bnk, int w, int src, int tgt,
        EdgeCapacityMap &capacitymap, ResidualCapacityMap &rescapacitymap)

{
    capacitymap[e_bnk] = w;

    boost::successive_shortest_path_nonnegative_weights(G, src, tgt);
    int cost1 = boost::find_flow_cost(G);
    int s_flow = 0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend)=boost::out_edges(boost::vertex(src,G), G); e != eend; ++e){
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    int flow1 = s_flow;
    int t_cost = -(cost1 - 50*flow1);
    return t_cost; 
}
bool increasing_flow_cost( Graph &G, const Edge &e_bnk, int w, int src, int tgt,
        EdgeCapacityMap &capacitymap, ResidualCapacityMap &rescapacitymap)
{
    return get_flow_cost(G,e_bnk,w,src,tgt,capacitymap,rescapacitymap) <
           get_flow_cost(G,e_bnk,w+1,src,tgt,capacitymap,rescapacitymap);
}

void run()
{
    int b,s,p;
    cin>>b>>s>>p;
    
    int src = b+s;
    int bnk = b+s+1;
    int tgt = b+s+2;
    Graph G(b+s+3);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    eaG.addEdge(src,bnk,0,0);
    for( int i = 0; i < p; i++ ) {
        int b_u,s_v,c;
        cin>>b_u>>s_v>>c;
        eaG.addEdge(b_u,b+s_v,1,50-c);
    }
    for( int i = 0; i < b; i++ ) {
        eaG.addEdge(src,i,1,0);
    }
    for( int i = 0; i < b; i++ ) {
        eaG.addEdge(i,tgt,1,50);
    }

    for( int i = 0; i < s; i++ ) {
        eaG.addEdge(b+i,tgt,1,0);
    }
    
    Edge e_bnk; bool ok;
    tie(e_bnk,ok) = edge(src,bnk,G);
    
    
    capacitymap[e_bnk] = min(b,s)+1;
    boost::successive_shortest_path_nonnegative_weights(G, src, tgt);
    int cost1 = boost::find_flow_cost(G);
    cout<<-(cost1-50*b)<<endl;

}

int main() {
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
