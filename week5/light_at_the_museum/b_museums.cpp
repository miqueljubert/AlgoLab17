#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;



void print_binary( int num, int k )
{
    for( int i = 0; i < k; i++ ) {
        int p = num & (0x01<<(k-1));
        cout<<(p>>(k-1));
        num = num<<1;
    }
}



void backtrack_and_list(int current, int mask_value, const int end, vector<vector<int>> &brightness_list, const vector<vector<int>> &lights_on, const vector<vector<int>> &lights_off)
{
    
    //Cas base
    if( current == end ) {
        int position = mask_value;
        //Will it be fast enough?
        //cout<<"end:"<<end<<endl;
        for( int i = 0; i < end; i++ ) {//Switch
            if( 0x01 & mask_value ) {//Light is on
                for( unsigned int j = 0; j < brightness_list[position].size(); j++ ) {//room
                    //brightness_list[position][j] -= lights_on[i][j];
                    brightness_list[position][j] += lights_on[i][j];
                }
            } else {
                for( unsigned int j = 0; j < brightness_list[i].size(); j++ ) {
                    //brightness_list[position][j] -= lights_off[i][j];
                    brightness_list[position][j] += lights_off[i][j];
                }

            }
            mask_value = mask_value>>1;
        }
        return;
    }

    //Cas recursiu
    backtrack_and_list(current+1, mask_value, end, brightness_list, lights_on, lights_off);
    backtrack_and_list(current+1, mask_value | (0x01<<current), end, brightness_list, lights_on, lights_off);
    
}


int nones(int n)
{
    int o = 0;
    while( n != 0 ) {
        if( n & 0x01 ) o++;
        n = n>>1;
    }
    return o;
}

void get_room_light()
{
    int nswitches,nrooms;
    cin>>nswitches>>nrooms;

    vector<int> brightness(nrooms);
    vector<vector<int>> lights_on( nswitches,   vector<int>(nrooms));
    vector<vector<int>> lights_off(nswitches,   vector<int>(nrooms));


    for( int i = 0; i < nrooms; i++ ) {
        cin>>brightness[i];
    }
    for( int i = 0; i < nswitches; i++ ) {
        for( int j = 0; j < nrooms; j++ ) {
            cin>>lights_on[i][j];
            cin>>lights_off[i][j];
        }
    }

    int midpoint = nswitches/2;
    //int second = midpoint+1;
    
    //vector<vector<int>> brightness_list1(0x01<<(midpoint),vector<int>(brightness));
    //vector<vector<int>> brightness_list2(0x01<<(nswitches-midpoint),vector<int>(brightness));
    vector<vector<int>> brightness_list1(0x01<<(midpoint),vector<int>(brightness.size(),0));
    vector<vector<int>> brightness_list2(0x01<<(nswitches-midpoint),vector<int>(brightness.size(),0));
    //cout<<"size:"<<nswitches<<endl;
    //cout<<"midpoint:"<<midpoint<<endl;
    //cout<<brightness_list1.size()<<endl;
    //cout<<brightness_list2.size()<<endl;
    backtrack_and_list(0,0,midpoint,brightness_list1,lights_on,lights_off);
    backtrack_and_list(0,0,nswitches-midpoint,brightness_list2,lights_on,lights_off);

    sort(brightness_list1.begin(), brightness_list1.end());
    sort(brightness_list2.begin(), brightness_list2.end());
    cout<<"list1:"<<endl;
    for( int i = 0; i < brightness_list1.size(); i++ ){ 
        for( int j = 0; j < brightness_list1[i].size(); j++ ){ 
            cout<<brightness_list1[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"list2:"<<endl;
    for( int i = 0; i < brightness_list2.size(); i++ ){ 
        for( int j = 0; j < brightness_list2[i].size(); j++ ){ 
            cout<<brightness_list2[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"--"<<endl;
    for( int i = 0; i < brightness.size(); i++ ){ 
            cout<<brightness[i]<<" ";
    }
    cout<<endl;
    cout<<"-----------"<<endl;

    
    //for each of the results
    for( int i = 0; i < brightness_list1.size(); i++ ) {
        vector<int> search_target(brightness);
        for( int j = 0; j < search_target.size(); j++ ) search_target[j] = brightness[j] - brightness_list1[i][j];
        bool exists = binary_search(brightness_list2.begin(),brightness_list2.end(),search_target);
        if( exists ) {
            cout<<"ww"<<endl;
            auto lb = lower_bound(brightness_list2.begin(),brightness_list2.end(),search_target);
            auto ub = upper_bound(brightness_list2.begin(),brightness_list2.end(),search_target);

            while(true) {
                int position = lb - brightness_list2.begin();
                cout<<position<<endl;
                if( lb == ub ) break;
                lb++;
            }
        }

    }

}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_room_light();
    }
}
