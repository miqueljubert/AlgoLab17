#include <vector>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;

const int MAX_INT = numeric_limits<int>::max();
typedef pair<int,int> Pair;

pair<int,int> min_max( const Pair &p1, const Pair &p2 )
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
    }
}




Pair optimal_cost( int i, int n, int c, int k,
                   const vector<int> &cst,
                   const vector<int> &vol,
                   vector<vector<Pair>> &memo )
{
    if( c == 0 ) return Pair(0,0); //Check if works without
    if( i == n-1 ) { //Last bottle!
        int opt_count = c/vol[i];
        if( opt_count*vol[i] != c ) opt_count++;
        return Pair(opt_count*cst[i],1);
    }

    if( memo[i][c].first != -1 ) return memo[i][c];
    
    Pair o1,o2,o3;
    //o1->dont take
    //o2->take more times
    //o3->take last time

    //don't take this bottle
    o1 = optimal_cost(i+1,n,c,k,cst,vol,memo);
    if( vol[i] >= c ) {
        o2 = Pair(cst[i],1);//Can only take this time...
        o3 = Pair(cst[i],1);
    } else {
        o2 = optimal_cost(i,n,c-vol[i],k,cst,vol,memo);
        o2.first += cst[i];
        o3 = optimal_cost(i+1,n,c-vol[i],k,cst,vol,memo);
        o3.first += cst[i];
        o3.second += 1;
    }

    Pair res = min_max(o1,min_max(o2,o3));
    memo[i][c] = res;
    return res;
}

void run()
{
    int n,k;
    cin>>n>>k;
    

    vector<int> cst(n);
    vector<int> vol(n);
    for( int i = 0; i < n; i++ ) {
        cin>>cst[i]>>vol[i];
    }
    vector<vector<Pair>> memo(n,vector<Pair>(k+1,Pair(-1,-1)));
    Pair res = optimal_cost(0,n,k,k,cst,vol,memo);
    cout<<res.first<<" "<<res.second<<endl;

    
}


int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
