#include <iostream>
#include <vector>
using namespace std;


int mod( int i, int k ) {
    return (i%k)<0? (i%k)+k : (i%k);
}




bool dp( int d, int n, int i, int k, vector<int> &heights, vector<vector<int>> &memo ) 
{
    if( d == heights.size() ) return false;

    if( memo[d][i] != -1 ) return memo[d][i] > 0;

    int ni = mod( i - heights[d], k );

    //cout<<ni<<" "<<i<<" "<<k<<" "<<heights[d]<<endl;

    if( ni == 0 ) {
        memo[d][i] = 1;
        return true;
    }
    bool b = dp(d+1,n,ni,k,heights,memo) or dp(d+1,n,i,k,heights,memo);
    memo[d][i] = b? 1 : 0;
    return memo[d][i] > 0;

}


void run()
{
    int n,i,k;
    cin>>n>>i>>k;
    vector<int> heights(n);
    for( int j = 0; j < n; j++ ) cin>>heights[j];

    vector<vector<int>> memo(n,vector<int>(k,-1));
    if( dp( 0, n, i, k, heights, memo ) ) cout<<"yes"<<endl;
    else cout<<"no"<<endl;

}


int main()
{
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
