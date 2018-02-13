#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <map>
using namespace std;
using namespace boost;



typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_iterator  Face_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Face Face;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Edge Edge_tri;

typedef adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_weight_t, K::FT>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::edge_iterator		EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

typedef pair<K::FT,int> Pair;

K::FT distance_limited_dfs(int c, int outside,const K::FT &d, vector<bool> &visited,
                            const Graph &G, const WeightMap &weightmap)
{
    if( visited[c] ) return false;
    if( c == outside ) return true;
    visited[c] = true;

    OutEdgeIt ei,ee;
    for( tie(ei,ee) = out_edges(c,G); ei != ee; ++ei ) {
        int v = target(*ei,G);
        if( weightmap[*ei] < 4*d ) continue;
        if(distance_limited_dfs(v,outside,d,visited,G,weightmap)) return true;
    }
    return false;
}


void testcases( int n ) {

    vector<Point> points(n);
    for( int i = 0; i < n; i++ ) {
        long x,y;
        cin>>x>>y;
        points[i] = Point(x,y);
    }
    Triangulation t;
    t.insert(points.begin(),points.end());

    int m;
    cin>>m;
    vector<Point> users(m);
    vector<K::FT> dist(m);
    for( int i = 0; i < m; i++ ) {
        long x,y;
        cin>>x>>y;
        users[i] = Point(x,y);
        cin>>dist[i];
    }

    int V = t.number_of_faces();
    Graph G(V+1);
	WeightMap weightmap = get(edge_weight, G);
    map<Face_handle,int> face_index;
    int i = 0;
    Face_iterator fi;
    for( fi = t.finite_faces_begin(); fi != t.finite_faces_end(); ++fi ) {
        face_index[fi] = i++;
    }

    int outside = V;//Vertex that represents the "outside"
    for( fi = t.finite_faces_begin(); fi != t.finite_faces_end(); ++fi ) {
        int f1 = face_index[fi];
        for( int j = 0; j < 3; j++ ) {

            K::FT dn = CGAL::squared_distance(fi->vertex((j+1)%3)->point(),
                                              fi->vertex((j+2)%3)->point());

            Edge e; bool b;
            if( t.is_infinite(fi->neighbor(j)) ) {
                tie(e,b) = add_edge(f1,outside,G);
                weightmap[e] = dn;
                continue;
            }
            int fn = face_index[fi->neighbor(j)];

            tie(e,b) = add_edge(f1,fn,G);
            weightmap[e] = dn;

        }
    }

    vector<K::FT> bottleneck(V+1,-1);
    //cout<<kft_max<<endl;
    
    //cout<<numeric_limits<K::FT>::max()<<endl;

    vector<bool> seen(V+1,false);
    priority_queue<Pair> pque;
    pque.push(Pair(K::FT(numeric_limits<K::FT>::max()),outside));
    bottleneck[outside] = numeric_limits<K::FT>::max();
    while( not pque.empty() ) {
        Pair t = pque.top();
        pque.pop();

        int u = t.second;
        K::FT d = t.first;

        if( seen[u] ) continue;
        seen[u] = true;
        
        OutEdgeIt ei,ee;
        for( tie(ei,ee) = out_edges(u,G); ei != ee; ei++ ) {
            int v = target(*ei,G);

            K::FT temp = max(bottleneck[v], min(bottleneck[u],weightmap[*ei]));

            if( temp > bottleneck[v] ) {
                bottleneck[v] = temp;
                pque.push(Pair(bottleneck[v],v));
            }

        }
    }


    for( int i = 0; i < m; i++ ) {
        Face_handle f = t.locate(users[i]);

        Point test = t.nearest_vertex(users[i])->point();
        K::FT d = CGAL::squared_distance( users[i], test );
        if( d < dist[i] ) {
            cout<<"n";
            continue;
        }

        if( t.is_infinite(f) ) {
            cout<<"y";
            continue;
        }
        int c = face_index[f];
        
        vector<bool> visited(V+1,false);
        //if(distance_limited_dfs(c,outside,dist[i],visited,G,weightmap)) cout<<"y";
        //else cout<<"n";
        //if(distance_limited_dfs(c,outside,dist[i],visited,G,weightmap)) cout<<"y";
        //else cout<<"n";
        if( bottleneck[c] >= 4*dist[i] ) cout<<"y";
        else cout<<"n";

    }


    cout<<endl;


}

int main() {
	ios_base::sync_with_stdio(false);
	int n;
    cin >> n;
    while( n ) { testcases(n); cin>>n; }
	return 0;
}

