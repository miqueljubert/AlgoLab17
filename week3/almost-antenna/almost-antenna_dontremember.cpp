#include <iostream>
#include <vector>
#include <limits>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;


double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x) a -= 1;
    while (a < x) a += 1;
    return a;
}


double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

long int get_min_circle( int ncitizens )
{
    Min_circle min_circle;
    vector<K::Point_2> citizens(ncitizens);
    long int x,y;
    for( int i = 0; i < ncitizens; i++ ) {
        cin>>x>>y;
        citizens[i] = K::Point_2(x,y);
        min_circle.insert(citizens[i]);
    }
    //cout<<"o:"<<old_supports.size()<<endl;
    K::FT min_rad;
    if( ncitizens == 1 || ncitizens == 2 ) {
        return 0;
    } else if( ncitizens == 3 ) {
        K::FT d1,d2,d3;
        d1 = CGAL::squared_distance(citizens[0],citizens[1]);
        d2 = CGAL::squared_distance(citizens[1],citizens[2]);
        d3 = CGAL::squared_distance(citizens[0],citizens[2]);
        long int ret = ceil_to_double(sqrt(min(d1,min(d2,d3)))/2.);
        return ret;
    } else {
        if( min_circle.number_of_support_points() > 3 ) {
            return (long int) ceil_to_double(sqrt(min_circle.circle().squared_radius()));
        }

        vector<K::Point_2> supports(min_circle.support_points_begin(),min_circle.support_points_end());
        K::FT min_radius = min_circle.circle().squared_radius();

        min_circle.clear();
        //Build each circle removing one of the support points

        for( int i = 0; i < supports.size(); i++ ) {//O(1), max is 3
            bool removed = false;
            for( int j = 0; j < citizens.size(); j++ ) {
                if( removed or citizens[j] != supports[i] ) {//If removed once always true
                    if( min_circle.circle().has_on_boundary(citizens[j]) or min_circle.circle().has_on_unbounded_side(citizens[j]) ) {
                        min_circle.insert(citizens[j]);
                    }

                } else {
                    removed = true;
                }
            }
            
            min_radius = min(min_radius,min_circle.circle().squared_radius());
            min_circle.clear();
        }
        //cout<<min_radius<<endl;

        return (long int)ceil_to_double(sqrt(min_radius));
        

    }

}


int main()
{
    int n;
    cin>>n;
    while(n != 0){
        cout<<get_min_circle(n)<<endl;
        cin>>n;
    }
}
