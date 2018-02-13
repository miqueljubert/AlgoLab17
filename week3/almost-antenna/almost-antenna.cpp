#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <map>

using namespace std;


// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K2;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

typedef K::Point_2 Point;

typedef K2::Point_2 Point_sqrt;
typedef CGAL::Min_circle_2_traits_2<K2>  Traits_sqrt;
typedef CGAL::Min_circle_2<Traits_sqrt>      Min_circle_sqrt;


double floor_to_double(const K2::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void run(int n) {
    vector<Point> points(n);
    vector<Point_sqrt> points_sqrt(n);
    map<Point,int> point_index;

    for (int i = 0; i < n; ++i) {
        long x,y;
        cin>>x>>y;
        points[i] = Point(x,y);
        point_index[points[i]] = i;
        points_sqrt[i] = Point_sqrt(x,y);
    }


    Min_circle mc(points.begin(), points.end(), true);
    //Traits::Circle c = mc.circle();
    vector<int> supports;
    for( auto it = mc.support_points_begin(); it != mc.support_points_end(); it++ ) {
        supports.push_back(point_index[*it]);
        //cout<<supports.back()<<endl;
    }
    //cout<<endl;
    
    K2::FT m = -1;
    for( int i = 0; i < supports.size(); i++ ) {
        vector<Point> semi_points = points;
        int sup = supports[i];
        semi_points.erase(semi_points.begin()+sup);
        int p = 0;
        Min_circle mct(semi_points.begin(), semi_points.end(), true);
        //Min_circle mct(points.begin(), points.begin()+supports[i], true);
        //mct.insert(points.begin()+supports[i]+1, points.end());

        //vector<Point> semi_points(n-1);
        //for( int j = 0; j < n; j++ ) {
        //    if( j == supports[i] ) continue;
        //    semi_points[p] = points[j];
        //    p++;
        //}
        //Min_circle mct(semi_points.begin(), semi_points.end(), true);

        //Min_circle_sqrt mcts;
        vector<Point_sqrt> ps;
        for( auto it = mct.support_points_begin(); it != mct.support_points_end(); it++ ) {
            ps.push_back(points_sqrt[point_index[*it]]);
        }
        Min_circle_sqrt mcts(ps.begin(),ps.end(),true);

        Traits_sqrt::Circle c = mcts.circle();
        if( m == -1 ) m = c.squared_radius();
        else m = min(m,c.squared_radius());
        
    }
    //cout <<sqrt(c.squared_radius()) <<endl;
    cout<<-(long)floor_to_double(-sqrt(m))<<endl;

    //std::cout << c.center() << " " << c.squared_radius() << std::endl;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    int n;
    cin>>n;
    while( n != 0 ){
        run(n);
        cin>>n;
    }
}
