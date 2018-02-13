#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


void compute_intersections( int nlines )
{
    long long x,y,a,b;
    cin>>x>>y>>a>>b;

    K::Point_2 start(x,y);
    K::Point_2 end(a,b);
    K::Ray_2 ray(start,end);
    
    vector<K::Segment_2> walls(nlines);
    for( int i = 0; i < nlines; i++ ) {
        cin>>x>>y>>a>>b;
        K::Point_2 s_start(x,y);
        K::Point_2 s_end(a,b);
        walls[i] = K::Segment_2(s_start,s_end);
    }
    //Compute intersections
    bool intersected = false;
    K::Point_2 intersection;
    K::FT distance;
    for( int i = 0; i < nlines; i++ ) {

        if(CGAL::do_intersect(ray,walls[i])) {
            auto result = CGAL::intersection(ray,walls[i]);
            if( const K::Point_2 *p = boost::get<K::Point_2>(&*result) ) {
                K::Point_2 new_intersection = *p;
                K::FT new_distance          = CGAL::squared_distance(start,new_intersection);
                if( intersected ) {
                    if( distance > new_distance ) {
                        intersection = new_intersection;
                        distance = new_distance;
                    }
                } else {
                    intersection = new_intersection;
                    distance = new_distance;
                }

            } else if( const K::Segment_2 *s = boost::get<K::Segment_2>(&*result) ) {
                K::Point_2 new_intersection;
                if( CGAL::squared_distance(s->vertex(0),start) < CGAL::squared_distance(s->vertex(1),start) ) {
                    new_intersection = s->vertex(0);
                } else {
                    new_intersection = s->vertex(1);
                }
                K::FT new_distance          = CGAL::squared_distance(start,new_intersection);
                if( intersected ) {
                    if( distance > new_distance ) {
                        intersection = new_intersection;
                        distance = new_distance;
                    }
                } else {
                    intersection = new_intersection;
                    distance = new_distance;
                }
            }
            intersected = true;
        }


    }
    if(intersected) {
        //cout<<intersection.x()<<","<<intersection.y()<<endl;
        x = floor_to_double(intersection.x());
        y = floor_to_double(intersection.y());
        cout<<x<<" "<<y<<endl;
    } else {
        cout<<"no"<<endl;
    }
    
}


int main()
{
    ios_base::sync_with_stdio(false);
    int n;
    cin>>n;
    while( n > 0 ) {
        compute_intersections(n);
        cin>>n;
    }
}
