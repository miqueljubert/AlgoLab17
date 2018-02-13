// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <string>
#include <unordered_map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Namespaces
//using namespace std;
using namespace boost;

// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,                     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
                                no_property,                        // interior properties of vertices  
                                property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                property<edge_reverse_t, Traits::edge_descriptor>>>        // interior properties of edges
                                >                   Graph;
typedef graph_traits<Graph>::edge_descriptor        Edge;           // Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor      Vertex;         // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef graph_traits<Graph>::edge_iterator          EdgeIt;         // to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;      // to iterate over all outgoing edges of a vertex



typedef property_map<Graph, edge_capacity_t>::type          EdgeCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type           ReverseEdgeMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;




void compute_buddies()
{
    int nstudents, nchars, f;
    std::cin>>nstudents>>nchars>>f;
    
    int current_chars = 0;

    std::string str;
    std::unordered_map<std::string,int> caract_map;
    std::vector<std::vector<int>> nodes_by_char(0,std::vector<int>(0));

    for( int i = 0; i < nstudents; i++ ) {
        for( int j = 0; j < nchars; j++ ) {
            std::cin>>str;
            bool found = caract_map.find(str) != caract_map.end();
            int char_id = -1;
            if( not found ) {
                caract_map[str] = current_chars;
                char_id = current_chars;

                nodes_by_char.push_back(std::vector<int>(0));
                current_chars++;
            } else {
                char_id = caract_map[str];
            }
            nodes_by_char[char_id].push_back(i);
        }
    }

    /*
    for( auto it = caract_map.begin(); it != caract_map.end(); it++ ) {
        std::cout<<it->first<<" "<<it->second<<std::endl;
    }
    std::cout<<std::endl;
    for( int i = 0; i < nodes_by_char.size(); i++ ) {
        std::cout<<i<<":";
        for( int j = 0; j < nodes_by_char[i].size(); j++ ) {
            std::cout<<nodes_by_char[i][j];
        }
        std::cout<<std::endl;
    }
    */
    

    //[0-nstudents-               graph nodes 1
    //[nstudents-2*nstudents-1]     graph nodes 2
    // 2*nstudents                  s
    // 2*nstudents+1                t
    
    //Common characteristics betweent two nodes
    std::vector<std::vector<int>> shared_chars(nstudents,std::vector<int>(nstudents,0));
    for( int i = 0; i < nodes_by_char.size(); i++ ) {
        for( int j = 0; j < nodes_by_char[i].size(); j++ ) {
            int p1 = nodes_by_char[i][j];
            for( int k = 0; k < j; k++ ) {
                if( j == k ) std::cout<<"ERROR"<<std::endl;
                int p2 = nodes_by_char[i][k];
                shared_chars[p1][p2]++;
            }
        }
    }
    /*
    for( int i = 0; i < shared_chars.size(); i++ ) {
        for( int j = 0; j < shared_chars[i].size(); j++ ) {
            std::cout<<shared_chars[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    */
    Vertex s,t;
    s = 2*nstudents;
    t = 2*nstudents + 1;
    Graph flow_network(2*nstudents+2);
    
    EdgeCapacityMap capacity = get(edge_capacity, flow_network);
    ReverseEdgeMap rev_edge = get(edge_reverse, flow_network);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, flow_network);



    for( int i = 0; i < nstudents; i++ ) {
        for( int j = 0; j < i; j++ ) {
            int ncommon = shared_chars[i][j];
            if( f < ncommon ) {
                Edge e, er; bool success;
                tie(e,success)  = add_edge(i,           nstudents+j,flow_network);
                tie(er,success) = add_edge(nstudents+j, i,          flow_network);
                
                capacity[e]  = 1;
                capacity[er] = 0;

                rev_edge[e]  = er;
                rev_edge[er] = e;



                tie(e,success)  = add_edge(j,           nstudents+i,flow_network);
                tie(er,success) = add_edge(nstudents+i, j,          flow_network);
                
                capacity[e]  = 1;
                capacity[er] = 0;

                rev_edge[e]  = er;
                rev_edge[er] = e;


            }
            //std::cout<<shared_chars[i][j]<<" ";
        }
        //std::cout<<std::endl;
    }


    for( int i = 0; i < nstudents; i++ ) {
        Edge e,er; bool success;

        tie(e,success)  = add_edge(s, i, flow_network);
        tie(er,success) = add_edge(i, s, flow_network);
        capacity[e]  = 1;
        capacity[er] = 0;
        rev_edge[e]  = er;
        rev_edge[er] = e;
        if( not success ) std::cout<<"error adding edge"<<std::endl;

        tie(e,success)  = add_edge(nstudents + i,             t, flow_network);
        tie(er,success) = add_edge(t            , nstudents + i, flow_network);
        if( not success ) std::cout<<"error adding edge"<<std::endl;
        capacity[e]  = 1;
        capacity[er] = 0;
        rev_edge[e]  = er;
        rev_edge[er] = e;
    }
    
    //edmonds_karp_max_flow( flow_network, s, t);
    push_relabel_max_flow( flow_network, s, t);
    
    int nmatches = 0;
    for( int i = 0; i < nstudents; i++ ) {
        Edge e,er; bool success;

        //tie(e,success)  = edge(s, i, flow_network);
        tie(e,success)  = edge(i+nstudents, t, flow_network);
        if( not success ) std::cout<<"error getting edge"<<std::endl;
        //std::cout<<res_capacity[e]<<std::endl;
        if(res_capacity[e] == 0 ) nmatches++;

        //tie(e,success)  = edge(nstudents + i,             t, flow_network);
        //if( not success ) std::cout<<"error getting edge"<<std::endl;
    }
    if( nmatches != nstudents ) {
        std::cout<<"optimal"<<std::endl;
    } else {
        std::cout<<"not optimal"<<std::endl;
    }
    //std::cout<<std::endl;
    /*
    std::cout<<nstudents<<"-"<<nmatches<<std::endl;
    EdgeIt ei,ee;
    for( tie(ei,ee) = edges(flow_network); ei != ee; ei++ ) {
        Edge e = *ei;
        if( capacity[e] == 1 ) std::cout<<source(e,flow_network)<<" "<<target(e,flow_network)<<" "<<1-res_capacity[e]<<std::endl;
    }
    */


}



int main()
{
    int ntests;
    std::cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        compute_buddies();
    }
}
