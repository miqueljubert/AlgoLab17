#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

typedef pair<int,vector<int>> Pair;


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


    int n1 = n/2;
    int n2 = n - n1;

    vector<bool> seen1(0x01<<n1,false);
    vector<Pair> list1;
    for( int b = 0x00; b < (0x01<<n1); b++ ) {
        if( seen1[b] ) continue;
        seen1[ ~b & ((0x01<<n1) -1) ] = true;
        //First split and list
        long value1 = 0;
        vector<int> idxs;
        for( int i = 0; i < n1; i++ ) {
            if( b & (0x01<<i) ) {
                value1 += planks[i];
                idxs.push_back(i);
            } 
        }

        list1.push_back(Pair(value1,idxs));
    }
    vector<bool> seen2(0x01<<n2,false);
    vector<Pair> list2;
    for( int b = 0x00; b < (0x01<<n2); b++ ) {
        if( seen2[b] ) continue;
        seen2[ ~b & ((0x01<<n2) -1) ] = true;

        //First split and list
        long value2 = 0;
        vector<int> idxs;
        for( int i = 0; i < n2; i++ ) {
            if( b & (0x01<<i) ) {
                value2 += planks[n1+i];
                idxs.push_back(n1+i);
            } 
        }

        list2.push_back(Pair(value2,idxs));
    }

    sort(list2.begin(),list2.end());
    
    vector<vector<int>> candidates1;
    vector<vector<int>> candidates2;
    int half = tot_value/2;
    int quarter = tot_value/4;
    for( Pair el : list1 ) { //For each element in the first list
        int rem = half-el.first;
        auto lb = lower_bound(list2.begin(),list2.end(),Pair(rem,vector<int>()),
                []( const Pair &p, const Pair &v) {
                     return p.first < v.first;
                });
        auto ub = upper_bound(list2.begin(),list2.end(),Pair(rem,vector<int>()),
                []( const Pair &p, const Pair &v) {
                     return p.first < v.first;
                });
        while( lb != ub ) {
            vector<int> idxs = el.second;
            for( int idx : lb->second ) idxs.push_back(idx);
            
            vector<int> idxs_2;
            int i = 0;
            for( int j = 0; j < n; j++ ) {
                if( idxs[i] == j ) {
                    i++;
                } else {
                    idxs_2.push_back(j);
                }
            }
            //for( int idx : idxs )   cout<<idx<<",";cout<<endl;
            //for( int idx : idxs_2 ) cout<<idx<<",";cout<<endl;

            candidates1.push_back(idxs);
            candidates2.push_back(idxs_2);
            lb++;
        }
    }
    
    long different = 0;
    for( int i = 0; i < candidates1.size(); i++ ) {

        int valid_1 = 0;
        vector<int> idxs_1 = candidates1[i];
        int count1 = idxs_1.size();
        vector<bool> seen1(0x01<<count1,false);
        for( int b1 = 0x00; b1 < 0x01<<count1; b1++ ) {
            if( seen1[b1] ) continue;
            seen1[ ~b1 & ((0x01<<count1) -1) ] = true;
            long value3 = 0;
            long value4 = 0;
            for( int j = 0; j < count1; j++ ) {
                if( b1 & (0x01<<j) ) {
                    value3 += planks[idxs_1[j]];
                } else {
                    value4 += planks[idxs_1[j]];
                }
            }
            if( value3 != quarter ) continue;
            
            valid_1++;
        }

        int valid_2 = 0;
        vector<int> idxs_2 = candidates2[i];
        int count2 = idxs_2.size();
        vector<bool> seen2(0x01<<count2,false);
        //Do the same for the second subset
        for( int b2 = 0x00; b2 < 0x01<<count2; b2++ ) {
            if( seen2[b2] ) continue;
            seen2[ ~b2 & ((0x01<<count2) -1) ] = true;
            long value3 = 0;
            long value4 = 0;
            for( int j = 0; j < count2; j++ ) {
                if( b2 & (0x01<<j) ) {
                    value3 += planks[idxs_2[j]];
                } else {
                    value4 += planks[idxs_2[j]];
                }
            }
            if( value3 != quarter ) continue;
            
            valid_2++;
        }

        different += valid_1*valid_2;

    }
    cout<<different<<endl;
   
}


















int main()
{
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
