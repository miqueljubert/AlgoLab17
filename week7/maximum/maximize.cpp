#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double( const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while(a < x) a += 1;
    while( a-1 >= x ) a -= 1;
    return a;
}

void do_stuff( int program, int a, int b ){
    //std::cout<<"pr:"<<program<<" a:"<<a<<" b:"<<b<<std::endl;
    if (program == 1) {
        Program p1 (CGAL::SMALLER, false, 0, false, 0);
        //Program p1 (CGAL::LARGER, true, 0, false, 0);
        
        const int X = 0;
        const int Y = 1;

        //Setup program 1
        p1.set_d(X,X,2*a);

        p1.set_c(Y,-b);

        p1.set_a(X,0,1);
        p1.set_a(X,1,4);
        p1.set_a(X,2,-1);

        p1.set_a(Y,0,1);
        p1.set_a(Y,1,2);
        p1.set_a(Y,2,1);

        p1.set_b(0,4);
        p1.set_b(1,a*b);
        p1.set_b(2,1);
        

        p1.set_l(X,true,0);
        p1.set_l(Y,true,0);

        Solution s = CGAL::solve_quadratic_program(p1,ET());
        //std::cout<<"correct:"<<(s.solves_quadratic_program(p1))<<std::endl;
        //std::cout<<"1:"<<s<<std::endl;
        //return;
        if( s.status() == CGAL::QP_INFEASIBLE ) {
            std::cout<<"no"<<std::endl;
        } else if( s.status() == CGAL::QP_UNBOUNDED ) {
            std::cout<<"unbounded"<<std::endl;
            
        }else {
            //CGAL::Quotient<ET> d =
                //-(s.objective_value().numerator() / s.objective_value().denominator());
	
            std::cout<<(int)-ceil_to_double(s.objective_value())<<std::endl;
        }
    } else {
        Program p2 (CGAL::LARGER, false, 0, false, 0);
        //Program p2 (CGAL::SMALLER, true, 0, false, 0);
        //D
        const int X = 0;
        const int Y = 1;
        const int Z = 2;
        p2.set_d(X,X,2*a);
        p2.set_d(Z,Z,2);
        
        //C
        p2.set_c(Y,b);

        //A
        p2.set_a(X,0,1);
        p2.set_a(X,1,4);
        p2.set_a(X,2,-1);

        p2.set_a(Y,0,1);
        p2.set_a(Y,1,2);
        p2.set_a(Y,2,1);

        p2.set_a(Z,1,1);


        //b
        p2.set_b(0,-4);
        p2.set_b(1,-a*b);
        p2.set_b(2,-1);

        //ub
        p2.set_u(0,true,0);
        p2.set_u(1,true,0);
        p2.set_l(2,true,0);
        Solution s = CGAL::solve_quadratic_program(p2,ET());
        //std::cout<<"correct:"<<(s.solves_quadratic_program(p2))<<std::endl;
        //std::cout<<"2:"<<s<<std::endl;
        //return;
        if( s.status() == CGAL::QP_INFEASIBLE ) {
            std::cout<<"no"<<std::endl;
        } else if( s.status() == CGAL::QP_UNBOUNDED ) {
            std::cout<<"unbounded"<<std::endl;
            
        }else {
            CGAL::Quotient<ET> d =
                (s.objective_value().numerator() / s.objective_value().denominator());
            std::cout<<(int)ceil_to_double(s.objective_value())<<std::endl;

        }
    }

}
int main() {
    int p,a,b;
    std::cin>>p;
    while( p != 0 ) {
        std::cin>>a>>b;
        do_stuff(p,a,b);
        std::cin>>p;
    }
}
