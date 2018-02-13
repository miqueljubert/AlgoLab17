#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

bool compute_intersections( int n )
{
    long long x,y,a,b;
    cin>>x>>y>>a>>b;
    K::Point_2  start(x,y);
    K::Point_2  end(a,b);
    K::Ray_2    ray(start,end);
    bool rval = false;
    for( int i = 0; i < n; i++ ) {
        cin>>x>>y>>a>>b;
        K::Point_2      s_start(x,y);
        K::Point_2      s_end(a,b);
        K::Segment_2    seg(s_start,s_end);
        if( not rval and CGAL::do_intersect(ray,seg)) rval = true;
    }
    return rval;

}



int main()
{
    int n;
    cin>>n;
    while( n != 0) {
        compute_intersections(n)? cout<<"yes"<<endl : cout<<"no"<<endl;
        cin>>n;
    }
}
