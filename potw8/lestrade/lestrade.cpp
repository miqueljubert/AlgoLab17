#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <map>
#include <unordered_map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef Triangulation::Point Point;

void investigate()
{
    
    long top_limit,u,v,w;
    Program lp (CGAL::LARGER, true, 0, true, 24);
    std::cin>>top_limit>>u>>v>>w;
    lp.set_b(0,u);
    lp.set_b(1,v);
    lp.set_b(2,w);
    
    Triangulation t;
    long n_agents, n_gang;
    std::cin>>n_agents>>n_gang;
    std::vector<Point> gang_positions(n_gang);
    std::vector<long> gang_u(n_gang);
    std::vector<long> gang_v(n_gang);
    std::vector<long> gang_w(n_gang);

    std::map<Point,long> points;
    for( long i = 0; i < n_gang; i++ ) {
        long x,y,ug,vg,wg;
        std::cin>>x>>y>>ug>>vg>>wg;
        gang_positions[i] = Point(x,y);
        points[Point(x,y)] = i;
        gang_u[i] = ug;
        gang_v[i] = vg;
        gang_w[i] = wg;
    }
    t.insert(gang_positions.begin(),gang_positions.end());


    //std::vector<long> follower(n_gang,-1);
    std::unordered_map<long,long> follower;
    std::vector<long> hourly_wage(n_agents);
    std::vector<Point> agent_position(n_agents);
    for( long i = 0; i < n_agents; i++ ) {
        long x,y,wage;
        std::cin>>x>>y>>wage;
        agent_position[i] = Point(x,y);
        hourly_wage[i] = wage;
        Point p = (t.nearest_vertex(Point(x,y)))->point();
        long followed = points[p];
        //First is variable, second is equation
        //lp.set_c(i,hourly_wage[i]);
        //lp.set_a(i,0,gang_u[followed]); // u equation
        //lp.set_a(i,1,gang_v[followed]); // v equation
        //lp.set_a(i,2,gang_w[followed]); // w equation

        if( follower.find(followed) == follower.end() ) {
            follower[followed] = i;
        } else {
            if( hourly_wage[follower[followed]] > wage ) {
                follower[followed] = i;
            }
        }
        /*
        if( follower[followed] == -1 ) {
            follower[followed] = i;
        }else {
            if( hourly_wage[follower[followed]] > wage ) {
                follower[followed] = i;
            }
        }
        */
    }
    for( auto it = follower.begin(); it != follower.end(); ++it ) {
        //long agent = it.second;
        lp.set_c(it->second,hourly_wage[it->second]);
        lp.set_a(it->second,0,gang_u[it->first]);
        lp.set_a(it->second,1,gang_v[it->first]);
        lp.set_a(it->second,2,gang_w[it->first]);
    }
    /*
    for( long i = 0; i < n_gang; i++ ) {
        if( follower[i] != -1 ) {
            long agent = follower[i];
            lp.set_c(agent,hourly_wage[agent]);
            lp.set_a(agent,0,gang_u[i]);
            lp.set_a(agent,1,gang_v[i]);
            lp.set_a(agent,2,gang_w[i]);
        }
    }
    */

    Solution s = CGAL::solve_linear_program(lp, ET());
    //std::cout<<top_limit<<std::endl;
    //std::cout<<s<<std::endl;

    if (s.is_optimal() ) {
        //std::cout<<(s.objective_value())<<"  "<<(CGAL::Quotient<ET>(top_limit))<<std::endl;
        if( s.objective_value() <= CGAL::Quotient<ET>(top_limit) ) {
            std::cout<<"L"<<std::endl;
        } else {
            std::cout<<"H"<<std::endl;
        }
    } else {
        std::cout<<"H"<<std::endl;
    }
    
    /*
    // by default, we have a nonnegative LP with Ax <= b

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
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        investigate();
    }

}
