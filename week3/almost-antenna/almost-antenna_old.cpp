#include <iostream>
#include <vector>
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
    }

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
        K::Point_2 left_out;
        K::FT left_dist;
        K::FT r0,r1,r2,r3;
        
        //Compute 1 2 3 radius-0 out
        min_circle.insert(citizens[1]);
        min_circle.insert(citizens[2]);
        min_circle.insert(citizens[3]);
        r0 = min_circle.circle().squared_radius();
        min_circle.clear();

        //Compute 0 2 3 radius-1 out
        min_circle.insert(citizens[0]);
        min_circle.insert(citizens[2]);
        min_circle.insert(citizens[3]);
        r1 = min_circle.circle().squared_radius();
        min_circle.clear();

        //Compute 0 1 3 radius-2 out
        min_circle.insert(citizens[0]);
        min_circle.insert(citizens[1]);
        min_circle.insert(citizens[3]);
        r2 = min_circle.circle().squared_radius();
        min_circle.clear();

        //Compute 0 1 2 radius-3 out
        min_circle.insert(citizens[0]);
        min_circle.insert(citizens[1]);
        min_circle.insert(citizens[2]);
        r3 = min_circle.circle().squared_radius();
        min_circle.clear();

        K::FT rmin = min(r0,min(r1,min(r2,r3)));
        if( rmin == r0 ) {
            min_circle.insert(citizens[1]);
            min_circle.insert(citizens[2]);
            min_circle.insert(citizens[3]);
            left_out = citizens[0];
        } else if( rmin == r1 ) {
            min_circle.insert(citizens[0]);
            min_circle.insert(citizens[2]);
            min_circle.insert(citizens[3]);
            left_out = citizens[1];
        } else if( rmin == r2 ) {
            min_circle.insert(citizens[0]);
            min_circle.insert(citizens[1]);
            min_circle.insert(citizens[3]);
            left_out = citizens[2];
        } else if( rmin == r3 ) {
            min_circle.insert(citizens[0]);
            min_circle.insert(citizens[1]);
            min_circle.insert(citizens[2]);
            left_out = citizens[3];
        } else {
            cout<<"ERROR, wrong r"<<endl;
        }


        Min_circle test_circle;

        test_circle.insert( min_circle.support_points_begin(), min_circle.support_points_end() );
        test_circle.insert( left_out );
        left_dist = test_circle.circle().squared_radius();
        test_circle.clear();

        for( int i = 4; i < ncitizens; i++ ) {
            test_circle.insert( min_circle.support_points_begin(), min_circle.support_points_end() );
            test_circle.insert( citizens[i] );
            K::FT new_dist = test_circle.circle().squared_radius();
            test_circle.clear();
            if( new_dist > left_dist ) {
                min_circle.insert(left_out);
                left_out = citizens[i];

                test_circle.insert( min_circle.support_points_begin(), min_circle.support_points_end() );
                test_circle.insert( left_out );
                left_dist = test_circle.circle().squared_radius();
                test_circle.clear();
            } else {
                min_circle.insert(citizens[i]);

                test_circle.insert( min_circle.support_points_begin(), min_circle.support_points_end() );
                test_circle.insert( left_out );
                left_dist = test_circle.circle().squared_radius();
                test_circle.clear();
            }
        }

    }
    long int radius = ceil_to_double(sqrt(min_circle.circle().squared_radius()));
    return radius;

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
