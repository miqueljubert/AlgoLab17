#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
typedef property_map<Graph, edge_weight_t>::type	WeightMap;


void testcases() {

    int k;
    cin>>k;

    
    int n = k*(k+1)/2;
    vector<vector<int>> values(k);
    vector<int> lin_values;
    for( int i = 0; i < k; i++ ) {
        for( int j = 0; j <= i; j++ ) {
            int a;
            cin>>a;
            values[i].push_back(a);
            lin_values.push_back(a);
        }
    }

    vector<int> i_m = { -1, 0, -1, 1, 0, 1 };
    vector<int> j_m = { -1, -1, 0, 0, 1, 1 };
	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
    for( int i = 0; i < k; i++ ) {
        for( int j = 0; j <= i; j++ ) {
            int p1 = i*(i+1)/2 + j;
            for( int l = 0; l < 6; l++ ) {
                int ii = i+i_m[l];
                int jj = j+j_m[l];
                if( ii < 0 or ii >= k ) continue;
                if( jj < 0 or jj > ii ) continue;
                int p2 = ii*(ii+1)/2 + jj;
                Edge e; bool b;
                //tie(e,b) = edge(p1,p2,G);
                //if( b ) continue;
                tie(e,b) = add_edge(p1,p2,G);
                weightmap[e] = values[ii][jj];
                //cout<<"("<<i<<","<<j<<")->("<<ii<<","<<jj<<") = "<<values[ii][jj]<<endl;
            }
        }
    }

    int v1,v2;
    v1 = n-1;
    v2 = k*(k-1)/2;


    std::vector<int> d(n);
    dijkstra_shortest_paths(G, 0,
          distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, G))));
    std::vector<int> d1(n);
    dijkstra_shortest_paths(G, v1,
          distance_map(boost::make_iterator_property_map(d1.begin(), get(boost::vertex_index, G))));
    std::vector<int> d2(n);
    dijkstra_shortest_paths(G, v2,
          distance_map(boost::make_iterator_property_map(d2.begin(), get(boost::vertex_index, G))));
    int min_amount = numeric_limits<int>::max();
    int min_idx = -1;
    for( int i = 0; i < n; i++ ) {
        if( min_amount > d[i] + d1[i] + d2[i] - 2*lin_values[i] ) {
            min_idx = i;
        }
        //cout<<"index:"<<i<<" = "<<d[i]<<"+"<<d1[i]<<"+"<<d2[i]<<"->"<<d[i]+d1[i]+d2[i]<<endl;
        min_amount = min(min_amount, d[i] + d1[i] + d2[i] - 2*lin_values[i] );
    }
    //cout<<"j."<<min_idx<<endl;
    cout<<min_amount<<endl;



}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int t;	cin >> t;	// First input line: Number of testcases.
    for( int i = 0; i < t; i++ ) {
        testcases();
    }
	return 0;
}

