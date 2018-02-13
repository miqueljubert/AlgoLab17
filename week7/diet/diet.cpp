#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;


// round up to next integer double
double ceil_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}



void compute_diet( int nnutrients, int nproducts )
{
    //std::cout<<"n:"<<nnutrients<<" m:"<<nproducts<<std::endl;

    std::vector<int> nutrient_min(nnutrients);
    std::vector<int> nutrient_max(nnutrients);
    for( int i = 0; i < nnutrients; i++ ) {
        std::cin>>nutrient_min[i];
        std::cin>>nutrient_max[i];
    }

    std::vector<int> product_price(nproducts);
    std::vector<std::vector<int>> product_nutrients(nproducts,std::vector<int>(nnutrients));
    for( int i = 0; i < nproducts; i++ ) {
        std::cin>>product_price[i];
        for( int j = 0; j < nnutrients; j++ ) {
            std::cin>>product_nutrients[i][j];
        }
    }

    //Linear program set-up
    Program lp (CGAL::LARGER, true, 0, false, 0);
    //Set up the matrix
    //Cost function
    for( int i = 0; i < nproducts; i++ ) {
        lp.set_c(i,product_price[i]);//Minimize price of amoutns
    }
    //Equations:
    for( int i = 0; i < nnutrients; i++ ) { //For each nutrient (aka equation)
        //std::cout<<nutrient_min[i]<<" <= ";
        for( int j = 0; j < nproducts; j++ ) { //For each product
            //std::cout<<product_nutrients[j][i]<<"*x"<<j<<" + ";
            lp.set_a(j,i,product_nutrients[j][i]);
            lp.set_a(j,i+nnutrients,-product_nutrients[j][i]);
        }
        //std::cout<<" <= "<<nutrient_max[i]<<std::endl;
        lp.set_b(i,nutrient_min[i]);
        lp.set_b(i+nnutrients,-nutrient_max[i]);
    }



    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.status() == CGAL::QP_INFEASIBLE) {
        std::cout << "No such diet."<<std::endl;
    } else {
        //std::cout<<s<<std::endl;
        std::cout<<(int)-ceil_to_double(-s.objective_value())<<std::endl;
    }

    // output solution
    //std::cout << s;
}




int main()
{
    int n,m;
    std::cin>>n>>m;
    while( n != 0 and m != 0 ) {
        compute_diet(n,m);
        std::cin>>n>>m;
    }
}
