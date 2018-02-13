#include <iostream>
#include <vector>
#include <utility>
using namespace std;



int nlights, k, pattern;
vector<pair<int,bool>> time_table;

//Get minimum amount of time required to turn integer a into integer b
//Return true if an odd number of "left-bit-flips" was required
pair<int,bool> get_time( int a, const int b )
{
    if( a == b ) {
        return pair<int,bool>(0,false);
    }
    pair<int,bool> temp;

    temp = time_table[a];
    if( temp.first != -1 ) return temp;
    
    int mintime = -1;
    bool flipped = false;
    for( int i = 0; i < k; i++ ) {
        //Flip
        temp = get_time( a xor 1<<i, b );
        if( (mintime == -1) or (mintime > temp.first)) {
            mintime = temp.first;
            flipped = temp.second;
        }

        //Flip all till k
        temp = get_time( a xor ((1<<(i+1)) - 1), b );
        if( (mintime == -1) or (mintime > temp.first)) {
            mintime = temp.first;
            flipped = not temp.second; //We do a flip!
        }
        
    }
    time_table[a] = pair<int,bool>(1+mintime,flipped);
    return pair<int,bool>(1+mintime,flipped);

}


void get_pattern()
{
    cin>>nlights>>k>>pattern;

    vector<int> lights(nlights/k,0);
    cout<<nlights/k<<endl;
    for( int i = 0; i < nlights; i++ ) {
        int val;
        cin>>val;
        lights[i/k] |= val<<(k-(i%k)-1);
    }

    time_table = vector<pair<int,bool>>(1<<k, pair<int,bool>(-1,false));
    cout<<"table"<<endl;
    cout<<lights[0]<<","<<pattern<<endl;
    cout<<time_table.size()<<endl;
    for( int i = 0; i < lights.size(); i++ ) {
        cout<<"light"<<i<<":"<<lights[i]<<endl;
    }
    cout<<"res:"<<get_time(lights[0],pattern).first<<endl;;

    int time = 0;
    bool flipped = false;
    for( int i = lights.size()-1; i >= 0; i-- ) {
        pair<int,bool> temp;
        if( not flipped ) temp = get_time(lights[i],pattern);
        else temp = get_time( lights[i] xor ((1<<(k+1))-1), pattern );
        time += temp.first;
        flipped = temp.second;
    }
    cout<<time<<endl;

}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_pattern();
    }
}
