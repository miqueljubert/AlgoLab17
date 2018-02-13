#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;


double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x) a -= 1;
    while (a < x) a += 1;
    return a;
}


void get_antenna( int ncitizens )
{
    vector<K::Point_2> citizens(ncitizens);
    long int x,y;
    for( int i = 0; i < ncitizens; i++ ) {
        cin>>x>>y;
        citizens[i] = K::Point_2(x,y);
    }
    Min_circle mc(citizens.begin(),citizens.end(),true);
    K::FT r = sqrt(mc.circle().squared_radius());
    long int ret = ceil_to_double(r);
    cout<<ret<<endl;
}

int main()
{
    int n;
    cin>>n;
    while(n != 0) {
        get_antenna(n);
        cin>>n;
    }
}
