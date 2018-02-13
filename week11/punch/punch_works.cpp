#include <iostream>
#include <vector>
#include <utility>
#include <limits>
using namespace std;


const int MAX_INT = numeric_limits<int>::max();

pair<int,int> minmax_pair( const pair<int,int> &p1, const pair<int,int> &p2 )
{
    if( p1.first < p2.first ) {
        return p1;
    } else if( p1.first > p2.first ) {
        return p2;
    } else {
        if( p1.second > p2.second ) {
            return p1;
        } else {
            return p2;
        }
        //p1.second > p2.second? p1 : p2;
    }
}


pair<int,int> beverages( int i, int n,
                         int c, int k,
                         const vector<int> &vol,
                         const vector<int> &cst,
                         vector<vector<pair<int,int>>> &memo )
{
    if( memo[i][c].first != MAX_INT ) return memo[i][c];

    if( c == 0 ) return pair<int,int>(0,0);
    if( i == n-1 ) {
        //Fill the remaining
        pair<int,int> opt;
        if( c % vol[i] != 0 ) opt = pair<int,int>((c/vol[i]+1)*cst[i],1);
        else opt = pair<int,int>((c/vol[i])*cst[i],1);
        memo[i][c] = opt;
        return memo[i][c];
    }

    pair<int,int> opt1 = pair<int,int>(MAX_INT,-1);
    pair<int,int> opt2 = pair<int,int>(MAX_INT,-1);
    pair<int,int> opt3 = pair<int,int>(MAX_INT,-1);
    if( vol[i] <= c ) {
        opt1 = beverages(i+1,n,c-vol[i],k,vol,cst,memo);
        if( opt1.first != MAX_INT ) {
            opt1.first  += cst[i];
            opt1.second += 1;
        }

        opt2 = beverages(i,n,c-vol[i],k,vol,cst,memo);
        if( opt2.first != MAX_INT ) {
            opt2.first  += cst[i];
        }
    } else {

        opt3 = beverages(i+1,n,c,k,vol,cst,memo);
        memo[i][c] = minmax_pair(opt3,pair<int,int>(cst[i],1));
        return memo[i][c];
    }

    opt3 = beverages(i+1,n,c,k,vol,cst,memo);
    
    memo[i][c] = minmax_pair(opt1,minmax_pair(opt2,opt3));
    return memo[i][c];

}


void run()
{
    int n,k;
    cin>>n>>k;

    vector<int> vol(n);
    vector<int> cst(n);
    int max_volume = -1;
    for( int i = 0; i < n; i++ ) {
        cin>>cst[i]>>vol[i];
        max_volume = max(vol[i],max_volume);
    }
    
    vector<vector<pair<int,int>>>
                    max_bev( n, vector<pair<int,int>>(k+1,pair<int,int>(MAX_INT,-1)));


    pair<int,int> o = pair<int,int>(MAX_INT,-1);
    o = minmax_pair(o,beverages(0,n,k,k,vol,cst,max_bev));
    cout<<o.first<<" "<<o.second<<endl;
    

}


int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
