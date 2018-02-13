#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point Point;

using namespace std;


void run()
{
    long n,m,h,imax;
    cin>>n>>m>>h>>imax;
    
    vector<Point> sensor_p(n);
    vector<long> sensor_e(n);
    for( long i = 0; i < n; i++ ) {
        long x,y,e;
        cin>>x>>y>>e;
        sensor_p[i] = Point(x,y);
        sensor_e[i] = e;
    }
    vector<Point> mpe_p(m);
    for( long i = 0; i < m; i++ ) {
        long x,y;
        cin>>x>>y;
        mpe_p[i] = Point(x,y);
    }
    vector<Point> hman_p(h);
    for( long i = 0; i < h; i++ ) {
        long x,y;
        cin>>x>>y;
        hman_p[i] = Point(x,y);
    }


    //Get ranges to closest ones
    vector<ET> max_range(m);
    if( h > 0 ) {
        Triangulation t;
        t.insert(hman_p.begin(),hman_p.end());

        for( int i = 0; i < m; i++ ) {
            Point p = t.nearest_vertex(mpe_p[i])->point();
            max_range[i] = CGAL::squared_distance(p,mpe_p[i]);
            //cout<<max_range[i]<<endl;
        }
    }

    

    //for( int l = 1; l <= m; l++ ) {//For each [0,l] range
    bool exp = true;
    long start = 1;
    long end = 1;
    long valid = -1;
    while( start <= end ) {
        int l;
        if( exp ) {
            l = end;
        } else {
            l = (start+end)/2;
        }

        Program lp (CGAL::LARGER, false, 0, false, 0);
        for( int i = 0; i < l; i++ ) {
            lp.set_a(i,n,1);

            lp.set_c(i,1);

            lp.set_l(i,true,0);
        }
        lp.set_b(n,imax);
        lp.set_r(n,CGAL::SMALLER);

        for( int i = 0; i < n; i++ ) {//For each sensor (equation)
            for( int j = 0; j < l; j++ ) {//For each MPE (variable)
                ET range = CGAL::squared_distance(mpe_p[j],sensor_p[i]);
                if( h > 0 and max_range[j] <= range ) continue;
                lp.set_a(j,i,ET(1)/range);
            }
            lp.set_b(i,sensor_e[i]);
        }
        Solution s = CGAL::solve_linear_program(lp,ET());
        
        //if( s.is_optimal() ) {
        //    cout<<l<<endl;
        //    return;
        //}
        bool ok = s.is_optimal();
        if( exp ) {
            if( ok ) {
                end = end-1; 
                valid = end;
                exp = false;
            } else {
                if( end == m ) break;//break
                start = end;
                end = std::min(m,2*end);
            }
        } else {
            if( ok ) {
                end = l-1;
                valid = end;
            } else {
                start = l+1;
            }
        }
    }
    if( valid == -1 ) cout<<"impossible"<<endl;
    else cout<<valid+1<<endl;
    //cout<<"impossible"<<endl;


}



int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
