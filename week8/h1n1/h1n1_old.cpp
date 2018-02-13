#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <unordered_map>
#include <unordered_set>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;


bool is_face_accessible( const Triangulation::Face_handle &start, const K::FT &d,
                            Triangulation t,
                            std::unordered_map<Triangulation::Face_handle, bool> &is_accessible,
                            std::unordered_set<Triangulation::Face_handle> &visited )
{
    if( is_accessible.find(start) != is_accessible.end() ) {
        return is_accessible[start];
    }
    
    if( t.is_infinite( start ) ) { //If we get outside
        is_accessible[start] = true;
        return true;
    }

    bool is_blocked = true;
    Triangulation::Point p1 = t.dual(start);
    visited.insert(start);

    for( int i = 0; i < 3; i++ ) {
        //Triangulation::Face f = *start;
        Triangulation::Point p2 = t.dual(start->neighbor(i));
        Triangulation::Point e1,e2;//points of the edge separating
        e1 = (start->vertex( (i+1) % 3 ))->point();
        e2 = (start->vertex( (i+2) % 3 ))->point();

        K::Line_2 l(p1,p2);
        K::FT d1 = CGAL::squared_distance(l,e1);
        K::FT d2 = CGAL::squared_distance(l,e2);
        Triangulation::Face_handle fh = start->neighbor(i);
        if( d1 < d and d2 < d ) {//Can cross this edge
            is_blocked = false;
            if( not (visited.find(fh) != visited.end()) ) { //If we have not visited it
                if( is_face_accessible( fh, d, t, is_accessible, visited ) ) {
                    is_accessible[start] = true;
                    return true;
                }
            }
        }
    }

    //If all three sides are blocked
    if( is_blocked ) is_accessible[start] = false;
    return false;
}

void walk_out( int nsick )
{
    std::vector<Triangulation::Point> sick_people(nsick);
    for ( int i = 0; i < nsick; i++) {
        std::cin >> sick_people[i];
    }

    // construct triangulation
    Triangulation t;
    t.insert(sick_people.begin(), sick_people.end());

    int nhealthy;
    std::cin >> nhealthy;
    std::vector<Triangulation::Point> healthy_people(nhealthy);
    std::vector<K::FT> distance(nhealthy);
    for( int i = 0; i < nhealthy; i++ ) {
        std::cin >> healthy_people[i];
        std::cin >> distance[i];
    }

    std::unordered_map<Triangulation::Face_handle, bool> accessible_faces;

    for( int i = 0; i < nhealthy; i++ ) {
        Triangulation::Point p_healthy = healthy_people[i];
        Triangulation::Face_handle f = t.locate(p_healthy);

        std::unordered_set<Triangulation::Face_handle> visited;
        //Check if we have memoized the face
        if( is_face_accessible(f, distance[i], t, accessible_faces, visited) ) std::cout<<"y"<<std::endl;
        else                     std::cout<<"n"<<std::endl;
        //std::cout<<fa<<std::endl;
        //std::cout<<*f<<std::endl;
    }



    // output all edges
    //for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        //std::cout << t.segment(e) << "\n";
}

int main()
{
    int n;
    std::cin>>n;
    while( n != 0 ) {
        walk_out(n);
        std::cin>>n;
    }
}
