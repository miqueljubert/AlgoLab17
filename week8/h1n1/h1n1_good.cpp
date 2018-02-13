#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Object.h>
#include <unordered_map>
#include <unordered_set>
#include <cassert>



typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>   Triangulation;
typedef Triangulation::Edge_iterator        Edge_iterator;
typedef Triangulation::Face_iterator        Face_iterator;
typedef Triangulation::Vertex_iterator      Vertex_iterator;
typedef Triangulation::Point                Point;
typedef Triangulation::Edge                 Edge;
typedef Triangulation::Face_handle          Face_handle;


bool is_face_accessible( const Triangulation::Face_handle &current, //We "are" currently at this point
                            const K::FT &d,
                            const Triangulation &t,
                            std::unordered_map<Triangulation::Face_handle, bool> &is_accessible,
                            std::unordered_set<Triangulation::Face_handle> &visited )
{
    
    if( is_accessible.find(current) != is_accessible.end() ) {//Avoid cycling over already-visited vertices
        return is_accessible[current];
    }

    if( t.is_infinite(current) ) {
        return true;
    }
    //From this point on: our face has a voronoi point! (therefore it's not infinite)
    
    if( visited.find(current) != visited.end() ) {//Avoid cycling over already-visited vertices
        return false;
    } else {
        visited.insert(current);
    }

    for( int i = 0; i < 3; i++ ) {
        Face_handle next = current->neighbor(i);
        Point e1 = current->vertex( (i+1)%3 )->point();
        Point e2 = current->vertex( (i+2)%3 )->point();
        K::FT distance = CGAL::squared_distance(e1,e2)/4;
        if( distance >= d ) {//can cross
            if(is_face_accessible(next,d,t,is_accessible,visited)) {
                is_accessible[current] = true;
                return true;
            }
        }
    }
    
    is_accessible[current] = false;
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

    std::unordered_map<Triangulation::Face_handle,bool> accessible_faces;
    
    Triangulation::Face_iterator fi,fe;
    for( int i = 0; i < nhealthy; i++ ) {
        Triangulation::Point p_healthy = healthy_people[i];
        Triangulation::Face_handle current = t.locate(p_healthy);
        
        Point nearest_point = t.nearest_vertex(p_healthy)->point();
        K::FT di = CGAL::squared_distance(nearest_point, p_healthy);
        if( di < distance[i] ) {
            std::cout<<"n";
            continue;
        }
        
        std::unordered_set<Triangulation::Face_handle> visited;

        if( is_face_accessible(current,distance[i],t,accessible_faces,visited) ) std::cout<<"y";
        else std::cout<<"n";
    }
    std::cout<<std::endl;





}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin>>n;
    while( n != 0 ) {
        walk_out(n);
        std::cin>>n;
    }
}
