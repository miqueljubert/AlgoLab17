#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <limits>
#include <unordered_map>
#include <map>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;
using namespace boost;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef disjoint_sets_with_storage<> union_find;



//    std::size_t n;
//    std::cin >> n;
//
//    std::vector<K::Point_2> pts;
//    pts.reserve(n);
//    for (std::size_t i = 0; i < n; ++i) {
//        K::Point_2 p;
//        std::cin >> p;
//        pts.push_back(p);
//    }
//
//    Triangulation t;
//    t.insert(pts.begin(), pts.end());
//
//    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
//        std::cout << t.segment(e) << "\n";
//    }



void run()
{
    int n;
    long r;
    cin>>n>>r;
    long rs = r*r; 
    map<Point,int> point_index;
    vector<Point> nodes(n);
    for( int i = 0; i < n; i++ ) {
        long x,y;
        cin>>x>>y;
        nodes[i] = Point(x,y);
        point_index[Point(x,y)] = i;
    }


    union_find uf(n);

    Triangulation t;
    unordered_map<int,int> set_size;

    int optimal_k = 1;//Can always just choose the last node
    t.insert(nodes[n-1]);
    uf.make_set(n-1);
    set_size[n-1] = 1;
    for( int i = n-2; i >= 0; i-- ) {
        Vertex_handle nh = t.insert(nodes[i]);

        uf.make_set(i);
        set_size[i] = 1;

        Vertex_circulator vh = t.incident_vertices(nh);
        do {
            if( not t.is_infinite( vh ) ) {
                K::FT dist = CGAL::squared_distance(nodes[i], vh->point());
                int neigh = point_index[vh->point()];
                if( dist <= rs and (uf.find_set(i) != uf.find_set(neigh))) {
                    //If we can reach and they are in different ccs
                    int n1 = set_size[uf.find_set(i)] + set_size[uf.find_set(neigh)];
                    uf.union_set(i,neigh);
                    int rep = uf.find_set(i);
                    set_size[rep] = n1;

                    optimal_k = max(optimal_k,min(i,n1));//n-k is the limiter of the best size
                }
            }
        } while( (++vh) != t.incident_vertices( nh ) );
    }

    cout<<optimal_k<<endl;


//    Triangulation t;
//    t.insert(pts.begin(), pts.end());
}



int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
