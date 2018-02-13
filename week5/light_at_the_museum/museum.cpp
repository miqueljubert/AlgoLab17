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



void backtrack_and_list(    int current,
                            int mask_value,
                            const int end,
                            int nset,
                            int offset,
                            vector<pair<vector<int>,int>> &brightness_list,
                            const vector<vector<int>> &lights_on,
                            const vector<vector<int>> &lights_off )
{
    
    //Cas base
    if( current == end ) {
        //cout<<current<<" "<<end<<"-"<<nset<<":";print_binary(mask_value,end);cout<<endl;
        int position = mask_value;
        brightness_list[position].second = nset;
        for( int i = 0; i < end; i++ ) {//Switch
            if( 0x01 & mask_value ) {//Light is "flicked"
                for( unsigned int j = 0; j < brightness_list[position].first.size(); j++ ) {//room
                    brightness_list[position].first[j] += lights_off[offset+i][j];
                }
            } else {
                for( unsigned int j = 0; j < brightness_list[position].first.size(); j++ ) {
                    brightness_list[position].first[j] += lights_on[offset+i][j];
                }

            }
            mask_value = mask_value>>1;
        }
        return;
    }

    //Cas recursiu
    backtrack_and_list(current+1, mask_value, end, nset, offset, brightness_list, lights_on, lights_off);
    backtrack_and_list(current+1, mask_value | (0x01<<current), end, nset+1, offset, brightness_list, lights_on, lights_off);
    
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
    
    vector< pair<vector<int>,int> > brightness_list1(0x01<<(midpoint), pair<vector<int>,int>(vector<int>(brightness.size(),0),0) );
    vector< pair<vector<int>,int> > brightness_list2(0x01<<(nswitches-midpoint), pair<vector<int>,int>(vector<int>(brightness.size(),0),0) );

    backtrack_and_list(0,0,midpoint,0,0,brightness_list1,lights_on,lights_off);
    //cout<<"set2"<<endl;
    backtrack_and_list(0,0,nswitches-midpoint,0,midpoint,brightness_list2,lights_on,lights_off);

    sort(brightness_list1.begin(), brightness_list1.end());
    sort(brightness_list2.begin(), brightness_list2.end());
    //cout<<"list1:"<<endl;
    //for( int i = 0; i < brightness_list1.size(); i++ ){ 
    //    cout<<brightness_list1[i].second<<": ";
    //    for( int j = 0; j < brightness_list1[i].first.size(); j++ ){ 
    //        cout<<brightness_list1[i].first[j]<<" ";
    //    }
    //    cout<<endl;
    //}
    //cout<<"list2:"<<endl;
    //for( int i = 0; i < brightness_list2.size(); i++ ){ 
    //    cout<<brightness_list2[i].second<<": ";
    //    for( int j = 0; j < brightness_list2[i].first.size(); j++ ){ 
    //        cout<<brightness_list2[i].first[j]<<" ";
    //    }
    //    cout<<endl;
    //}
    //cout<<"--"<<endl;
    //for( int i = 0; i < brightness.size(); i++ ){ 
    //        cout<<brightness[i]<<" ";
    //}
    //cout<<endl;
    //cout<<"-----------"<<endl;

    
    //for each of the results
    int minimum = 60;
    //cout<<"search targets"<<endl;
    for( int i = 0; i < brightness_list1.size(); i++ ) {
        pair<vector<int>,int> search_target(pair<vector<int>,int>(vector<int>(brightness.size(),0),0));
        for( int j = 0; j < search_target.first.size(); j++ ) search_target.first[j] = brightness[j] - brightness_list1[i].first[j];
        //for( int i = 0; i < search_target.first.size(); i++ ) {
        //    cout<<search_target.first[i]<<" ";
        //}
        //cout<<endl;

        bool exists = binary_search(
                                    brightness_list2.begin(),
                                    brightness_list2.end(),
                                    search_target,
                                    []( const pair<vector<int>,int> &p1, const pair<vector<int>,int> &p2) { return p1.first < p2.first; });

        if( exists ) {
            //cout<<"ww"<<endl;
            auto lb = lower_bound(brightness_list2.begin(),brightness_list2.end(),search_target,[]( const pair<vector<int>,int> &p1, const pair<vector<int>,int> &p2) { return p1.first < p2.first; });
            auto ub = upper_bound(brightness_list2.begin(),brightness_list2.end(),search_target,[]( const pair<vector<int>,int> &p1, const pair<vector<int>,int> &p2) { return p1.first < p2.first; });
            while(lb != ub) {
                //cout<<"nset:"<<(*lb).second<<"+"<<brightness_list1[i].second<<" min:"<<minimum<<endl;
                //for( int j = 0; j < (*lb).first.size(); j++ ) {
                 //   cout<<((*lb).first[j]+brightness_list1[i].first[j] == brightness[j])<<endl;
                //}
                minimum = min(minimum,(*lb).second+brightness_list1[i].second);
                //cout<<minimum<<endl;
                lb++;
            }
        }

    }
    if( minimum != 60 ) cout<<minimum<<endl;
    else cout<<"impossible"<<endl;
    //cout<<endl<<endl;

}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_room_light();
    }
}
