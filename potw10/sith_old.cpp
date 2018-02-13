// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>
#include <cassert>
#include <cmath>
#include <unordered_map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#include <boost/pending/disjoint_sets.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::no_property
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type VertexIndex;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;


// Functions
// ========= 
void testcases() {

    long n_vertices, radius;
    std::cin>>n_vertices>>radius;
    K::FT squared_radius = K::FT(std::pow(radius,2));

    std::vector<Point> vertexs(n_vertices);

    for( long i = 0; i < n_vertices; i++ ) {
        Point p;
        std::cin>>p;
        vertexs[i] = p;
    }
	Graph G(n_vertices);


    std::vector<VertexIndex> rank(num_vertices(G));
    std::vector<Vertex> parent(num_vertices(G));

    typedef VertexIndex* Rank;
    typedef Vertex* Parent;

    boost::disjoint_sets<Rank, Parent> ds(&rank[0], &parent[0]);
    
    long current_optimal = 1;
    ds.make_set(vertexs.size()-1);
    for( long i = vertexs.size()-2; i >= 0; i-- ) {
        ds.make_set(i);
        for( long j = i+1; j < vertexs.size(); j++ ) {
            K::FT d = CGAL::squared_distance(vertexs[i],vertexs[j]);
            if( d <= squared_radius ) {
                ds.union_set(i,j);
            }
        }
        
        std::unordered_map<long,long> sets;
        for( int j = i; j < vertexs.size(); j++ ) {
            int set = ds.find_set(j);
            if( sets.find(set) == sets.end() ) {
                sets[set] = 1;
            } else {
                sets[set]++;
            }
        }
        long largestcc  = -1;
        for( auto ei = sets.begin(); ei != sets.end(); ++ei ) {
            //std::cout<<ei->first<<":"<<ei->second<<std::endl;
            largestcc = std::max(largestcc,ei->second);
        }
        current_optimal = std::max(current_optimal,std::min( i, largestcc));
    }
    std::cout<<current_optimal<<std::endl;
    /*
	std::vector<int> componentmap(n_vertices);
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G))); 
	std::cout << "Connected components: " << ncc << "\n";
	std::vector<int> componentsize(ncc);
	// Iterate over all vertices
	for (int i = 0; i < n_vertices; ++i)
		++componentsize[componentmap[i]];
	for (int i = 0; i < ncc; ++i)
		std::cout << "Component " << i << " has size " << componentsize[i] << ".\n";
	std::cout << std::endl;
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
