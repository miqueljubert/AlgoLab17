#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>
#include <map>
#include <utility>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point  Point;

typedef std::pair<int,int> Pair;
typedef std::pair<K::FT,int> Edge_i;

typedef boost::disjoint_sets_with_storage<> UF;

void run()
{


    long n,m;
    double p;
    std::cin>>n>>m>>p;


    std::vector<Point> points(n);
    std::map<Point,int> point_index;
    for( long i = 0; i < n; i++ ) {
        long x,y;
        std::cin>>x>>y;
        points[i] = Point(x,y);
        point_index[points[i]] = i;

    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    std::vector<Point> mission_start(m);
    std::vector<Point> mission_end(m);
    for( long i = 0; i < m; i++ ) {
        long x,y;
        std::cin>>x>>y;
        mission_start[i] = Point(x,y);
        std::cin>>x>>y;
        mission_end[i] = Point(x,y);
    }

    std::map<int,Pair> edge_map;//Maps edge index->vertices
    std::vector<Edge_i> edges;
    int ie = 0;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Point p1 = e->first->vertex( (e->second + 1) % 3)->point();
        Point p2 = e->first->vertex( (e->second + 2) % 3)->point();
        int i1 = point_index[p1];
        int i2 = point_index[p2];
        K::FT d = t.segment(e).squared_length();
        edges.push_back(Edge_i(d,ie));
        edge_map[ie] = Pair(i1,i2);
        ie++;
    }
    std::sort(edges.begin(),edges.end());

    UF union_find_p(n);
    for( int i = 0 ; i < edges.size(); i++ ) {
        if( edges[i].first > p ) break;
        int u = edge_map[edges[i].second].first;
        int v = edge_map[edges[i].second].second;
        union_find_p.union_set(u,v);
    }


    K::FT a = 0;
    K::FT b = 0;
    int a_it = 0;
    int b_it = 0;
    UF union_find_a(n);
    UF union_find_b(n);
    for( long j = 0; j < m; j++ ) {
        Point s = t.nearest_vertex(mission_start[j])->point();
        Point e = t.nearest_vertex(mission_end[j])->point();
        int i_s = point_index[s];
        int i_e = point_index[e];
        K::FT d1 = CGAL::squared_distance(mission_start[j],s);
        K::FT d2 = CGAL::squared_distance(mission_end[j],e);
        K::FT d_p = 4*std::max(d1,d2);

        if( d_p <= p and union_find_p.find_set(i_s) == union_find_p.find_set(i_e) ) { //If they can be reached
            std::cout<<"y";
            while( union_find_b.find_set(i_s) != union_find_b.find_set(i_e) ) {
                K::FT d = edges[b_it].first;
                int u = edge_map[edges[b_it].second].first;
                int v = edge_map[edges[b_it].second].second;

                union_find_b.union_set(u,v);
                b_it++;
            }
            b = std::max(b,d_p);
            if( b_it > 0 ) b = std::max(b,edges[b_it-1].first);
        }else{
            std::cout<<"n";
        }
        while( union_find_a.find_set(i_s) != union_find_a.find_set(i_e) ) {
            K::FT d = edges[a_it].first;
            int u = edge_map[edges[a_it].second].first;
            int v = edge_map[edges[a_it].second].second;

            union_find_a.union_set(u,v);
            a_it++;
        }
        a = std::max(a,b);
        a = std::max(a,d_p);
        if( a_it > 0 ) a = std::max(a,edges[a_it-1].first);
        

    }
    std::cout<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;



}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
