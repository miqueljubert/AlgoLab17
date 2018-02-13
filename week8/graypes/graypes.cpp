//#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <algorithm>

//typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Finite_vertices_iterator  Vertex_iterator;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void bet_on_graypes( int ngrapes )
{
    //Read the grapes
    Triangulation t;
    std::vector<Triangulation::Point> apes(ngrapes);
    //std::vector<> apes(ngrapes);
    for ( int i = 0; i < ngrapes; ++i) {
        Triangulation::Point p;
        std::cin >> p;
        apes[i] = p;
    }
    if( ngrapes == 2 ) {
        std::cout<<(int)-floor_to_double(-50*sqrt(CGAL::squared_distance(apes[0],apes[1])))<<std::endl;
        return;
    }

    t.insert(apes.begin(),apes.end());
    if( t.number_of_faces() == 0 ) { //All points are aligned...
        //Min distance betweent point in line that is not O(n^2)
        std::vector<K::FT> distance_to_first(ngrapes-1);
        Triangulation::Point p1 = apes[0];
        for( int i = 0; i < ngrapes-1; i++ ) {
            Triangulation::Point p2 = apes[i+1];
            distance_to_first[i] = sqrt(CGAL::squared_distance(p1,p2));
            //std::cout<<"dist:"<<distance_to_first[i]<<std::endl;
        }
        std::sort(distance_to_first.begin(),distance_to_first.end());

        K::FT opt_distance = std::abs(distance_to_first[1]-distance_to_first[0]);
        opt_distance = std::min(opt_distance,std::abs(distance_to_first[0]));
        opt_distance = std::min(opt_distance,std::abs(distance_to_first[1]));
        //std::cout<<"startdist:"<<opt_distance<<std::endl;
        //std::cout<<"mindist:"<<abs(distance_to_first[0])<<std::endl;
        //std::cout<<"mindist:"<<abs(distance_to_first[1])<<std::endl;
        //std::cout<<"optdist:"<<opt_distance<<std::endl;
        for( int i = 2; i < ngrapes-1; i++ ) {
            opt_distance = std::min(opt_distance, std::abs(distance_to_first[i]-distance_to_first[i-1]));
            //std::cout<<"subdist:"<<abs(distance_to_first[i]-distance_to_first[i-1])<<std::endl;
            //std::cout<<"mindist:"<<abs(distance_to_first[i])<<std::endl;
            opt_distance = std::min(opt_distance,std::abs(distance_to_first[i]));
        }
        //for( int i = 0; i < ngrapes-1; i++ ) {
            //std::cout<<"waw "<<(int)floor_to_double(-50*distance_to_first[i])<<std::endl;
        //}

        std::cout<<(int)-floor_to_double(-50*opt_distance)<<std::endl;

    } else { 
        std::vector<K::FT> distances(ngrapes);;
        int i = 0;

        bool ninit = true;
        K::FT min_distance = -1.;
        for( Vertex_iterator vi = t.finite_vertices_begin(); vi != t.finite_vertices_end(); ++vi ) {
            Triangulation::Point p1 = vi->point();

            Triangulation::Vertex_circulator vc = t.incident_vertices(vi);
            Triangulation::Point p2 = vc->point();
            if( ninit ) {
                min_distance = CGAL::squared_distance(p1,p2);
                ninit = false;
            } else {
                min_distance = std::min(min_distance,CGAL::squared_distance(p1,p2));
            }
            vc++;
            while( vc != t.incident_vertices(vi) ) {
                Triangulation::Point p2 = vc->point();
                min_distance = std::min(min_distance,CGAL::squared_distance(p1,p2));
                vc++;
            }
        }
        std::cout<<(int)-floor_to_double(-50*sqrt(min_distance))<<std::endl;
    }
    

}

int main()
{
    int n;
    std::cin>>n;
    while( n != 0 ) {
        bet_on_graypes(n);
        std::cin>>n;
    }

}
