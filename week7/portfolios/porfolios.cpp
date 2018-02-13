
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

// round up to next integer double
double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}




void compute_portfolio( int nassets, int nportfolios )
{
    std::vector<int> asset_cost(nassets);
    std::vector<int> asset_return(nassets);
    for( int i = 0; i < nassets; i++ ) {
        std::cin>>asset_cost[i];
        std::cin>>asset_return[i];
    }

    std::vector<std::vector<int>> covariance(nassets,std::vector<int>(nassets));
    for( int i = 0; i < nassets; i++ ) {
        for( int j = 0; j < nassets; j++ ) {
            std::cin>>covariance[i][j];
        }
    }
    std::vector<int> max_portfolio_cost(nportfolios);
    std::vector<int> min_portfolio_return(nportfolios);
    std::vector<int> max_portfolio_variance(nportfolios);
    for( int i = 0; i < nportfolios; i++ ) {
        std::cin>>max_portfolio_cost[i];
        std::cin>>min_portfolio_return[i];
        std::cin>>max_portfolio_variance[i];
    }

    //Lower bound for buying assets
    Program qp( CGAL::LARGER, true, 0, false, 0 );

    //Set up the covariances
    for( int i = 0; i < nassets; i++ ) {
        for( int j = 0; j <= i ; j++ ) {
            qp.set_d(i,j,2*ET(covariance[i][j]));
        }
    }

    //Set up the cost equations
    for( int i = 0; i < nassets; i++ ) {
        //Cost equation
        qp.set_a(i,0,-asset_cost[i]);
        //Return equation
        qp.set_a(i,1,asset_return[i]);
    }

    for( int i = 0; i < nportfolios; i++ ) { //For each portfolio
        qp.set_b(0, -max_portfolio_cost[i]);
        qp.set_b(1, min_portfolio_return[i]);

        Solution s = CGAL::solve_quadratic_program(qp, ET());
        //std::cout<<s<<std::endl;
        if( s.status() == CGAL::QP_OPTIMAL ) {

            if(ET(max_portfolio_variance[i])>=s.objective_value()) {
                std::cout<<"Yes."<<std::endl;
            } else {
                std::cout<<"No."<<std::endl;
            }
        } else {
            std::cout<<"No."<<std::endl;
        }
        //std::cout<<max_portfolio_variance[i]<<" =? "<< s.objective_value()<<std::endl;
    }
    
}






int main()
{
    int n,m;
    std::cin>>n>>m;
    while( n != 0 and m != 0 ) {
        compute_portfolio(n,m);
        std::cin>>n>>m;
    }
}
