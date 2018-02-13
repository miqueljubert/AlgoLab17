#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;


typedef pair<int,int> interval;

int max_value_k_intervals(int curr, int n, int m,
                            const vector<interval> &intervals,
                            const vector<int> &values,
                            vector<vector<int>> &memo)
{
    if( curr == n and m > 0) return -1;//No valid way, m > 0 
    if( curr == n ) return 0;
    if( m == 0 ) return 0;
    if( memo[curr][m-1] != 0 ) return memo[curr][m-1];

    //int nc = curr;
    //while( nc < n and intervals[curr].second >= intervals[nc].first ) {
    //    nc++;
    //}
    auto it = upper_bound(intervals.begin()+curr,intervals.end(),intervals[curr],
            [](const interval &i, const interval &f ) {
                return i.second < f.first;
            });
    int nc = it-intervals.begin();

    //I pick this one
    int i1 = max_value_k_intervals(nc,n,m-1,intervals,values,memo);
    int i1v = (intervals[curr].second-intervals[curr].first+1);
    //I don't
    int i2 = max_value_k_intervals(curr+1,n,m,intervals,values,memo);

    if( i1 == -1 and i2 == -1 ) {
        memo[curr][m-1] = -1;
        return -1;
    }
    if( i1 == -1 ) {
        memo[curr][m-1] = i2;
        return i2;
    }
    if( i2 == -1 ) {
        memo[curr][m-1] = i1+i1v;
        return i1+i1v;
    }

    memo[curr][m-1] = max(i2,i1+i1v);

    return max(i2,i1+i1v);

}
void run()
{
    int n,m,k;
    cin>>n>>m>>k;
    
    vector<int> defenders(n);
    for( int i = 0; i < n; i++ ) {
        cin>>defenders[i];
    }
    

    vector<interval> intervals;
    int start = 0;
    int end = 0;
    int val = defenders[0];
    while( true ) {
        if( val == k ) intervals.push_back(interval(start,end));
        if( val < k ) {
            end++;
            if( end == n ) break;
            val += defenders[end];
        } else { // >= k
            val -= defenders[start];
            start++;
        }
    }

    //for( interval i : intervals ) cout<<i.first<<","<<i.second<<endl;
    //sort( intervals.begin(), intervals.end(),
    //        []( const interval &i1, const interval &i2 ) {
    //            return i1.second < i2.second;
    //        });
    
    vector<vector<int>> memo(intervals.size(),vector<int>(m,0)); 
    int r = max_value_k_intervals(0,intervals.size(),m,intervals,defenders,memo);
    if( r == -1 ) cout<<"fail"<<endl;
    else cout<<r<<endl;
    


}



int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
