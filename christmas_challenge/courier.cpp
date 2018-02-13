#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <limits>

using namespace std;
using namespace boost;

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
    const int v_target = 6;

    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    eaG.addEdge(v_shop2, v_target, 3, 0);

    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    int t_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        t_flow += rescapacitymap[*e] - capacitymap[*e];
    }

    return 0;
	*/


void run()
{
	int z,j;
	cin>>z>>j;

	long total_zone_costs = 0;
	long total_job_profits = 0;

	vector<int> zone_cost(z);
	for( int i = 0; i < z; i++ ) {
		cin>>zone_cost[i];
		total_zone_costs += zone_cost[i];
	}
	
	vector<int> job_profit(j);
	for( int i = 0; i < j; i++ ) {
		cin>>job_profit[i];
		total_job_profits += job_profit[i];
	}
	
	vector<vector<int>> job_ticket(j,vector<int>());
	for( int i = 0; i < j; i++ ) {
		int num; cin>>num;
		for( int k = 0; k < num; k++ ) {
			int nw;
			cin>>nw;
			job_ticket[i].push_back(nw);
		}
	}
	
	int N = z+j+2;
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
	
	int src = z+j;
	int tgt = z+j+1;
	for( int i = 0; i < z; i++ ) {
		eaG.addEdge(src, i, zone_cost[i], 0);
	}
	
	for( int i = 0; i < j; i++ ) {
		eaG.addEdge(z+i, tgt, job_profit[i], 0);
	}

	const long infinite = numeric_limits<long>::max();
	for( int i = 0; i < j; i++ ) {
		for( int k = 0; k < job_ticket[i].size(); k++ ) {
			eaG.addEdge(job_ticket[i][k], i+z, infinite, 0);
		}
	}

    int flow1 = boost::push_relabel_max_flow(G, src, tgt);
	//cout<<flow1<<endl;
	

	//cout<<(total_zone_costs - total_job_profits - flow1)<<endl;
	long profit = 0;
	for( int i = 0; i < z; i++ ) {
		Edge e; bool b;
		tie(e,b) = edge(src,i,G);
		if( rescapacitymap[e] == 0 ) {//If not picked need to pay
			profit -= zone_cost[i];
		}
	}
	for( int i = 0; i < j; i++ ) {
		Edge e; bool b;
		tie(e,b) = edge(i+z,tgt,G);
		if( rescapacitymap[e] != 0 ) {//If picked
			profit += job_profit[i];
		}
	}
	cout<<(total_job_profits - flow1)<<endl;
	
}



int main() {
	int t;
	cin>>t;
	for( int i = 0; i < t; i++ ) run();
}

