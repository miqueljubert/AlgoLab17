#include <vector>
#include <iostream>
using namespace std;


bool can_do_permutation( int current_node, int n, vector<int> &permutation, vector<int> &bomb_timers )
{
    if( current_node >= (n-1)/2 ) {
        permutation = vector<int>(1,bomb_timers[current_node]);
        return true;
    }

    bool b1,b2;
    std::vector<int> p1, p2;
    b1 = can_do_permutation( 2*current_node + 1, n, p1, bomb_timers );
    b2 = can_do_permutation( 2*current_node + 2, n, p2, bomb_timers );
    
    if( b1 and b2 ) {
        //Merge the two lists
        int i,i1,i2;

        permutation = vector<int>(p1.size()+p2.size()+1);
        i = permutation.size()-2;
        i1 = p1.size()-1;
        i2 = p2.size()-1;
            /*
        std::cout<<std::endl<<"p1:";
        for( int i = 0; i < p1.size(); i++ ) std::cout<<p1[i]<<" ";
        std::cout<<std::endl<<"p2:";
        for( int i = 0; i < p2.size(); i++ ) std::cout<<p2[i]<<" ";
        std::cout<<std::endl<<"current:";
        std::cout<<bomb_timers[current_node]<<std::endl;
        */

        while( i1 >= 0 and i2 >= 0 ) {
            if( p1[i1] > p2[i2] ) { //Start by the largest one
                if( i < p1[i1] ) {
                    permutation[i] = p1[i1];
                    i1--;
                } else {
                    //std::cout<<"break1: "<< i <<" < "<<p1[i1]<<std::endl;
                    return false;
                }
            } else {//add the p2p2
                if( i < p2[i2] ) {
                    permutation[i] = p2[i2];
                    i2--;
                } else {
                    //std::cout<<"break2: "<< i <<" < "<<p2[i2]<<std::endl;
                    return false;
                }
            }
            i--;
        }

        while( i1 >= 0 ) {
            if( i < p1[i1] ) {
                permutation[i] = p1[i1];
            } else {
                    //std::cout<<"break1: "<< i <<" < "<<p1[i1]<<std::endl;
                return false;
            }
            i1--;
            i--;
       }
       while( i2 >= 0 ) {
            if( i < p2[i2] ) {
                permutation[i] = p2[i2];
            } else {
                    //std::cout<<"break2: "<< i <<" < "<<p2[i2]<<std::endl;
                return false;
            }
            i2--;
            i--;
       }
       if( i < bomb_timers[current_node] ) {
            permutation[permutation.size()-1] = bomb_timers[current_node];
            return true;
       } else {
                    //std::cout<<"bombt: "<< i <<" < "<<bomb_timers[current_node]<<std::endl;
            return false;
       }

    } else {
        return false;
    }
}


void deactivate_bombs()
{
    int n_bombs;
    std::cin>>n_bombs;
    vector<vector<int>> fills(n_bombs,vector<int>(0));


    std::vector<int> bomb_timers(n_bombs);
    for( int i = 0; i < n_bombs; i++ ) {
        std::cin>>bomb_timers[i];
    }
    std::vector<int> permu;
    if (can_do_permutation( 0, n_bombs, permu, bomb_timers ) ) {
        std::cout<<"yes"<<std::endl;
    } else {
        std::cout<<"no"<<std::endl;
    }

}



int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        deactivate_bombs();
    }
}
