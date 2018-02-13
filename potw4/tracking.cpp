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
#include <unordered_map>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, int>
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

typedef std::pair<int,int> Pair;


void run_away() {

    int n,m,k,s,e;
    std::cin>>n>>m>>k>>s>>e;
	Graph G((k+1)*n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);

    std::vector<Pair> edges(m);
    std::vector<int> edge_weight(m);
    std::vector<bool> important_edge(m,false);

    for( int i = 0; i < m; i++ ) {
        int a,b,c,d;
        std::cin>>a>>b>>c>>d;

        edges[i] = Pair(a,b);
        edge_weight[i] = c;
        if( d == 1 ) important_edge[i] = true;
    }

    //Create the graph
    for( int i = 0; i < m; i++ ) {
        for( int j = 0; j <= k; j++ ) {
            Edge ed; bool ok;
            int a = j*n + edges[i].first;
            int b = j*n + edges[i].second;
            boost::tie(ed,ok) = boost::add_edge(a,b,G);
            weightmap[ed] = edge_weight[i];
            if( important_edge[i]  and j > 0 ) {
                a = j*n     + edges[i].first;
                b = (j-1)*n + edges[i].second;
                boost::tie(ed,ok) = boost::add_edge(a,b,G);
                weightmap[ed] = edge_weight[i];

                a = (j-1)*n + edges[i].first;
                b = j*n     + edges[i].second;
                boost::tie(ed,ok) = boost::add_edge(a,b,G);
                weightmap[ed] = edge_weight[i];
            }
        }
    }
    std::vector<int> pred((k+1)*n);
    std::vector<int> dist((k+1)*n);
    dijkstra_shortest_paths(G, s, predecessor_map(boost::make_iterator_property_map(pred.begin(), get(boost::vertex_index, G))).distance_map(boost::make_iterator_property_map(dist.begin(), get(boost::vertex_index, G))));

    std::cout<<dist[ k*n+e ]<<std::endl;

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;
    std::cin>>T;
	while(T--)	run_away();
	return 0;
}

