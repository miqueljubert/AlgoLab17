#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
using namespace std;




//cost,amount
pair<int,int> max_amount( int i, int n, int k, const std::vector<int> &volume, const std::vector<int> &price, vector<vector<pair<int,int>>> &memo )
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
    pair<int,int> base = max_amount(i+1,n,k,volume,price,memo);
    
    int min_cost = base.first;
    int max_drinks = base.second;
    int min_idx = 0;
    for( int j = 1; j <= m_amount; j++ ) {

        pair<int,int> r = max_amount(i+1,n,k-j*volume[i],volume,price,memo);
        int cost = r.first + j*price[i];
        int n_drinks = r.second + 1;

        if( cost < min_cost ) {
            min_cost = cost;
            max_drinks = n_drinks;
            min_idx = j;
        } else if( cost == min_cost ) {
            if( max_drinks < n_drinks ) {
                min_cost = cost;
                max_drinks = n_drinks;
                min_idx = j;
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
    std::vector<pair<double,int>> criteria(n);
    for( int i = 0; i < n; i++ ) {
        std::cin>>price[i];
        std::cin>>volume[i];
        criteria[i] = pair<double,int>(double(price[i])/double(volume[i]),i);
    }

    //std::sort(criteria.begin(),criteria.end(),std::greater<std::pair<double,int>>());
    //std::sort(criteria.begin(),criteria.end());
    std::vector<int> temp1(n);
    std::vector<int> temp2(n);
    for( int i = 0; i < n; i++ ) {
        temp1[i] = price[criteria[i].second];
        temp2[i] = volume[criteria[i].second];
        //std::cout<<criteria[i].first<<std::endl;
    }
    price = temp1;
    volume = temp2;


    vector<vector<pair<int,int>>> memo(n,vector<pair<int,int>>(k+1,pair<int,int>(-1,-1)));
    pair<int,int> p = max_amount(0,n,k,volume,price,memo);
    std::cout<<p.first<<" "<<p.second<<std::endl;
}

int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
