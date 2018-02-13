#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double( const SolT& x )
{
    double a = std::ceil(CGAL::to_double(x));
    while( a < x ) a+=1;
    while( a-1 >= x ) a-= 1;
    return a;
}



void compute_circle( int n, int d )
{
    std::vector<std::vector<long>> inequalities(n,std::vector<long>(d+1));
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < d+1; j++ ) {
            std::cin>>inequalities[i][j];
        }
    }
    std::vector<long> norm(n,0);
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < d; j++ ) {
            norm[i] += inequalities[i][j]*inequalities[i][j];
        }
        norm[i] = std::sqrt(norm[i]);
    }

    Program p(CGAL::SMALLER,false,0,false,0);

    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < d; j++ ) {
            //p.set_a( j, i, ET(ET(inequalities[i][j])/ET(norm[i])) );//One equation per constraint
            p.set_a( j, i, ET(inequalities[i][j]) );
        }
        //p.set_a(d,i,1);
        //p.set_b(i, ET(ET(inequalities[i][d])/ET(norm[i])) );
        p.set_a(d,i,norm[i]);
        p.set_b(i, ET(inequalities[i][d]) );
    }
    p.set_c(d,ET(-1));
    p.set_l(d,true,0);

    Solution s = CGAL::solve_linear_program(p,ET());
    //std::cout<<s<<std::endl;
    if( s.is_unbounded() ) {
        std::cout<<"inf"<<std::endl;
    } else if( s.is_optimal() ) {
        std::cout<<-ceil_to_double(s.objective_value())<<std::endl;
    } else {
        std::cout<<"none"<<std::endl;
    }
    //std::cout<<s<<std::endl;

    
}

int main() {
    int n,d;
    std::cin>>n>>d;
    while( n != 0 and d != 0 ) {
        compute_circle(n,d);
        std::cin>>n>>d;
    }
}
