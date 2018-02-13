// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

ET power_r( ET value, int exp )
{
    if( exp == 0 ) return ET(1);
    if( exp == 1 ) return value;
    if( (exp % 2) == 0 ) {
        ET temp = power_r( value, exp/2 );
        return temp*temp;
    }
    return value*power_r( value, exp-1 );
}



ET power( int value, int exp )
{
    if( exp == 0 ) return ET(1);
    if( exp == 1 ) return ET(value);
    return ET(value)*power( value, exp-1 );
}


bool is_viable( int d, int n_1, int n_2,
                std::vector<ET> &x_1,
                std::vector<ET> &y_1,
                std::vector<ET> &z_1,
                std::vector<ET> &x_2,
                std::vector<ET> &y_2,
                std::vector<ET> &z_2)
{
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);

    Program lp (CGAL::SMALLER, false, 0, false, 0);
    
    int variable = 0;
    for( int p_x = 0; p_x <= d; p_x++ ) {
        for( int p_y = 0; p_y <= d-p_x; p_y++ ) {
            for( int p_z = 0; p_z <= d-p_y-p_x; p_z++ ) {
                for( int i = 0; i < n_1; i++ ) {
                    ET value = power_r(x_1[i],p_x)*power_r(y_1[i],p_y)*power_r(z_1[i],p_z);
                    //double value = std::pow(x_1[i],p_x)*std::pow(y_1[i],p_y)*std::pow(z_1[i],p_z);
                    lp.set_a(variable,i,value);
                }
                for( int i = 0; i < n_2; i++ ) {
                    ET value = power_r(x_2[i],p_x)*power_r(y_2[i],p_y)*power_r(z_2[i],p_z);
                    //double value = std::pow(x_2[i],p_x)*std::pow(y_2[i],p_y)*std::pow(z_2[i],p_z);
                    lp.set_a(variable,i+n_1,-value);
                }
                variable++;
            }
        }
    }
    for( int i = 0; i < n_1; i++ ) {
        lp.set_a(variable,i,1); //Set the delta
    }
    for( int i = 0; i < n_2; i++ ) {
        lp.set_a(variable,i+n_1,1); //Set the delta
    }

    lp.set_c(variable,-1);
    lp.set_u(variable,true,1);



    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    return s.objective_value() < 0;

    return false;


}



void run()
{
    int n_healthy,n_tumor;
    std::cin>>n_healthy>>n_tumor;

    std::vector<ET> healthy_x(n_healthy);
    std::vector<ET> healthy_y(n_healthy);
    std::vector<ET> healthy_z(n_healthy);

    std::vector<ET> tumor_x(n_tumor);
    std::vector<ET> tumor_y(n_tumor);
    std::vector<ET> tumor_z(n_tumor);

    for( int i = 0; i < n_healthy; i++ ) {
        std::cin>>healthy_x[i];
        std::cin>>healthy_y[i];
        std::cin>>healthy_z[i];
    }
    for( int i = 0; i < n_tumor; i++ ) {
        std::cin>>tumor_x[i];
        std::cin>>tumor_y[i];
        std::cin>>tumor_z[i];
    }
    
    int start = 0;
    int end = 1;
    int valid = -1;
    bool exp = true;
    while( start <= end ) {
        int current;
        if( exp ) {
            current = end;
        } else {
            current = (start+end)/2;
        }
        //std::cout<<"c:"<<current<<" s:"<<start<<" e:"<<end<<std::endl;

        bool result = is_viable( current, n_healthy, n_tumor, healthy_x, healthy_y, healthy_z, tumor_x, tumor_y, tumor_z );
        

        if( exp ) {
            if( result ) { //Stop exponential search
                exp = false;
                valid = current;
                end = end-1;
            } else {
                start = end+1;
                end = std::min(30,2*end);
            }
        } else {
            if( result ) {
                end = current-1;
                valid = current;
            } else {
                start = current+1;
            }
        }
    }
    if( valid >= 0 ) std::cout<<valid<<std::endl;
    else std::cout<<"Impossible!"<<std::endl;

}




/*
int main() {
  // by default, we have an LP with Ax <= b and no bounds for
  // the four variables alpha, beta, gamma, delta
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  const int alpha = 0;
  const int beta  = 1;
  const int gamma = 2;
  const int delta = 3;

  // number of red and blue points
  int m; std::cin >> m;
  int n; std::cin >> n;

  // read the red points (cancer cells)
  for (int i=0; i<m; ++i) {
    int x; std::cin >> x;
    int y; std::cin >> y;
    // set up <= constraint for point inside/on circle:
    //   -alpha x - beta y - gamma <= -x^2 - y^2
    lp.set_a (alpha, i, -x);
    lp.set_a (beta,  i, -y);
    lp.set_a (gamma, i, -1);
    lp.set_b (       i, -x*x - y*y);
  }
  // read the blue points (healthy cells)
  for (int j=0; j<n; ++j) {
    int x; std::cin >> x;
    int y; std::cin >> y;
    // set up <= constraint for point outside circle:
    //   alpha x + beta y + gamma + delta <= x^2 + y^2
    lp.set_a (alpha, m+j, x);
    lp.set_a (beta,  m+j, y);
    lp.set_a (gamma, m+j, 1);
    lp.set_a (delta, m+j, 1);
    lp.set_b (       m+j, x*x + y*y);
  }

  // objective function: -delta (the solver minimizes)
  lp.set_c(delta, -1);

  // enforce a bounded problem:
  lp.set_u (delta, true, 1);

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert (s.solves_linear_program(lp));

  // output exposure center and radius, if they exist
  if (s.is_optimal() && (s.objective_value() < 0)) {
    // *opt := alpha, *(opt+1) := beta, *(opt+2) := gamma
    CGAL::Quadratic_program_solution<ET>::Variable_value_iterator
      opt = s.variable_values_begin();
    CGAL::Quotient<ET> alpha = *opt;
    CGAL::Quotient<ET> beta = *(opt+1);
    CGAL::Quotient<ET> gamma = *(opt+2);
    std::cout << "There is a valid exposure:\n";
    std::cout << " Center = ("        // (alpha/2, beta/2)
	      << alpha/2 << ", " <<  beta/2
	      << ")\n";
    std::cout << " Squared Radius = " // gamma + alpha^2/4 + beta^2/4
	      << gamma + alpha*alpha/4 + beta*beta/4 << "\n";
  } else
    std::cout << "There is no valid exposure.";
  std::cout << "\n";
  return 0;
}
*/
int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
