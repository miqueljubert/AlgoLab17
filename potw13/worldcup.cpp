#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <cmath>

#include <algorithm>
#include <iostream>
#include <set>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;


using namespace std;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

//int main()
//{
//  K::Point_2 p(2,1), q(1,0), r(-1,-1);
//  K::Line_2 l(p,q);
//  K::FT d = sqrt(CGAL::squared_distance(r,l));
//  std::cout << floor_to_double(d) << std::endl;
//}
//
//int main() {
// solve the program, using ET as the exact type
//  Solution s = CGAL::solve_linear_program(lp, ET());
//  assert (s.solves_linear_program(lp));
//
//  // output exposure center and radius, if they exist
//  if (s.is_optimal() && (s.objective_value() < 0)) {
//    // *opt := alpha, *(opt+1) := beta, *(opt+2) := gamma
//    CGAL::Quadratic_program_solution<ET>::Variable_value_iterator
//      opt = s.variable_values_begin();
//    CGAL::Quotient<ET> alpha = *opt;
//    CGAL::Quotient<ET> beta = *(opt+1);
//    CGAL::Quotient<ET> gamma = *(opt+2);
//    std::cout << "There is a valid exposure:\n";
//    std::cout << " Center = ("        // (alpha/2, beta/2)
//	      << alpha/2 << ", " <<  beta/2
//	      << ")\n";
//    std::cout << " Squared Radius = " // gamma + alpha^2/4 + beta^2/4
//	      << gamma + alpha*alpha/4 + beta*beta/4 << "\n";
//  } else
//    std::cout << "There is no valid exposure.";
//  std::cout << "\n";
//  return 0;
//}
void run()
{
    int n,m,c;
    cin>>n>>m>>c;

    vector<Point> warehouses(n);
    vector<int> supply(n);
    vector<int> content(n);
    for( int i = 0; i < n; i++ ) {
        int x,y,s,a;
        cin>>x>>y>>s>>a;
        warehouses[i] = Point(x,y);
        supply[i] = s;
        content[i] = a;
    }
    vector<Point> stadiums(m);
    vector<int> demand(m);
    vector<int> limit(m);
    for( int i = 0; i < m; i++ ) {
        int x,y,d,l;
        cin>>x>>y>>d>>l;
        stadiums[i] = Point(x,y);
        demand[i] = d;
        limit[i] = l;
    }

    vector<vector<int>> revenue(n,vector<int>(m));
    for( int i = 0; i < n; i++ ) { //Per warehouse
        for( int j = 0; j < m; j++ ) {//Per stadiom
            int r;
            cin>>r;
            revenue[i][j] = r;
        }
    }

    vector<Point> contours(c);
    vector<K::FT> c_radius(c);
    for( int i = 0; i < c; i++ ) {
        int x,y;
        long r;
        cin>>x>>y>>r;
        contours[i] = Point(x,y);
        c_radius[i] = r*r;
    }

    vector<set<int>> inside_w(n);
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < c; j++ ) {
            K::FT d = CGAL::squared_distance(warehouses[i],contours[j]);
            if( d < c_radius[j] ) {//Inside
                inside_w[i].insert(j);
            }
        }
    }
    vector<set<int>> inside_s(m);
    for( int i = 0; i < m; i++ ) {
        for( int j = 0; j < c; j++ ) {
            K::FT d = CGAL::squared_distance(stadiums[i],contours[j]);
            if( d < c_radius[j] ) {//Inside
                inside_s[i].insert(j);
            }
        }
    }

    //Number of crossings
    vector<vector<int>> crossing(n,vector<int>(m));
    for( int i = 0; i < n; i++ ) {//Warehosue
        for( int j = 0; j < m; j++ ) {//Stadium
            set<int> un_diff;
            for( int el : inside_w[i] ) {
                if( inside_s[j].find(el) == inside_s[j].end() ) {
                    un_diff.insert(el);
                }
            }
            for( int el : inside_s[j] ) {
                if( inside_w[i].find(el) == inside_w[i].end() ) {
                    un_diff.insert(el);
                }
            }
            crossing[i][j] = un_diff.size();
        }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);
    
    int equation = 0;
    //Constraints per stadium, amount needed
    for( int i = 0; i < m; i++ ) { //Per stadium (equation)
        for( int j = 0; j < n; j++ ) { //Per brewery (variable)
            int var = i*n + j;
            lp.set_a(var,equation,1);//Amount recieved
        }
        lp.set_r(equation,CGAL::EQUAL);
        lp.set_b(equation,demand[i]);
        equation++;
    }
    //Constraints per stadium, alcohol limit
    for( int i = 0; i < m; i++ ) { //Per stadium (equation)
        for( int j = 0; j < n; j++ ) { //Per brewery (variable)
            int var = i*n + j;
            lp.set_a(var,equation,content[j]);//Amount recieved
        }
        lp.set_b(equation,100*limit[i]);
        equation++;
    }
    
    //Constraint per warehouse, limit to send
    for( int j = 0; j < n; j++ ) { //Per brewery (equation)
        for( int i = 0; i < m; i++ ) { //Per stadium (variable)
            int var = i*n + j;
            lp.set_a(var,equation,1); //Amoutn taken from each brewery
        }
        lp.set_b(equation,supply[j]);
        equation++;
    }
    //Cost function
    for( int i = 0; i < m; i++ ) { //Per stadium (equation)
        for( int j = 0; j < n; j++ ) { //Per brewery (variable)
            int var = i*n + j;
            lp.set_c(var,-(100*revenue[j][i]-crossing[j][i]));
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    //cout<<s<<endl;
    if( s.is_infeasible() ) {
        cout<<"RIOT!"<<endl;
    } else {
        ET num = -s.objective_value().numerator();
        ET den = s.objective_value().denominator()*100;
        ET value = num/den;
        if( value < 0 and (value*den != num) ){
            cout<<value-1<<endl;
        }else {
            cout<<value<<endl;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
