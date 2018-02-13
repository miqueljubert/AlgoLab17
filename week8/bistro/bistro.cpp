#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


void closest_restaurants( int nrestaurants )
{
    // construct triangulation
    std::vector<Triangulation::Point> existing_restaurants(nrestaurants);
    Triangulation t;
    for (std::size_t i = 0; i < nrestaurants; ++i) {
        std::cin >> existing_restaurants[i];
    }
    t.insert(existing_restaurants.begin(),existing_restaurants.end());

    int nnewrestaurants;
    std::cin>>nnewrestaurants;
    std::vector<Triangulation::Point> target_restaurants(nnewrestaurants);
    for( int i = 0; i < nnewrestaurants; i++ ) {
        std::cin>>target_restaurants[i];
    }

    //std::cout<<"faces:"<<t.number_of_faces()<<std::endl;
    //if( t.number_of_faces() == 0 ) {
        //DEGENERATE CASE, everything is on a line...
        //std::cout<<"degen"<<std::endl;
    //}
    //} else {
        for( int i = 0; i < nnewrestaurants; i++ ) {
            Triangulation::Vertex_handle vh = t.nearest_vertex(target_restaurants[i]);
            Triangulation::Point p2 = target_restaurants[i];
            Triangulation::Point pe = vh->point();
            K::FT sq_dist = CGAL::squared_distance(p2,pe);
            std::cout<<(unsigned long)floor_to_double(sq_dist)<<std::endl;
        }


        // output all triangles
        //for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
            //std::cout << t.triangle(f) << "\n";
        //}
    //}
}

int main()
{
    int n;
    std::cin>>n;
    while( n != 0 ) {
        closest_restaurants(n);
        std::cin>>n;
    }
}
