#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INT_MAX = numeric_limits<int>::max();


void print_bool( int n, int n_bits )
{
    for( int i = 0; i < n_bits; i++ ) {
        cout<<(0x01 & (n>>i));
    }
    cout<<endl;
}


inline int mask( int num, int nbits ) {
    return num & ((0x01<<(nbits)) - 1);
}

int get_edit_distance( int current, int pattern, int k, vector<int> &memo )
{
    //print_bool(current,k);
    //print_bool(pattern,k);
    //cout<<endl;

    if( current == mask(pattern,k) ) return 0; 
    if( memo[current] != -1 ) return memo[current];

    int actual = INT_MAX;
    for( int i = 0; i < k; i++ ) {
        if( (current xor pattern) & (0x01<<i) ) {
            int n_pattern = current xor (0x01<<i);
            actual = min(actual, get_edit_distance(n_pattern, pattern, k, memo)+1);
        }
    }
    
    //cout<<"distance:"<<actual<<endl;
    //print_bool(current,k);
    //print_bool(pattern,k);
    //cout<<endl;

    memo[current] = actual;
    return actual;
    
}



void run()
{
    int n,k;
    cin>>n>>k;

    int pattern;
    cin>>pattern;

    int n_sets = n/k;//whole number for sure
    vector<int> vi(n_sets,0x00);
    
    for( int i = 0; i < n_sets; i++ ) {
        for( int j = 0; j < k; j++ ) {
            bool bit;
            cin>>bit;
            if( bit ) vi[i] = vi[i] | (0x01<<(k-j-1));
        }
    }

    vector<int> memo(0x01<<k,-1);
    vector<int> memof(0x01<<k,-1);
    //int current = get_edit_distance(0,pattern,0,k,memo);
    //current = get_edit_distance(1,pattern,0,k,memo);

    int total_dist = 0;
    bool flipped = false;
    bool sure = true;
    for( int i = n_sets-1; i >= 0; i-- ) {
        int d  = get_edit_distance(vi[i],  pattern, k, memo);
        int df = get_edit_distance(mask(~vi[i],k), pattern, k, memof);


        //If we know that it was flipped/not flipped
        if( sure ) {
            if( flipped ) {d += 1; }
            else          {df += 1;}
            //cout<<vi[i]<<" "<<pattern<<" "<<d<<"-"<<df<<" flipped:"<<flipped<<" sure:"<<sure<<endl;
            
            if( d == df ) { //If our choice does not matter
                sure = false;
                total_dist += d;
            } else if( d > df ) {
                flipped = true;
                total_dist += df;
            } else {
                flipped = false;
                total_dist += d;
            }
        } else { //If not sure, we just choose the smaller of the two
            //cout<<vi[i]<<" "<<pattern<<" "<<d<<"-"<<df<<" flipped:"<<flipped<<" sure:"<<sure<<endl;

            if( d == df ) {
                total_dist += d;
            } else if( d > df ) {
                sure = true;
                flipped = true;
                total_dist += df;
            } else {
                sure = true;
                flipped = false;
                total_dist += d;
            }

        }


    }
    cout<<total_dist<<endl;
    
    //vector<int> memo1(0x01<<6,-1);
    //vector<int> memof1(0x01<<6,-1);
    //int d  = get_edit_distance( 3, 33, 6, memo1 );
    //int df = get_edit_distance( mask(~3,6), 33, 6, memof1 );
    //print_bool(3,6);
    //print_bool(mask(~3,6),6);
    //print_bool(33,6);
    //cout<<d<<" "<<df<<endl;

}


int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
