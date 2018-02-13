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




typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::undirectedS> Undir_Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Undir_Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Undir_Graph; // new!

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

typedef boost::property_map<Undir_Graph, boost::edge_capacity_t>::type          Undir_EdgeCapacityMap;
typedef boost::property_map<Undir_Graph, boost::edge_weight_t >::type           Undir_EdgeWeightMap; // new!
typedef boost::property_map<Undir_Graph, boost::edge_residual_capacity_t>::type Undir_ResidualCapacityMap;
typedef boost::property_map<Undir_Graph, boost::edge_reverse_t>::type           Undir_ReverseEdgeMap;
typedef boost::graph_traits<Undir_Graph>::vertex_descriptor                     Undir_Vertex;
typedef boost::graph_traits<Undir_Graph>::edge_descriptor                       Undir_Edge;
typedef boost::graph_traits<Undir_Graph>::out_edge_iterator                     Undir_OutEdgeIt; // Iterator


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














// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
    int n_vertexs, n_edges, start_vertex, end_vertex;
    std::cin>>n_vertexs>>n_edges>>start_vertex>>end_vertex;

	int V = n_vertexs;
    //std::cout<<"n_vertexs:"<<n_vertexs<<std::endl;
	Undir_Graph G(V);
	Undir_EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
	Undir_EdgeCapacityMap capmap = boost::get(boost::edge_capacity, G);
	for (int i = 0; i < n_edges; ++i) {
		Undir_Edge e;	bool success;
        int a,b,c,d;
        std::cin>>a>>b>>c>>d;
		boost::tie(e, success) = boost::add_edge(a, b, G);
        capmap[e] = c;
		weightmap[e] = d;
	}
	//std::cout << std::endl;

	// Dijkstra shortest paths
	// =======================
	std::vector<Undir_Vertex> predmap(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	boost::dijkstra_shortest_paths(G, end_vertex, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    
	

    //Graph with only the edges that are a part of a shortest path
    Graph G_flow(V);
    EdgeCapacityMap     flow_capacitymap = boost::get(boost::edge_capacity, G_flow);
    EdgeWeightMap       flow_weightmap = boost::get(boost::edge_weight, G_flow);
    ReverseEdgeMap      flow_revedgemap = boost::get(boost::edge_reverse, G_flow);
    ResidualCapacityMap flow_rescapacitymap = boost::get(boost::edge_residual_capacity, G_flow);
    EdgeAdder eaG(G_flow, flow_capacitymap, flow_weightmap, flow_revedgemap);


    std::vector<bool> visited(n_vertexs,false);
    std::queue<std::pair<int,int>> cua;
    cua.push(std::pair<int,int>(start_vertex,distmap[start_vertex]));
    while( not cua.empty() ) {
        std::pair<int,int> current = cua.front();
        //visited[current.first] = true;
        cua.pop();

        int opt_distance = current.second;
        Undir_OutEdgeIt ei,ee;
        for( boost::tie(ei,ee) = boost::out_edges(current.first,G); ei != ee; ++ei ) {
            int s = boost::source(*ei,G);
            int v = boost::target(*ei,G);
            
            int edge_weight = weightmap[*ei];
            int target_distance = distmap[v];

            //std::cout<<current.first<<":"<<s<<" "<<v<<std::endl;
            //std::cout<<"is-goal? "<<(v == end_vertex)<<std::endl;
            //std::cout<<"opt_distance:"<<opt_distance<<std::endl;
            //std::cout<<"target_distance:"<<target_distance<<std::endl;
            //std::cout<<"edge_weight:"<<edge_weight<<std::endl;

            if( (opt_distance - edge_weight) == target_distance ) {
                //std::cout<<"part of optimal path"<<std::endl;
                
                eaG.addEdge(s,v,capmap[*ei],0);
                
                //std::cout<<s<<","<<v<<"-"<<visited[s]<<","<<visited[v]<<std::endl;
                if( not visited[v] ) {
                    cua.push(std::pair<int,int>(v,opt_distance-edge_weight));
                    visited[v] = true;
                }
                //boost::tie(e,b) = boost::edge(u,v,G_flow);


            } else {
                //std::cout<<"not part of optimal path"<<std::endl;
            }
            
        }
    }
    
    //std::cout<<"---------------------------------------"<<std::endl;
    
    
    
    long flow1 = boost::push_relabel_max_flow(G_flow, start_vertex, end_vertex);
    ///long flow2 = boost::edmonds_karp_max_flow(G_flow, start_vertex, end_vertex);
    std::cout<<flow1<<std::endl;
    
    
    
    
    /*
	int maxdist = 0;
	Vertex furthest = start;
	int reachable = 0;
	for (int i = 0; i < V; ++i) {
		if (distmap[i] < INT_MAX) {	// check whether vertex i is reachable, i.e. has "finite" distance from the start
			++reachable;		// INT_MAX comes from climits and the type of distmap (vector<int>).
			if (distmap[i] > maxdist) {
				maxdist = distmap[i];
				furthest = i;
			}
		}
	}

	std::cout << "Dijkstra starting in node " << start << " reaches " << reachable << " out of " << V << " vertices.\n";
	std::cout << "The furthest reachable vertex is " << furthest << " with distance " << maxdist << ".\n";
	std::cout << "Edges in Dijkstra's shortest paths tree:\n";
	// EdgeIterators
	// =============
	EdgeIt ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges. 
		// For undirected graphs, each edge is visited once, with some orientation.
		// ebeg is EdgeIterator, *ebeg is EdgeDescriptor
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);
		if (predmap[u] == v || predmap[v] == u)
			std::cout << u << " -- " << v << " (weight " << weightmap[*ebeg] << ")\n";

	}
	std::cout << "List of disconnected single nodes: ";
	for (int i = 0; i < V; ++i)
		if (predmap[i] == i && i != start)
			std::cout << i << " ";
	std::cout << "\n" << std::endl;
	*/

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;
    std::cin>>T;
	while(T--)	testcases();
	return 0;
}

