#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;



void run()
{
    int n;
    cin>>n;
    vector<long> planks(n);
    for( int i = 0; i < n; i++ ) cin>>planks[i];


    int number = 0;
    //Divide into two equally-sized subsets
    for( int b = 0x00; b < (0x01<<n); b++ ) {
        long value1 = 0;
        long value2 = 0;
        vector<int> idxs_1;
        vector<int> idxs_2;
        for( int i = 0; i < n; i++ ) {
            if( b & (0x01<<i) ) {
                value1 += planks[i];
                idxs_1.push_back(i);
            } else {
                value2 += planks[i];
                idxs_2.push_back(i);
            }
        }

        if( value1 != value2 ) continue;
        

        vector<int> values_1;
        //Do the same for the second subset
        int count1 = idxs_1.size();
        for( int b1 = 0x00; b1 < 0x01<<count1; b1++ ) {
            long value3 = 0;
            long value4 = 0;
            vector<int> idxs_3;
            vector<int> idxs_4;
            for( int i = 0; i < count1; i++ ) {
                if( b1 & (0x01<<i) ) {
                    value3 += planks[idxs_1[i]];
                    //idxs_3.push_back(idxs_1[i]);
                } else {
                    value4 += planks[idxs_1[i]];
                    //idxs_4.push_back(idxs_1[i]);
                }
            }
            if( value3 != value4 ) continue;

            values_1.push_back(value3);
        }

        vector<int> values_2;
        //Do the same for the second subset
        int count2 = idxs_2.size();
        for( int b2 = 0x00; b2 < 0x01<<count2; b2++ ) {
            long value3 = 0;
            long value4 = 0;
            vector<int> idxs_3;
            vector<int> idxs_4;
            for( int i = 0; i < count2; i++ ) {
                if( b2 & (0x01<<i) ) {
                    value3 += planks[idxs_2[i]];
                    //idxs_3.push_back(idxs_1[i]);
                } else {
                    value4 += planks[idxs_2[i]];
                    //idxs_4.push_back(idxs_1[i]);
                }
            }
            if( value3 != value4 ) continue;

            values_2.push_back(value3);
        }
        sort(values_2.begin(),values_2.end());
        for( int i = 0; i < values_1.size(); i++ ) {
            if( binary_search(values_2.begin(),values_2.end(),values_1[i]) ) {
                //cout<<"1"<<endl;
                //return;
                number++;
            }
        }


    }
    cout<<number<<endl;

}


















int main()
{
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}
