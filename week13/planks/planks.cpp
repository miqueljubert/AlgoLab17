#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

//Value-indices that produce it pair
typedef pair<int,int> Pair;


void list( int n, const vector<int> &indices, const vector<long> &values,
           vector<Pair> &list )
{
    list = vector<Pair>();
    for( int b = 0x00; b < (0x01<<n); b++ ) {

        long value = 0;
        for( int i = 0; i < n; i++ ) {
            if( b & (0x01<<i) ) {
                value += values[indices[i]];
            } 
        }
        list.push_back(Pair(value,b));
    }
}

bool comp( const Pair& p1, const Pair &p2 ) {
    return p1.first < p2.first;
}

void split_and_list( int n, int target, const vector<int> &indices,
                     const vector<long> &values, vector<int> &results )
{
    results = vector<int>();
    int n1 = n/2;
    int n2 = n - n1;

    vector<Pair> list1;
    vector<int> idxs1(n1);
    for( int i = 0; i < n1; i++ ) idxs1[i] = indices[i];
    list(n1, idxs1, values, list1);

    vector<Pair> list2;
    vector<int> idxs2(n2);
    for( int i = 0; i < n2; i++ ) idxs2[i] = indices[n1+i];
    list(n2, idxs2, values, list2);
    
    vector<bool> seen(0x01<<n,false);
    sort(list2.begin(),list2.end());
    for( const Pair &p : list1 ) {
        int obj = target - p.first;
        auto lb = lower_bound(list2.begin(),list2.end(),Pair(obj,0),comp);
        auto ub = upper_bound(list2.begin(),list2.end(),Pair(obj,0),comp);
        while( lb != ub ) {
            int b1 = p.second;
            int b2 = lb->second;
            int b = b2<<n1 | b1;
            
            if( seen[b] ) {
                lb++;
                continue;
            }
            seen[ ~b & ((0x01<<n)-1)] = true;


            results.push_back(b);
            lb++;
        }
    }

}


void run()
{
    int n;
    cin>>n;
    vector<long> planks(n);

    long tot_value = 0;
    for( int i = 0; i < n; i++ ) {
        cin>>planks[i];
        tot_value += planks[i];
    }
    if( (tot_value%4) != 0 ) cout<<"0"<<endl;

    int half    = tot_value/2;
    int quarter = tot_value/4;

    int n1 = n/2;
    int n2 = n - n1;
    
    vector<int> indices(n);
    for( int i = 0; i < n; i++ ) indices[i] = i;
    
    //cout<<"first"<<endl;
    vector<int> results;
    split_and_list(n,half,indices,planks,results);

    //cout<<"second"<<endl;

    vector<bool> seen(0x01<<n,false);
    long differents = 0;
    for( int i = 0; i < results.size(); i++ ) {

        int b = results[i];
        vector<int> indices_1;
        vector<int> indices_2;

        if( seen[b] ) continue;
        seen[ ~b & ((0x01<<n)-1)] = true;


        for( int i = 0; i < n; i++ ) {
            if( b & (0x01 << i ) ) {
                indices_1.push_back(i);
            } else {
                indices_2.push_back(i);
            }
        }
        
        int n1 = indices_1.size();
        int n2 = indices_2.size();

        vector<int> results1;
        vector<int> results2;

        split_and_list(n1,quarter,indices_1,planks,results1);
        split_and_list(n2,quarter,indices_2,planks,results2);

        //cout<<results1.size()<<" "<<results2.size()<<endl;
        differents += results1.size()*results2.size();

    }
    //cout<<differents/12<<endl;
    cout<<differents/3<<endl;


}


















int main()
{
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
