//#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <utility>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <stack>

//typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void get_non_intersectors()
{
    long n_points;
    std::cin>>n_points;
    
    std::vector<std::pair<long,long>> indices(n_points);
    std::vector<std::pair<long,K::FT>> lines(n_points);
    long y0,x1,y1;
    for( long i = 0; i < n_points; i++ ) {
        std::cin>>y0;
        std::cin>>x1>>y1;
        lines[i] = std::pair<long,K::FT>(y0, K::FT(y1-y0)/K::FT(x1) );
        indices[i] = std::pair<long,long>(y0,i);
    }

    std::sort(lines.begin(),lines.end(),
                []( const std::pair<long,K::FT> &p1, const std::pair<long,K::FT> &p2 )->bool {
                    return p1.first > p2.first;
                });
    std::sort(indices.begin(),indices.end(),
                []( const std::pair<long,K::FT> &p1, const std::pair<long,K::FT> &p2 )->bool {
                    return p1.first > p2.first;
                });
    


    //We start by choosing the last line
    std::stack<long> non_conflicts;
    non_conflicts.push(0);

    for( long i = 1; i < n_points; i++ ) { //non_conflicts cannot be empty or code is wrong ;)
        long last_line = non_conflicts.top();
        long last_y = lines[last_line].first;
        K::FT last_m = lines[last_line].second;

        long current_line = i;
        long current_y = lines[current_line].first;
        K::FT current_m = lines[current_line].second;
        //std::cout<<"last_line: "<<last_y<< " "<<last_m<<"  "<<indices[last_line].second<<std::endl;
        //std::cout<<"current_line: "<<current_y<< " "<<current_m<<"  "<<indices[current_line].second<<std::endl;

        bool conflicts = last_m < current_m; //If equal there is no conflict, parallel!
        if( conflicts ) {
            while( conflicts ) {
                if( last_m >= 0 ) {
                    //Both segments go upwards but the other one does it faster. The other one is discarded
                    //std::cout<<"conflict, discarding currnet"<<std::endl;
                    conflicts = false;
                } else if( current_m < 0 ) { //If both same sign other procedure. Current is larger, therefore less negative. the top of the pile needs to go
                    //std::cout<<"conflict, discarding last"<<std::endl;
                    //The segments go downwards. Our current top of-stack segment gets discarded
                    non_conflicts.pop();

                    if( non_conflicts.empty() ){//If we run out, our current segment stays valid
                        non_conflicts.push(current_line);
                        conflicts = false;
                        break;
                    }

                    last_line = non_conflicts.top();
                    last_y = lines[last_line].first;
                    last_m = lines[last_line].second;
                    //std::cout<<"new last_line: "<<last_y<< " "<<last_m<<std::endl;
                    conflicts = last_m < current_m; //If equal there is no conflict, parallel!
                        if( not conflicts ) {
                            non_conflicts.push(current_line);
                        }
                } else { //If different signs:
                    //std::cout<<"opposite signs"<<std::endl;
                    //std::cout<<last_m<<"  "<<current_m<<std::endl;
                    if( CGAL::abs(last_m) < CGAL::abs(current_m) ) {
                        //std::cout<<"conflict, discarding currnet"<<std::endl;
                        conflicts = false;
                    } else {
                        //std::cout<<"n.conflict, discarding last"<<std::endl;
                        //The segments go downwards. Our current top of-stack segment gets discarded
                        non_conflicts.pop();

                        if( non_conflicts.empty() ){
                            non_conflicts.push(current_line);
                            conflicts = false;
                            break;
                        }

                        last_line = non_conflicts.top();
                        last_y = lines[last_line].first;
                        last_m = lines[last_line].second;
                        //std::cout<<"new last_line: "<<last_y<< " "<<last_m<<std::endl;
                        conflicts = last_m < current_m;
                        if( not conflicts ) {
                            non_conflicts.push(current_line);
                        }
                    }
                }
            }
        } else {
            //std::cout<<"no conflict"<<std::endl;
            non_conflicts.push(current_line);
        }
        
        
    }
    //std::cerr<<"printing"<<std::endl;
    std::vector<long> result;
    int noninter = non_conflicts.size();
    //std::cout<<"noninter:"<<non_conflicts.size()<<std::endl;
    for( long i = 0; i < noninter; i++ ) {
        //std::cout<<indices[non_conflicts.top()].second<<" ";
        result.push_back(indices[non_conflicts.top()].second);
        non_conflicts.pop();
    }
    std::sort(result.begin(),result.end());
    for( long i = 0; i < noninter; i++ ) {
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;
    //std::cerr<<"done printing"<<std::endl;+

/*
  K::Point_2 p(2,1), q(1,0), r(-1,-1);
  K::Line_2 l(p,q);
  K::FT d = sqrt(CGAL::squared_distance(r,l));
  std::cout << floor_to_double(d) << std::endl;
*/
}

int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        get_non_intersectors();
    }
}
