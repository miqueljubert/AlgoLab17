
// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_circulator Vertex_circulator;



// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, K::FT> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t




long longest_edge_dfs( int u, int v, const Graph &G, const WeightMap &wm, std::vector<bool> &visited)
{
    if( visited[u] ) return 0;
    visited[u] = true;

    if( u == v ) return 0;
    
    OutEdgeIt ei, ee;
    for( boost::tie(ei,ee) = boost::out_edges(u,G); ei != ee; ++ei ) {
        int k = boost::target(*ei,G);
        K::FT longest = longest_edge_dfs(k,v,G,wm,visited);
        if( longest != -1 ) return std::max( wm[*ei], longest );
    }

    return -1;
}


void run()
{
    long n,m;
    K::FT p;
    std::cin>>n>>m>>p;
    std::vector<Point> jammers(n);
    std::vector<Point> m_start(m);
    std::vector<Point> m_end(m);

    std::map<Point,int> point_index;

    for( int i = 0; i < n; i++ ) {
        long x1,y1;
        std::cin>>x1>>y1;
        jammers[i] = Point(x1,y1);
        point_index[jammers[i]] = i;
    }
    for( int i = 0; i < m; i++ ) {
        long x1,y1,x2,y2;
        std::cin>>x1>>y1>>x2>>y2;
        m_start[i] = Point(x1,y1);
        m_end[i] = Point(x2,y2);
    }

    Triangulation t;
    t.insert(jammers.begin(),jammers.end());
    
    if( n < 3 ) std::cout<<"skip"<<std::endl;
    if( n < 3 ) return;


    Graph G(n);
    WeightMap wm = boost::get(boost::edge_weight,G);
    for( Edge_iterator ee = t.finite_edges_begin(); ee != t.finite_edges_end(); ++ee ) {
        Point p1 = (ee->first)->vertex( (ee->second + 1)%3 )->point();
        Point p2 = (ee->first)->vertex( (ee->second + 2)%3 )->point();

        int i1 = point_index[p1];
        int i2 = point_index[p2];
        K::FT dist = CGAL::squared_distance(p1,p2);
        
        Edge e; bool b;
        boost::tie(e,b) = boost::add_edge(i1,i2,G);
        wm[e] = dist;
    }
    std::vector<int> predecessor(n);
    //boost::kruskal_minimum_spanning_tree(G,&predecessor[0]);
    boost::prim_minimum_spanning_tree(G,&predecessor[0]);

    //for( int i = 0; i < n; i++ ) std::cout<<i<<"->"<<predecessor[i]<<std::endl;

    Graph MST(n);
    WeightMap wmmst = boost::get(boost::edge_weight,MST);
    for( int i = 0; i < n; i++ ) {
        if( i == predecessor[i] ) continue;

        Edge e; bool b;
        boost::tie(e,b) = boost::edge(predecessor[i],i,G);
        if( not b ) std::cerr<<"ERROR1"<<std::endl;
        K::FT w = wm[e];
        if( w <= p ) {
            boost::tie(e,b) = boost::add_edge(predecessor[i],i,MST);
            if( not b ) std::cerr<<"ERROR2"<<std::endl;
            wmmst[e] = w;
        }
    }
    
    K::FT min_power = -1;
    K::FT min_power_same = -1;
    //Decide which antennas to go to
    for( int i = 0; i < m; i++ ) {
        Point p_start = m_start[i];
        Point p_end   = m_end[i];
        
        Face_handle f1 = t.locate(p_start);
        Face_handle f2 = t.locate(p_end);


        std::vector<int> j_start;
        std::vector<K::FT> d_start;
        std::vector<int> j_end;
        std::vector<K::FT> d_end;

        Vertex_handle v1 = t.nearest_vertex(p_start);
        Vertex_handle v2 = t.nearest_vertex(p_start);

        /*
        for( int j = 0; j < 3; j++ ) {
            Vertex_handle v1 = f1->vertex(j);
            Vertex_handle v2 = f2->vertex(j);

            if( not t.is_infinite(v1) ) {
                Point p_j = v1->point();
                j_start.push_back(point_index[p_j]);
                d_start.push_back(CGAL::squared_distance(p_start,p_j));
            }
            if( not t.is_infinite(v2) ) {
                Point p_j = v2->point();
                j_end.push_back(point_index[p_j]);
                d_end.push_back(CGAL::squared_distance(p_end,p_j));
            }
        }
        */
        
        Point p_t = v1->point();
        j_start.push_back(point_index[p_t]);
        d_start.push_back(CGAL::squared_distance(p_start,p_t));

        p_t = v2->point();
        j_end.push_back(point_index[p_t]);
        d_end.push_back(CGAL::squared_distance(p_end,p_t));

        Vertex_circulator vc = t.incident_vertices(v1);
        while( ++vc != t.incident_vertices(v1) ) {
            if( not t.is_infinite(vc) ) {
                Point p_j = vc->point();
                j_start.push_back(point_index[p_j]);
                d_start.push_back(CGAL::squared_distance(p_start,p_j));
            }
        }
        vc = t.incident_vertices(v2);
        while( ++vc != t.incident_vertices(v2) ) {
            if( not t.is_infinite(vc) ) {
                Point p_j = vc->point();
                j_end.push_back(point_index[p_j]);
                d_end.push_back(CGAL::squared_distance(p_end,p_j));
            }
        }




        
        K::FT longest_d = K::FT(-1);
        for( int j = 0; j < j_start.size(); j++ ) {
            for( int k = 0; k < j_end.size(); k++ ) {
                std::vector<bool> visited(n,false);
                K::FT longest = longest_edge_dfs(j_start[j],j_end[k],MST,wmmst,visited);
                longest = std::max(longest,4*std::max(d_start[j],d_end[k]));
                if( longest == -1 ) continue;

                if( longest_d < 0 ) longest_d = longest;
                else longest_d = std::min(longest,longest_d);
            }
        }
        //std::cout<<longest_d<<",";
        if( longest_d >= 0 and longest_d <= p ) {
            std::cout<<"y";
            min_power_same = std::max(min_power_same,longest_d);
        } else {
            std::cout<<"n";
        }

        min_power = std::max(min_power,longest_d);

    }
    std::cout<<std::endl;
    std::cout<<min_power<<std::endl;
    std::cout<<min_power_same<<std::endl;



    //std::vector<bool> visited(n,false);
    //std::cout<<longest_edge_dfs(0,n-1,MST,wmmst,visited)<<std::endl;
    
}



// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int t;
    std::cin>>t;
    for( int i = 0; i <t; i++ ) run();
	return 0;
}







