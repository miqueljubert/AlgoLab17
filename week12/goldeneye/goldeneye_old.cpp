//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

// Namespaces
using namespace std;
using namespace boost;




typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;




// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_weight_t, K::FT>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// Edge iterator
// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

//Needs to be done on the MST
K::FT DFS_expensive_edge( int u, int v, std::vector<bool> &visited, const Graph &g, const WeightMap &w )
{
    if( visited[u] or u == v ) return K::FT(0);
    
    visited[u] = true;

    OutEdgeIt ei,ee;
    for( boost::tie(ei,ee) = boost::out_edges(u,g); ei != ee; ei++ ) {
        K::FT current = DFS_expensive_edge(boost::target(*ei,g),v,visited,g,w);
        //std::cout<<"current:"<<current<<std::endl;
        if( current > K::FT(-1) ) {
            //std::cout<<"w:"<<w[*ei]<<std::endl;
            return std::max(current,w[*ei]);
        }
    }
    return K::FT(-1);
}


void run()
{
    long n,m;
    K::FT p;
    std::cin>>n>>m>>p;

    std::map<Point,int> point_index;
    std::vector<Point> jammers(n);
    for( int i = 0; i < n; i++ ) {
        K::FT x,y;
        std::cin>>x>>y;
        Point p = Point(x,y);
        jammers[i] = p;
        point_index[p] = i;
    }

    std::vector<Point> mission_start(m);
    std::vector<Point> mission_end(m);
    for( int i = 0; i < m; i++ ) {
        K::FT x,y;
        std::cin>>x>>y;
        mission_start[i] = Point(x,y);
        std::cin>>x>>y;
        mission_end[i] = Point(x,y);
    }

    Triangulation t;
    t.insert(jammers.begin(),jammers.end());

    Graph G(n);
    WeightMap weightmap = get(edge_weight, G);
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Point p1 = (e->first)->vertex((e->second+1)%3)->point();
        Point p2 = (e->first)->vertex((e->second+2)%3)->point();
        K::FT dist = CGAL::squared_distance(p1,p2);
        bool b; Edge edge;

        //std::cout<<"p1:"<<point_index[p1]<<" "<<point_index[p2]<<std::endl;

        boost::tie(edge,b) = boost::add_edge(point_index[p1],point_index[p2],G);
        weightmap[edge] = dist;
        //std::cout<<dist<<std::endl;

    }
    std::vector<int> predecessor(n,0);
    prim_minimum_spanning_tree(G,&predecessor[0]);

    Graph MST(n);
    WeightMap weightmapmst = get(edge_weight, MST);
    for( int i = 0; i < predecessor.size(); i++ ) {
        //std::cout<<i<<" "<<predecessor[i]<<std::endl;
        if( i == predecessor[i] ) continue;
        bool b; Edge edge;
        boost::tie(edge,b) = boost::add_edge(predecessor[i],i,MST);
        weightmapmst[edge] = weightmap[ boost::edge(predecessor[i],i,G).first ];

    }
    
    K::FT min_power_needed = -1;
    K::FT min_power_same_set = -1;
    std::vector<bool> visited(n,false);
    for( int i = 0; i < m; i++ ) {
        if( n == 1 ) {

            K::FT d1 = CGAL::squared_distance(mission_start[i],jammers[0]);
            K::FT d2 = CGAL::squared_distance(mission_end[i],jammers[0]);

            if( 4*d1 <= p and 4*d2 <= p ) std::cout<<"y";
            else std::cout<<"n";

            if( min_power_needed == -1 ) min_power_needed = 4*std::max(d1,d2);
            else                         min_power_needed = std::max(min_power_needed,4*std::max(d1,d2));


        } else if( n == 2 ) {
            K::FT d1 = CGAL::squared_distance(mission_start[i],jammers[0]);
            K::FT d2 = CGAL::squared_distance(mission_start[i],jammers[1]);
            K::FT d3 = CGAL::squared_distance(mission_end[i],jammers[0]);
            K::FT d4 = CGAL::squared_distance(mission_end[i],jammers[1]);
            K::FT dj = CGAL::squared_distance(jammers[0],jammers[1]);
            //std::cout<<"d1:"<<d1<<std::endl;
            //std::cout<<"d2:"<<d2<<std::endl;
            //std::cout<<"d3:"<<d3<<std::endl;
            //std::cout<<"d4:"<<d4<<std::endl;
            //std::cout<<"dj:"<<dj<<std::endl;
            bool valid = false;
            K::FT largest_edge;
            if( d1 < d2 ) {
                if( 4*d1 <= p and dj <= p and 4*d4 <= p ) { //a->j0->j1->b
                    std::cout<<"y";
                    largest_edge = std::max(4*d1,std::max(dj,4*d4));
                    valid = true;
                } else if( 4*d1 <= p and 4*d3 <= p ) {
                    std::cout<<"y";
                    largest_edge = std::max(4*d1,4*d3);
                    valid = true;
                } else {
                    std::cout<<"n";
                    largest_edge = std::min( std::max(4*d1, std::max( dj, 4*d4 )), std::max(4*d1, 4*d3) );
                }
                
            } else {
                if( 4*d2 <= p and dj <= p and 4*d3 <= p ) { //a->j1->j0->b
                    std::cout<<"y";
                    largest_edge = std::max(4*d2,std::max(dj,4*d3));
                    valid = true;
                } else if( 4*d2 <= p and 4*d4 <= p ) {
                    std::cout<<"y";
                    largest_edge = std::max(4*d2,4*d4);
                    valid = true;
                } else {
                    std::cout<<"n";
                    largest_edge = std::min( std::max(4*d2, std::max( dj, 4*d3 )), std::max(4*d2, 4*d4) );
                }

            }

            if( min_power_needed == -1 ) min_power_needed = largest_edge;
            else                         min_power_needed = std::max(min_power_needed,largest_edge);

            if( valid ) {
                if( min_power_same_set == -1 ) min_power_same_set = largest_edge;
                else                           min_power_same_set = std::max(min_power_same_set,largest_edge);
            }


        } else {

            Face_handle f1 = t.locate(mission_start[i]);
            Face_handle f2 = t.locate(mission_end[i]);
            
            std::vector<int> start_candidates;
            std::vector<int> end_candidates;
            std::vector<K::FT> start_distance;
            std::vector<K::FT> end_distance;


            K::FT largest_edge = -1;
            for( int j = 0; j < 3; j++ ) {
                Point p1,p2; 
                Vertex_handle v;
                K::FT d;

                v = f1->vertex(j);
                if( not t.is_infinite(v) ) {
                    p1 = v->point();
                    p2 = mission_start[i];
                    d = CGAL::squared_distance(p1,p2);

                    start_candidates.push_back(point_index[p1]);
                    start_distance.push_back(d);

                }

                v = f2->vertex(j);
                if( not t.is_infinite(v) ) {
                    p1 = v->point();
                    p2 = mission_end[i];
                    d = CGAL::squared_distance(p1,p2);

                    end_candidates.push_back(point_index[p1]);
                    end_distance.push_back(d);

                }


            }
            bool end = false;
            for( int j = 0; not end and j < start_candidates.size(); j++ ) {
                for( int k = 0; not end and k < end_candidates.size(); k++ ) {
                    std::vector<bool> visited(n,false);
                    
                    K::FT d1 = start_distance[j];
                    K::FT d2 = end_distance[k];

                    K::FT longest_dst = DFS_expensive_edge(start_candidates[j],end_candidates[k],visited,MST,weightmapmst);

                    //std::cout<<"b:"<<longest_dst<<std::endl;

                    if( 4*d1 <= p and 4*d2 <= p and longest_dst <= p ) {
                        std::cout<<"y";
                        end = true;
                    }
                }
            }
            if( not end ) std::cout<<"n";
        }
    }
    std::cout<<std::endl;
    std::cout<<min_power_needed<<std::endl;
    std::cout<<min_power_same_set<<endl;

    

}


int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        run();
    }
}
