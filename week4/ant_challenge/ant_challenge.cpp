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
// Namespaces
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,                 // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
                            no_property,                        // interior properties of vertices  
                            property<edge_weight_t, int>        // interior properties of edges
                            >                       Graph;
typedef graph_traits<Graph>::edge_descriptor        Edge;       // Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor      Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef graph_traits<Graph>::edge_iterator          EdgeIt;     // to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type    WeightMap;  // property map to access the interior property edge_weight_t




void ant_challenge()
{
    int n,e,s,a,b;
    cin>>n>>e>>s>>a>>b;
    vector<Graph> forests(s,Graph(n));
    vector<WeightMap> edge_weights(s);

    for( int i = 0; i < s; i++ ) {
        edge_weights[i] = get(edge_weight, forests[i]);
    }

    int u,v,w;
    for( int i = 0; i < e; i++ ) {
        cin>>u>>v;
        Edge e1; bool success;

        for( int j = 0; j < s; j++ ) {
            cin>>w;

            tie(e1,success) = add_edge(u,v,forests[j]);
            if( not success ) cout<<"Error adding edge"<<endl;

            edge_weights[j][e1] = w;
        }
    }
    vector<Vertex> hives(s);
    int h;
    for( int i = 0; i < s; i++ ) {
        cin>>h;
        hives[i] = h;
    }
    vector<vector<Vertex>> predecessors(s,vector<Vertex>(n));
    for( int i = 0; i < s; i++ ) {
        /*
        dijkstra_shortest_paths( forests[i], hives[i],weight_map( edge_weights[i]).
                                    predecessor_map( make_iterator_property_map( 
                                                                predecessors[i].begin(),
                                                                get(vertex_index, forests[i])) 
                                                    ) );
                                                    */
        prim_minimum_spanning_tree( forests[i], make_iterator_property_map( 
                                                            predecessors[i].begin(),
                                                            get(vertex_index, forests[i]))
                                                    , weight_map( edge_weights[i] ).root_vertex(hives[i]) );


    }
    /*
    cout<<"Species network"<<endl;
    for( int i = 0; i < s; i++ ) {
        cout<<"species "<< i <<" hive:"<<hives[i]<<endl;
        for( int j = 0; j < predecessors[i].size(); j++ ) {
            int pre = predecessors[i][j];
            if( j == pre ) continue;
            Edge e1; bool success;
            tie(e1,success) = edge(j,pre,forests[i]);
            cout<<j<<" "<<pre<<" "<<edge_weights[i][e1]<<endl;

        }
        cout<<endl;
    }
    */




    Graph forest_network(n);
    WeightMap forest_weights;
    forest_weights = get(edge_weight, forest_network);

    //We build the new graph with the corresponding weights
    for( int i = 0; i < s; i++ ) {
        for( int j = 0; j < predecessors[i].size(); j++ ) {
            u = j;
            v = predecessors[i][j];
            if( u == v ) continue;

            Edge e1,e2;
            bool success, exists;

            tie(e2,exists) = edge(u,v,forests[i]);
            int weight = edge_weights[i][e2];

            tie(e1,exists) = edge(u,v,forest_network);

            if( exists ) { //If the edge already exists
                forest_weights[e1] = min(weight,forest_weights[e1]);
            } else {
                tie(e1,success) = add_edge(u,v,forest_network);
                if( not success ) cout<<"Error adding edge"<<endl;
                forest_weights[e1] = weight;
            }


        }
    }
    /*
    cout<<"final"<<endl;
    EdgeIt ei,ee;
    for( tie(ei,ee) = edges(forest_network); ei != ee; ei++ ) {
        Edge e1 = *ei;
        int u = source(e1,forest_network);
        int v = target(e1,forest_network);
        cout<<u<<" "<<v<<" "<<forest_weights[e1]<<endl;
    }*/

    vector<int> distances(n);
    dijkstra_shortest_paths( forest_network, a, weight_map( forest_weights ).
                                distance_map( make_iterator_property_map( 
                                                distances.begin(),
                                                get(vertex_index, forest_network)) 
                                            ) );
    cout<<distances[b]<<endl;
}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        ant_challenge();
    }
}

