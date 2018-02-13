#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
using namespace std;


typedef pair<int,int> Pair;

//cost,amount
//pair<int,int> get_punch( int i, int n, int k, int max_cost, int current_amount, const std::vector<int> &volume, const std::vector<int> &price, vector<vector<pair<int,int>>> &memo )
//{
//}




//cost,amount
pair<int,int> max_amount( int i, int n, int k, int max_cost, const std::vector<int> &volume, const std::vector<int> &price, vector<vector<pair<int,int>>> &memo )
{
    int m_amount = (int) std::ceil(double(k)/double(volume[i]));

    if( k <= 0 ) return pair<int,int>(0,0);
    if( memo[i][k].first != -1 ) {
        //std::cout<<"i:"<<i<<" n:"<<n<<" k:"<<k<<std::endl;
        //std::cout<<"m"<<std::endl;
        return memo[i][k];
    }

    if( i == n-1 ) {
        int cost = m_amount*price[i];
        return pair<int,int>(cost,1);
    }

    //If I don't add it
    //pair<int,int> base = max_amount(i+1,n,k,max_cost,volume,price,memo);
    //int min_cost = base.first;
    //int max_drinks = base.second;
    //int min_idx = 0;
    int min_cost = numeric_limits<int>::max();
    int max_drinks = -1;

    for( int j = 0; j <= m_amount; j++ ) {

        pair<int,int> r = max_amount(i+1,n,k-j*volume[i],max_cost,volume,price,memo);
        int cost = r.first + j*price[i];
        int n_drinks = r.second + 1;

        if( cost < min_cost ) {
            min_cost = cost;
            max_drinks = n_drinks;
        } else if( cost == min_cost ) {
            if( max_drinks < n_drinks ) {
                min_cost = cost;
                max_drinks = n_drinks;
            }
        }
    }
    
    memo[i][k] = pair<int,int>(min_cost,max_drinks);
    return pair<int,int>(min_cost,max_drinks);

}




void run()
{
    int n,k;
    std::cin>>n>>k;
    
    //cout<<"tk:"<<k<<endl;
    std::vector<int> price(n);
    std::vector<int> volume(n);
    std::vector<Pair> criteria(n);
    for( int i = 0; i < n; i++ ) {
        std::cin>>price[i];
        std::cin>>volume[i];
        //criteria[i] = Pair(volume[i],i);
    }

    //std::sort(criteria.begin(),criteria.end(),std::greater<Pair>());

    int v_n = n;
    std::vector<bool> valid(n,true);//valid
    for( int i = 0; i < n; i++ ) {
        int pi = price[i];
        int vi = volume[i];
        double pvi = double(pi)/double(vi);
        for( int j = 0; j < n; j++ ) {
            if( not valid[j] ) continue;
            if( i == j ) continue;
            int pj = price[j];
            int vj = volume[j];
            double pvj = double(pj)/double(vj);

            if( vi <= vj and vj % vi == 0 and pvi < pvj ) {
                //If i has smaller volume, j is multiple, and is cheaper per volume we can always replace a j choice with a cheaper i choice
                //j will never be in an optimal solution
                //cout<<"vi:"<<vi<<" vj:"<<vj<<"  pvi:"<<pvi<<" pvj:"<<pvj<<std::endl;
                valid[j] = false; //Overriden by the other
                v_n--;
            }
        }
    }
    
    vector<int> g_volume(v_n);
    vector<int> g_price(v_n);
    int c = 0;
    //std::cout<<v_n<<std::endl;
    for( int i = 0; i < n; i++ ) {
        if( valid[i] ) {
            g_volume[c] = volume[i];
            g_price[c] = price[i];
            c++;
        }
    }
    //std::cout<<c<<std::endl;

    vector<vector<pair<int,int>>> memo(n,vector<pair<int,int>>(k+1,pair<int,int>(-1,-1)));
    //pair<int,int> p = max_amount(0,n,k,volume,price,memo);
    pair<int,int> p = max_amount(0,v_n,k,numeric_limits<int>::max(),g_volume,g_price,memo);
    std::cout<<p.first<<" "<<p.second<<std::endl;


}

int main()
{
    std::ios_base::sync_with_stdio(true);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
