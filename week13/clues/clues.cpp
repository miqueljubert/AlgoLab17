#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <map>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef Triangulation::Vertex_circulator  Vertex_circulator;
typedef Triangulation::Face_iterator  Face_iterator;
typedef Triangulation::Point Point;

typedef K::Circle_2 Circle;

using namespace std;

  //std::size_t n;
  //std::cin >> n;
  //std::vector<K::Point_2> pts;
  //pts.reserve(n);
  //for (std::size_t i = 0; i < n; ++i) {
  //  K::Point_2 p;
  //  std::cin >> p;
  //  pts.push_back(p);
  //}
  //Triangulation t;
  //t.insert(pts.begin(), pts.end());
  //for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
  //  std::cout << t.segment(e) << "\n";




bool valid_tag_dfs( int u, bool t, vector<bool> &visited,
                    vector<bool> &tag, const vector<vector<long>> &g)
{
    if( visited[u] and tag[u] != t ) return false;
    if( visited[u] ) return true;

    visited[u] = true;
    tag[u] = t;
    for( int i = 0; i < g[u].size(); i++ ) {
        if( not valid_tag_dfs( g[u][i], not t, visited, tag, g ) ) return false;
    }
    return true;
}

bool is_valid_network( long n, const vector<vector<long>> &g ) {
    vector<bool> visited(n,false);
    vector<bool> tag(n,false);

    for( long i = 0; i < n; i++ ) {
        if( visited[i] ) continue;
        if( not valid_tag_dfs(i,false,visited,tag,g) ) return false;
        
    }

    return true; //If we can do this bfs without any conflicts, the network is legal
}





void do_dfs( int u, vector<bool> &visited, const vector<vector<long>> &g)
{
    if( visited[u] ) return;
    visited[u] = true;
    for( int i = 0; i < g[u].size(); i++ ) {
        do_dfs( g[u][i], visited, g );
    }
}


void run()
{


    long n,m,r;
    cin>>n>>m>>r;
    K::FT rs = K::FT(r)*K::FT(r);

    vector<vector<long>> G(n);

    map<Point,int> station_index;
    vector<Point> stations(n);
    for( int i = 0; i < n; i++ ) {
        long x,y;
        cin>>x>>y;
        stations[i] = Point(x,y);
        station_index[stations[i]] = i;
    }

    vector<Point> start(m);
    vector<Point> end(m);
    for( int i = 0; i < m; i++ ) {
        long x,y;
        cin>>x>>y;
        start[i] = Point(x,y);
        cin>>x>>y;
        end[i] = Point(x,y);
    }


    Triangulation t;
    t.insert(stations.begin(),stations.end());
    
    //for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    //    vector<Point> ps(3);
    //    for( int i = 0; i < 3; i++ ) {
    //        ps[i] = f->vertex(i)->point();
    //    }
    //    K::FT d1 = CGAL::squared_distance(ps[0],ps[1]);
    //    K::FT d2 = CGAL::squared_distance(ps[1],ps[2]);
    //    K::FT d3 = CGAL::squared_distance(ps[2],ps[0]);
    //    if( d1 <= rs and d2 <= rs and d3 <= rs ) {
    //        for( int i = 0; i < m; i++ ) cout<<"n"; cout<<endl;
    //        return;
    //    }
    //}
    for( Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v ){

        vector<Point> neighbors;
        Vertex_circulator vi = t.incident_vertices(v);
        do {
            if( t.is_infinite(vi) ) {
                vi++;
                continue;
            }
            neighbors.push_back(vi->point());
            vi++;
        } while( vi != t.incident_vertices(v) );
        

        Point p = v->point();
        for( int i = 0; i < neighbors.size(); i++ ) {
            K::FT d1 = CGAL::squared_distance(neighbors[i],p);
            for( int j = i+1; j < neighbors.size(); j++ ) {
                K::FT d2 = CGAL::squared_distance(neighbors[j],p);
                K::FT d3 = CGAL::squared_distance(neighbors[i],neighbors[j]);
                if( d1 <= rs and d2 <= rs and d3 <= rs ) {
                    for( int i = 0; i < m; i++ ) cout<<"n"; cout<<endl;
                    return;
                }
            }
        }
        
    }
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        //std::cout << t.segment(e) << "\n";
        Point p1 = e->first->vertex( (e->second+1) % 3)->point();
        Point p2 = e->first->vertex( (e->second+2) % 3)->point();
        long i1 = station_index[p1];
        long i2 = station_index[p2];
        K::FT d = CGAL::squared_distance(p1,p2);
        if( d <= rs ) {
            G[i1].push_back(i2);
            G[i2].push_back(i1);
        }
    }
    if( not is_valid_network(n,G) ) {
        for( int i = 0; i < m; i++ ) cout<<"n"; cout<<endl;
        return;
    }
    
    for( long i = 0; i < m; i++ ) {
        //Point p = Point(start[i],end[i]);
        K::FT d = CGAL::squared_distance(start[i],end[i]);
        if( d <= rs ) {//They are close enough
            cout<<"y";
            continue;
        }
        
        Point p1 = t.nearest_vertex(start[i])->point();
        Point p2 = t.nearest_vertex(end[i])->point();
        K::FT d1 = CGAL::squared_distance(p1,start[i]);
        K::FT d2 = CGAL::squared_distance(p2,end[i]);
        if( d1 > rs or d2 > rs ) {//Cant reach
            cout<<"n";
            continue;
        }

        int i1 = station_index[p1];
        int i2 = station_index[p2];
        
        vector<bool> vb(n,false);
        do_dfs(i1,vb,G);
        bool reachable = vb[i2];
        if( reachable ) cout<<"y";
        else cout<<"n";

    }
    cout<<endl;
    

    //cout<<val<<endl;


}
int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) {
        run();
    }
}
