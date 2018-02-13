#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Vertex_handle Vertex_handle;


// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

    
void pewpew()
{
    long n_particles, n_shots, n_hunters;
    std::cin>>n_particles>>n_shots>>n_hunters;

    long energy;
    std::cin>>energy;

    std::vector<long> densities(n_particles);
    std::vector<Point> particle_positions(n_particles);
    for( long i = 0; i < n_particles; i++ ) {
        long x,y;
        std::cin>>x>>y;
        particle_positions[i] = Point(x,y);

        std::cin>>densities[i];
        
    }
    std::vector<Point> shot_positions(n_shots);
    for( long i = 0; i < n_shots; i++ ) {
        long x,y;
        std::cin>>x>>y;
        Point p(x,y);
        shot_positions[i] = p;
    }
    std::vector<Point> hunter_positions(n_hunters);
    for( long i = 0; i < n_hunters; i++ ) {
        long x,y;
        std::cin>>x>>y;
        Point p(x,y);
        hunter_positions[i] = p;
    }

    //Create hunter triangulations to find maximum radius of each shot
    Triangulation t;
    t.insert(hunter_positions.begin(),hunter_positions.end());
    std::vector<ET> max_shot_radius(n_shots);


    if( n_hunters > 0 ) {
        for( long i = 0; i < n_shots; i++ ) {
            Vertex_handle v = (t.nearest_vertex(shot_positions[i]));
            max_shot_radius[i] = CGAL::squared_distance(v->point(),shot_positions[i]);
        }
    }

    std::vector<std::vector<ET>> particle_shot_distance(n_particles,std::vector<ET>(n_shots));
    for( long i = 0; i < n_particles; i++ ) {
        for( long j = 0; j < n_shots; j++ ) {
            particle_shot_distance[i][j] = CGAL::squared_distance(particle_positions[i],shot_positions[j]);
        }
    }

    Program lp (CGAL::LARGER, true, 0, false, 0);

    for( long i = 0; i < n_shots; i++ ) {//For each variable, that is, the energy of each shot...

        lp.set_c(i,1.); //Set equation per variable

        for( long j = 0; j < n_particles; j++ ) {//For each shot to this particle...
            if( n_hunters == 0 or particle_shot_distance[j][i] < max_shot_radius[i] ) {
                ET dist = ET(1.)/std::max(ET(1.),particle_shot_distance[j][i]);
                //std::cout<<1./dist<<std::endl;
                //std::cout<<dist<<std::endl;
                lp.set_a(i,j,dist);//Energy contribution of shot i to particle j
                //std::cout<<dist<<std::endl;

                //std::cout<<dist<<std::endl;
            }
        }

    }
    for( long j = 0; j < n_particles; j++ ) {
        lp.set_b(j,densities[j]); //Set equation per variable
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    //std::cout<<s<<std::endl;
    assert (s.solves_linear_program(lp));

    if( s.is_optimal() ) {
        //std::cout<<"obj:"<<s.objective_value()<<" "<<energy<<std::endl;
        if( s.objective_value() <= ET(energy) ) {
            std::cout<<"y"<<std::endl;
        } else {
            std::cout<<"n"<<std::endl;
        }
    } else {
        std::cout<<"n"<<std::endl;
    }

    /*
    for( long i = 0; i < n_particles; i++ ) {
        for( long j = 0; j < n_shots; j++ ) {
            
        }
    }
    */

    /*
    // by default, we have a nonnegative LP with Ax <= b
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    // now set the non-default entries
    const int X = 0;
    const int Y = 1;
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 7);  //  x + y  <= 7
    lp.set_a(X, 1, -1); lp.set_a(Y, 1, 2); lp.set_b(1, 4);  // -x + 2y <= 4
    lp.set_u(Y, true, 4);                                   //       y <= 4
    lp.set_c(Y, -32);                                       // -32y
    lp.set_c0(64);                                          // +64

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output solution
    std::cout << s;
    return 0;
    */
}
int main() {
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        pewpew();
    }
}
