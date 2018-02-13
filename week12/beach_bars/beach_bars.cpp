#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>

typedef std::pair<int,int> Pair;

int length( int x, int y )
{
    return abs(x-y);
}


void do_test()
{
    int n;
    std::cin>>n;
    std::vector<int> points(n);
    for( int i = 0; i < n; i++ ) std::cin>>points[i];
    std::sort(points.begin(),points.end());
    //for( int i = 0; i < n; i++ ) std::cout<<points[i]<<","; std::cout<<std::endl;

    std::vector<Pair> opt_interval;
    int opt_val = 1;        //Maximize this
    int opt_length = 1;     //Minimize this, given that the other one is maximum
    int i = 0;              //Start pointer
    int j = 0;              //End pointer
    int val = 1;            //Number of values between the two pointers
    while( j < n ) {

        //If it's too large, make it smaller by moving the head forward
        if( length(points[j],points[i]) > 200 ) {
            i++;
        } else { //This interval is valid, is it large enough?
            int n_in = length(j,i)+1;
            int len = length(points[j],points[i])+1;
            if( n_in > opt_val ) { //If this is better than our previous best
                opt_interval = std::vector<Pair>(1,Pair(i,j));
                opt_val = n_in;
                opt_length = len; //Optimal length gets overwritten!
            } else if( n_in == opt_val ) { //Add it to the set
                if( (len/2) < (opt_length/2) ) { //If the length is smaller...
                    opt_interval = std::vector<Pair>(1,Pair(i,j));
                    opt_length = len;                               //New optimal length
                } else if( (len/2) == (opt_length/2) ) {
                    opt_interval.push_back(Pair(i,j));
                }
            }
            //If it's not an optimal interval then do nothing
            j++;
        }
    }
    //std::cout<<"result:"<<std::endl;
    //std::cout<<"opt length:"<<opt_length<<std::endl;
    //std::cout<<"opt intervals:"<<opt_interval.size()<<std::endl;
    //for( int i = 0; i <opt_interval.size(); i++ ) std::cout<<"("<<points[opt_interval[i].first]<<","<<points[opt_interval[i].second]<<"),"; std::cout<<std::endl;

    std::cout<<opt_val<<" "<<(opt_length/2)<<std::endl;
    for( int i = 0; i < opt_interval.size(); i++ ) {
        //std::cout<<std::endl;
        int pos2 = points[opt_interval[i].second]+points[opt_interval[i].first];
        if( pos2%2 ) {
            if(pos2 < 0) std::cout<<(pos2-1)/2<<" "<<pos2/2<<" ";
            else std::cout<<pos2/2<<" "<<(pos2+1)/2<<" ";
             
        } else {
            std::cout<<pos2/2<<" ";
        }
    }

    std::cout<<std::endl;

}


int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) do_test();
}
