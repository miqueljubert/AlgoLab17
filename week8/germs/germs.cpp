#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <algorithm>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_iterator  Face_iterator;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Point Point;

typedef std::map<Vertex_handle,  K::FT> VertexMap;


double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const K::FT &x) {
    return -floor_to_double(-x);
}

int get_closest_germs( int n )
{
    int lb,bb,rb,tb;
    std::cin>>lb>>bb>>rb>>tb;

    std::vector<Point> pts;
    pts.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        K::Point_2 p;
        std::cin >> p;
        pts.push_back(p);
    }

    Triangulation t;
    t.insert(pts.begin(), pts.end());

    VertexMap vm;

    //K::Line_2 l1(l,0,0), l2(r,0,0), l3(0,b,0), l4(0,t,0);


    //Can be very much optimised
    for( Vertex_iterator vi = t.finite_vertices_begin(); vi != t.finite_vertices_end(); ++vi ) {
        K::FT dl = std::pow(vi->point().x() - lb,2);
        K::FT db = std::pow(vi->point().y() - bb,2);
        K::FT dr = std::pow(vi->point().x() - rb,2);
        K::FT dt = std::pow(vi->point().y() - tb,2);
        vm[vi] = std::min(dl,std::min(db,std::min(dr,dt)));
        
    }

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Vertex_handle v1,v2;
        v1 = e->first->vertex( (e->second+1)%3 );
        v2 = e->first->vertex( (e->second+2)%3 );
        K::FT dis = CGAL::squared_distance(v1->point(),v2->point())/4;
        //std::cout<<v1->point()<<","<<v2->point()<<":"<<dis/4<<std::endl;
        vm[v1] = std::min(dis,vm[v1]);
        vm[v2] = std::min(dis,vm[v2]);
    }

    std::vector<K::FT> distances(vm.size());
    //std::cout<<vm.size()<<std::endl;
    int pos = 0;
    for( auto it = vm.begin(); it != vm.end(); ++it ) {
        distances[pos] = it->second;
        pos++;
    }
    std::sort(distances.begin(), distances.end()); 

    //std::cout<<distances.size()-1<<std::endl;
    //for( int i = 0; i < distances.size(); i++ ) std::cout<<distances[i]<<" ";
    //std::cout<<std::endl;
    //std::cout<<distances[0]<<" "<<distances[distances.size()/2]<<" "<<distances[distances.size()-1]<<std::endl;
    K::FT d1,d2,d3;
    d1 = distances[0];
    d2 = distances[distances.size()/2];
    d3 = distances[distances.size()-1];
    K::FT t1,t2,t3;
    //std::cout<<t1<<" "<<t2<<" "<<t3<<std::endl;
    std::cout<<ceil_to_double(std::sqrt(std::sqrt(d1)-0.5))<<" "<<ceil_to_double(std::sqrt(std::sqrt(d2)-0.5))<<" "<<ceil_to_double(std::sqrt(std::sqrt(d3)-0.5))<<std::endl;

}


int main()
{
    int n;
    std::cin>>n;
    while( n != 0 ) {
        get_closest_germs(n);
        std::cin>>n;
    }
}










