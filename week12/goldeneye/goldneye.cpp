#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;
using namespace boost;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point Point;
typedef disjoint_sets_with_storage<> union_find;

void run()
{
    //union_find d1;
    int n,m;
    long p;
    cin>>n>>m>>p;


    vector<Point> jammers(n);
    map<Point,int> jammer_index;
    for( int i = 0; i < n; i++ ) {
        long x,y;
        cin>>x>>y;
        Point p(x,y);
        jammers[i] = p;
        jammer_index[p] = i;
    }

    vector<Point> start(m);
    vector<Point> end(m);
    for( int i = 0; i < m; i++ ) {
        long x,y;
        cin>>x>>y;
        Point p(x,y);
        start[i] = p;
        cin>>x>>y;
        p = Point(x,y);
        end[i] = p;
    }


    Triangulation t;
    t.insert(jammers.begin(), jammers.end());

    vector<pair<K::FT,pair<int,int>>> values;

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Point p1 = e->first->vertex( (e->second+1) % 3 )->point();
        Point p2 = e->first->vertex( (e->second+2) % 3 )->point();
        K::FT dist = CGAL::squared_distance(p1,p2);
        int i1 = jammer_index[p1];
        int i2 = jammer_index[p2];
        values.push_back(pair<K::FT,pair<int,int>>(dist,pair<int,int>(i1,i2)));
    }
    sort(values.begin(),values.end());
    

    union_find set_p(n),set_a(n),set_b(n);

    //Initialize the sets
    for( int i = 0; i < n; i++ ) {
        set_p.make_set(i);
        set_a.make_set(i);
        set_b.make_set(i);
    }
    for( unsigned int i = 0; i < values.size(); i++ ) {//build p power union set
        K::FT d = values[i].first;
        if( d > p ) break;
        int i1 = values[i].second.first;
        int i2 = values[i].second.second;
        
        set_p.union_set(i1,i2);
    }

    vector<bool> valid(m,false);
    for( unsigned int i = 0; i < m; i++ ) {
        Point s = start[i];
        Point e = end[i];

        Point ps = t.nearest_vertex(s)->point();
        Point pe = t.nearest_vertex(e)->point();

        K::FT ds = CGAL::squared_distance(ps,s);
        K::FT de = CGAL::squared_distance(pe,e);

        int is = jammer_index[ps];
        int ie = jammer_index[pe];
        if( 4*ds <= p and 4*de <= p and (set_p.find_set(is) == set_p.find_set(ie)) ) {
            valid[i] = true;
        }
    }
    K::FT a = -1;
    int a_idx = 0;
    for( unsigned int i = 0; i < m; i++ ) {
        Point s = start[i];
        Point e = end[i];

        Point ps = t.nearest_vertex(s)->point();
        Point pe = t.nearest_vertex(e)->point();

        K::FT ds = CGAL::squared_distance(ps,s);
        K::FT de = CGAL::squared_distance(pe,e);

        int is = jammer_index[ps];
        int ie = jammer_index[pe];

        a = max(a,max(4*de,4*ds));
        while( set_a.find_set(is) != set_a.find_set(ie) ) {
            K::FT d = values[a_idx].first;

            int i1 = values[a_idx].second.first;
            int i2 = values[a_idx].second.second;

            set_a.union_set(i1,i2);
            a = max(a,d);
            a_idx++;
        }
    }
    K::FT b = 0;
    int b_idx = 0;
    for( unsigned int i = 0; i < m; i++ ) {
        if( not valid[i] ) continue;
        Point s = start[i];
        Point e = end[i];

        Point ps = t.nearest_vertex(s)->point();
        Point pe = t.nearest_vertex(e)->point();

        K::FT ds = CGAL::squared_distance(ps,s);
        K::FT de = CGAL::squared_distance(pe,e);

        int is = jammer_index[ps];
        int ie = jammer_index[pe];

        b = max(b,max(4*de,4*ds));
        while( set_b.find_set(is) != set_b.find_set(ie) ) {
            K::FT d = values[b_idx].first;

            int i1 = values[b_idx].second.first;
            int i2 = values[b_idx].second.second;

            set_b.union_set(i1,i2);
            b = max(b,d);
            b_idx++;
        }
    }

    for( int i = 0; i < m; i++ ) {
        if( valid[i] ) cout<<"y";
        else cout<<"n";
    }
    cout<<endl;
    cout<<(long)a<<endl;
    cout<<(long)b<<endl;

}


//int main()
//{
//  // read number of points
//  std::size_t n;
//  std::cin >> n;
//  // read points
//  std::vector<K::Point_2> pts;
//  pts.reserve(n);
//  for (std::size_t i = 0; i < n; ++i) {
//    K::Point_2 p;
//    std::cin >> p;
//    pts.push_back(p);
//  }
//  // construct triangulation
//  Triangulation t;
//  t.insert(pts.begin(), pts.end());
//  // output all edges
//  for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
//    std::cout << t.segment(e) << "\n";
//}
int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();

}
