#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#undef NDEBUG

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;


bool is_face_accessible( const Triangulation::Face_handle &start, const K::FT &d,
                            Triangulation &t,
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
    //for( int i = 0; i < 3; i++ ) {
        //std::cerr<<t.dual(start->neighbor(i))<<std::endl;
    //}
    visited.insert(start);
    Triangulation::Point p1 = t.dual(start);
    Triangulation::Point p2 = (start->vertex(0))->point();
    K::FT diameter = CGAL::squared_distance(p1,p2);//radius times 4, d^2 = 4r^2
    if( diameter < d ) {//Cannot be inside this 
        is_accessible[start] = false;
        return false;
    }
    
    for( int i = 0; i < 3; i++ ) {
        //Triangulation::Face f = *start;
        Triangulation::Point e1,e2;//points of the edge separating
        e1 = (start->vertex( (i+1) % 3 ))->point();
        e2 = (start->vertex( (i+2) % 3 ))->point();
        Triangulation::Face_handle fh = start->neighbor(i);

        
        //Check if the edge is large enough to cross
        K::FT edge_length = CGAL::squared_distance(e1,e2)/4;//Length of the edge divided by two squared
        if( edge_length >= d ) {//Can cross this edge
            if( t.is_infinite( fh ) ) { //If we get outside, it is accessible
                is_accessible[start] = true;
                return true;
            }
            //If not, will the crossing be legal?
            p2 = t.dual(fh); //p2 is the next voronoi center
            if( p1 != p2 ) { //If we actually change voronoi region...
                K::Line_2 l(p1,p2);
                K::FT d1 = CGAL::squared_distance(l,p1);
                K::FT d2 = CGAL::squared_distance(l,p2);
                if( d1 < d or d2 < d ) { //If we get too close
                    continue;
                }
            }



            is_blocked = false;//We can leave this triangle
            if( not (visited.find(fh) != visited.end()) ) { //If we have not visited it
                if( is_face_accessible( fh, d, t, is_accessible, visited ) ) {//Visit it and check the result
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
    assert(false);
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

    Triangulation::Face_iterator fi,fe;
    for( int i = 0; i < nhealthy; i++ ) {
        Triangulation::Point p_healthy = healthy_people[i];
        Triangulation::Face_handle f = t.locate(p_healthy);

        bool too_close = false;
        for( int j = 0; j < 3; j++ ) {
           Triangulation::Point p_t = f->vertex(j)->point();
           if( CGAL::squared_distance(p_t,p_healthy) < distance[i] ) {
               too_close = true;
               break;
           }
        }
        if( too_close ) { 
            std::cout<<"n";
            continue;
        }

        std::unordered_set<Triangulation::Face_handle> visited;
        //Check if we have memoized the face
        if( is_face_accessible(f, distance[i], t, accessible_faces, visited) ) std::cout<<"y";
        else                     std::cout<<"n";
        //std::cout<<fa<<std::endl;
        //std::cout<<*f<<std::endl;
    }
    std::cout<<std::endl;



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
