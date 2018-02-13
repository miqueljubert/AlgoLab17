#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;



void print_binary( int num, int n )
{
    for( int i = 0; i < n; i++ ) {
        bool b = 0x01 & (num>>(n-i)-1);
        if( b ) cout<<"1";
        else cout<<"0";
    }
    cout<<endl;
}

long get_count( int n1, int n2, int n, int k,
                vector<int> &subsets_1,
                vector<int> &subsets_2 )
{

}


void run()
{
    int n,k;
    cin>>n>>k;
    
    vector<int> values(n);
    for( int i = 0; i < n; i++ ) {
        int v;
        cin>>v;
        values[i] = v;
    }
   

    //split and list version
    int n1 = n/2;
    int n2 = n-n1;
    map<int,vector<int>> ss_1;
    for( int i = 0; i <= n1; i++ ) {
        ss_1[i] = vector<int>();
        ss_1[i].reserve(0x01<<n);
    }

    for( int b = 0x00; b < 0x01<<n1; b++ ) {
        int set_size = 0;
        vector<int> idxs;
        for( int j = 0; j < n1; j++ ) {
            bool bit = b & (0x01<<j);
            if( bit ) {
                set_size++;
                idxs.push_back(j);
            }
        }
        //cout<<"set_size:"<<set_size<<endl;
        vector<int> sums(1,0);
        for( int i = 0; i < idxs.size(); i++ ) {//For each element
            vector<int> prev_sums = sums;
            sums = vector<int>(prev_sums.size()*2);
            for( long j = 0; j < prev_sums.size(); j++ ) {
                sums[j] = prev_sums[j] + values[idxs[i]];
                sums[prev_sums.size() + j] = prev_sums[j] - values[idxs[i]];
            }
        }
        //for( long i = 0; i < sums.size(); i++ ) {
        //    ss_1[set_size].push_back(sums[i]);
        //}
        ss_1[set_size].insert(ss_1[set_size].end(),sums.begin(),sums.end());
    }
    map<int,vector<int>> ss_2;
    for( int i = 0; i <= n2; i++ ) {
        ss_2[i] = vector<int>();
        ss_2[i].reserve(0x01<<n);
    }

    for( int b = 0x00; b < 0x01<<n2; b++ ) {
        int set_size = 0;
        vector<int> idxs;
        for( int j = 0; j < n2; j++ ) {
            bool bit = b & (0x01<<j);
            if( bit ) {
                set_size++;
                idxs.push_back(j+n1);
            }
        }
        vector<int> sums(1,0);
        for( int i = 0; i < idxs.size(); i++ ) {//For each element
            vector<int> prev_sums = sums;
            sums = vector<int>(prev_sums.size()*2);
            for( long j = 0; j < prev_sums.size(); j++ ) {
                sums[j] = prev_sums[j] + values[idxs[i]];
                sums[prev_sums.size() + j] = prev_sums[j] - values[idxs[i]];
            }
        }
        //for( long i = 0; i < sums.size(); i++ ) {
        //    ss_2[set_size].push_back(sums[i]);
        //}
        ss_2[set_size].insert(ss_2[set_size].end(),sums.begin(),sums.end());
    }
    for( int i = 0; i <= n2; i++ ) sort(ss_2[i].begin(),ss_2[i].end());
    
    long count = 0;
    for( int i = 0; i <= n1; i++ ) {
        for( int j = 0; j <= n2; j++ ) {
            if( (i+j) < (n-k) ) continue;
            //cout<<(i+j)<<">="<<n<<"-"<<k<<endl;
            for( int l = 0; l < ss_1[i].size(); l++ ) {
                auto lb = lower_bound(ss_2[j].begin(),ss_2[j].end(),-ss_1[i][l]);
                auto ub = upper_bound(ss_2[j].begin(),ss_2[j].end(),-ss_1[i][l]);
                while( lb != ub ) {
                    count++;
                    lb++;
                }
            }
        }
    }

    //for( int i = 0; i <= n1; i++ ) {
    //    cout<<"1;"<<i<<": ";
    //    for( int j = 0; j < ss_1[i].size(); j++ ) {
    //        cout<<ss_1[i][j]<<",";
    //    }
    //    cout<<endl;
    //}
    //for( int i = 0; i <= n2; i++ ) {
    //    cout<<"2;"<<i<<": ";
    //    for( int j = 0; j < ss_2[i].size(); j++ ) {
    //        cout<<ss_2[i][j]<<",";
    //    }
    //    cout<<endl;
    //}

    cout<<count<<endl;



}



int main()
{
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
