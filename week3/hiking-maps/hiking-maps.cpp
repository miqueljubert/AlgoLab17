#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <limits>
#include <iostream>
#include <cmath>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Line_2 Line;
typedef K::Segment_2 Segment;
typedef K::Triangle_2 Triangle;


double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}



void run()
{
    long n,m;
    cin>>m>>n;

    vector<Point>   points(m);
    vector<Segment> segments(m-1);
    for( int i = 0; i < m; i++ ) {
        long x,y;
        cin>>x>>y;
        points[i] = Point(x,y);
        if( i > 0 ) {
            segments[i-1] = Segment(points[i],points[i-1]);
        }
    }

    vector<vector<bool>> contained(n,vector<bool>(m,false));
    for( int i = 0; i < n; i++ ) {
        long x1,y1,x2,y2;

        cin>>x1>>y1>>x2>>y2;
        Point p1(x1,y1),p2(x2,y2);
        cin>>x1>>y1>>x2>>y2;
        Point p3(x1,y1),p4(x2,y2);
        cin>>x1>>y1>>x2>>y2;
        Point p5(x1,y1),p6(x2,y2);

        for( int j = 0; j < m; j++ ) {
            auto o1 = CGAL::orientation(p1,p2,p3);
            auto o2 = CGAL::orientation(p3,p4,p5);
            auto o3 = CGAL::orientation(p5,p6,p1);
            bool orient1,orient2,orient3;

            if( o1 == CGAL::LEFT_TURN ) orient1 = true; //left
            else orient1 = false;                       //RIGHT
            if( o2 == CGAL::LEFT_TURN ) orient2 = true; //left
            else orient2 = false;                       //RIGHT
            if( o3 == CGAL::LEFT_TURN ) orient3 = true; //left
            else orient3 = false;                       //RIGHT

            auto or1 = CGAL::orientation(p1,p2,points[j]);
            auto or2 = CGAL::orientation(p3,p4,points[j]);
            auto or3 = CGAL::orientation(p5,p6,points[j]);
            bool side1,side2,side3;
            if( orient1 ) side1 = (or1 == CGAL::LEFT_TURN or or1 == CGAL::COLLINEAR);
            else          side1 = (or1 == CGAL::RIGHT_TURN or or1 == CGAL::COLLINEAR);
            if( orient2 ) side2 = (or2 == CGAL::LEFT_TURN or or2 == CGAL::COLLINEAR);
            else          side2 = (or2 == CGAL::RIGHT_TURN or or2 == CGAL::COLLINEAR);
            if( orient3 ) side3 = (or3 == CGAL::LEFT_TURN or or3 == CGAL::COLLINEAR);
            else          side3 = (or3 == CGAL::RIGHT_TURN or or3 == CGAL::COLLINEAR);

           contained[i][j] = side1 and side2 and side3;
        }

    }
    

    int n_covered = 0;
    vector<int> covered(m-1,0);
    int start = 0;
    int end = 0;
    for( int i = 0; i < m-1; i++ ) {
        if( contained[0][i] and contained[0][i+1] ) {
            covered[i]++;
            n_covered++;
        }
    }
    
    int length = numeric_limits<int>::max();
    while( end < n ) {
        if( n_covered == m-1 ) {

            length = min(length,end-start);

            for( int i = 0; i < m-1; i++ ) {
                if( contained[start][i] and contained[start][i+1] ) {
                    if( covered[i] == 1 ) n_covered--;
                    covered[i]--;
                }
            }

            start++;
        } else {

            end++;
            if( end == n ) break;

            for( int i = 0; i < m-1; i++ ) {
                if( contained[end][i] and contained[end][i+1] ) {
                    if( covered[i] == 0 ) n_covered++;
                    covered[i]++;
                }
            }

        }
    }
    cout<<length+1<<endl;

}


int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
