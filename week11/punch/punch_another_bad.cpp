#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;



const long LMAX = numeric_limits<long>::max();




long opt_p_for_k( long k,
                  vector<int> &indices,
                  vector<long> &v, vector<long> &p,
                  vector<long> &memo1,
                  vector<vector<int>> &memo2)
{
    if( memo1[k] != LMAX ) return memo1[k];
    int n = indices.size();
    long cheapest = LMAX;
    vector<int> candidates;
    for( int i = 0; i < n; i++ ) {
        int idx = indices[i];

        if( k == v[idx] ) {
            //cheapest = min(cheapest,p[idx]);
            if( cheapest > p[idx] ) {
                cheapest = p[idx];
                candidates = vector<int>(1); candidates[0] = idx;
            } if( cheapest == p[idx] ) {
                candidates.push_back(idx);;
            }
        } else if( k > v[idx] ) {
            long temp = opt_p_for_k( k - v[idx], indices, v, p, memo1, memo2 );
            if( temp == -1 ) continue;


            if( cheapest > temp+p[idx] ) {
                cheapest = temp+p[idx];
                candidates = vector<int>(1); candidates[0] = idx;
            } if( cheapest == temp + p[idx] ) {
                candidates.push_back(idx);;
            }
            //cheapest = min(cheapest, temp+p[idx]);
        }
    }

    //Build the optimal config
    //vector<set<long>> optimal_combos(0);
    //for( int i = 0; i < candidates.size(); i++ ) {
    //    int idx = candidates[i];
    //    int vol = k - v[idx];
    //    if( vol > 0 ) {
    //        for( int j = 0; j < memo2[vol].size(); j++ ) {
    //            optimal_combos.push_back(memo2[vol][j]);
    //            optimal_combos.back().insert(idx);
    //        }
    //    } else if( vol == 0 ) {
    //        optimal_combos.push_back(set<long>({idx}));
    //    }

    //}
    memo2[k] = candidates;
    ///
    
    if( cheapest == LMAX ) {
        memo1[k] = -1;
        return -1;
    }
    memo1[k] = cheapest;
    return memo1[k];
}

int search_max( int i, long prev_idx, vector<int> &count, vector<vector<int>> &memo2, vector<long> &v )
{
    if( i == 0 ) {
        //base case
        int c = 0;
        for( int j = 0; j < count.size(); j++ ) if( count[j] > 0 ) c++;
        return c;
    }
    int n = memo2[i].size();
    cout<<"i,j:("<<i<<","<<") " ;
    for( int j = 0; j < memo2[i].size(); j++ ) cout<<memo2[i][j]<<" "; cout<<endl;
    int largest = -1;
    vector<bool> searched(count.size(),false);
    for( int j = n-1; j >= 0; j-- ) {
        int idx = memo2[i][j];
        if( searched[idx] ) continue;
        searched[idx] = true;
        if( idx < prev_idx ) continue;
        int next_i =  i - v[idx];
        count[idx]++;
        largest = max( largest, search_max(next_i,idx,count,memo2,v));
        count[idx]--;
    }

    return largest;
}


//int search_max( int i, long prev_idx, int price, vector<int> &count,
//                const vector<int> indices, const vector<long> &v, const vector<long> &p )
//{
//
//}


void run()
{
    int n,k;
    cin>>n>>k;
    
    long max_v = 0;
    vector<long> p(n);//Price
    vector<long> v(n);//Volume
    vector<double> pv(n);//Price/volume
    vector<int> pv_order(n);
    for( int i = 0; i < n; i++ ) {
        cin>>p[i];
        cin>>v[i];
        max_v = max(max_v,v[i]);
        pv[i] = double(p[i])/v[i];
        pv_order[i] = i;
    }
    long max_k = k+max_v;//Largest volume we will achieve
    
    sort(pv_order.begin(),pv_order.end(), [&pv] ( const long i1, const long i2 ) {
                return pv[i1] < pv[i2];
            });
    vector<long> temp1(n);
    vector<long> temp2(n);
    vector<double> temp3(n);
    for( int i = 0; i < n; i++ ) {
        temp1[i] = p[pv_order[i]];
        temp2[i] = v[pv_order[i]];
        temp3[i] = pv[pv_order[i]];
    }
    p = temp1;
    v = temp2;
    pv = temp3;
   


    //Now obtain the useful values
    vector<bool> removed(n,false);
    vector<int> uis;//valid values
    uis.reserve(n);
    //Can do, but it seems to be useless! of course, the n factor is too small to bother improving it!
    //This part works, but I remove it to see the impact
    int valid_u = n;
    for( int i = 0; i < n; i++ ) {
        if( removed[i]) continue;
        for( int j = 0; j < n; j++ ) {
            if( removed[j] ) continue;
            if( pv[i] < pv[j] and (v[j] % v[i]) == 0 ) {
                removed[j] = true;
                valid_u--;
            }
        }
    }

    vector<int> opt_uis;
    for( int i = 0; i < n; i++ ) if( not removed[i] ) opt_uis.push_back(i);

    //Useless indices have been found
    vector<long> memo1(max_k, LMAX);
    vector<vector<int>> memo2(max_k, vector<int>(opt_uis.size()));
    long price_opt = LMAX;
    //cerr<<"call"<<endl;
    for( int i = k; i < max_k; i++ ) {
        long temp = opt_p_for_k( i, opt_uis, v, p, memo1, memo2 );
        if( temp == -1 ) continue;
        price_opt = min(price_opt,temp);
    }

    
    //int opt_num = -1;
    //for( int i = k; i < max_k; i++ ) {
    //    if( price_opt == memo1[i] ) {//It's "free"
    //        vector<int> count(opt_uis.size(),0);
    //        opt_num = max(opt_num, search_max( i, -1, count, memo2, v ));
    //    }
    //}

    //std::cout<<price_opt<<" "<<opt_num<<std::endl;


}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
