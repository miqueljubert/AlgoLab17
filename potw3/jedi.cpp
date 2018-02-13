#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;


int circular_distance( int start, int end, int circle_length ) {
    return ( end - start + 1 + circle_length ) % circle_length;
}

void get_maximal_set()
{
    int n_intervals, n_segments;
    std::cin>>n_intervals>>n_segments;

    vector<pair<int,int>> intervals(n_intervals);
    for( int i = 0; i < n_intervals; i++ ) {
        int a,b;
        std::cin>>a>>b;
        intervals[i].first = a;
        intervals[i].second = b;
    }

    sort(intervals.begin(),intervals.end());
    vector<bool> chosen(n_intervals,false);//Not a good idea probably...
    vector<bool> used(n_intervals,true);//Not a good idea probably...

    vector<bool> tried(n_intervals,false);
    
    bool converged = false;

    //Always choose the first one to start with
    int i = 1;
    int index_previous = 0;
    chosen[0] = true;
    tried[0] = true;
    pair<int,int> previous_interval = intervals[0];
    int d_previous = circular_distance( previous_interval.first, previous_interval.second, n_segments );
    int number_set = 1;
    while( not converged ) {
        pair<int,int> interval = intervals[i];
        if( not tried[i] or chosen[i] ) {

        int d_start = circular_distance( previous_interval.first, interval.first, n_segments ); //If it begins out of range

        if( d_start > d_previous ) { //If it starts after the other ends no conflict
            //std::cout<<"no conflict:"<<d_start<<" < "<<d_previous<<std::endl;
            //std::cout<<"old-interval:"<<previous_interval.first<<" - "<<previous_interval.second<<std::endl;
            //std::cout<<"new-interval:"<<interval.first<<" - "<<interval.second<<std::endl;

            index_previous = i;
            chosen[index_previous] = true;
            tried[index_previous] = true;
            previous_interval = interval;
            d_previous = circular_distance( previous_interval.first, previous_interval.second, n_segments );
        } else {
            //Substitution rule, does it end earlier than the previous interval?
            
            int d_current = circular_distance( previous_interval.first, interval.second, n_segments ); //Distance from previous endpoint to next
            if( d_current < d_previous ) { //If this point ends earlier... we replace it
                //std::cout<<"index: "<<i<<std::endl;
                //std::cout<<"old-interval:"<<previous_interval.first<<" - "<<previous_interval.second<<std::endl;
                //std::cout<<"new-interval:"<<interval.first<<" - "<<interval.second<<std::endl;
                //std::cout<<"distances:"<<d_previous<<"->"<<d_current<<std::endl<<std::endl;
                chosen[index_previous] = false;

                index_previous = i;
                chosen[index_previous] = true;
                previous_interval = interval;
                d_previous = circular_distance( previous_interval.first, previous_interval.second, n_segments );

            } else { //We don't choose it
                chosen[i]  = false;
            }
        }
        //Otherwise just continue
        
        }
        i = (i+1)%n_intervals;

        int next_distance = circular_distance( previous_interval.first, intervals[i].first, n_segments );// Distance to the start of the next one


        if( chosen[i] and (next_distance > d_previous ) ) converged = true;

    }
    
    int result_intervals = 0;
    for( int i = 0; i < chosen.size(); i++ ) {
        if( chosen[i] ) {
            result_intervals++;
            //std::cout<<intervals[i].first<<" - "<<intervals[i].second<<std::endl;
        }
    }
    std::cout<<result_intervals<<std::endl;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        //std::cout<<"test: "<<i<<std::endl;
        get_maximal_set();
    }
}
