#include <iostream>
#include <vector>
using namespace std;




//vector<int> brightness;
//vector<vector<int>> lights_on;
//vector<vector<int>> lights_off;




int backtrack( int s, int nswitches, int nrooms, vector<int> &brightness, vector<vector<int>> &lights_on, vector<vector<int>> &lights_off, int nswitched, int &minimum )
{
    //Cas base
    if( s == nswitches-1 ) {
        //comprova si la condicio ja es satisfa
        bool satisfied = true;
        
        for( int i = 0; i < nrooms; i++ ) {
            if( brightness[i] != lights_on[s][i] ) { //Si falta llum a una habitacio
                satisfied = false;
                break;
            }
        }
        if( satisfied ) {
            minimum = min(nswitched,minimum);
            return 0; //No cal tocar cap interruptor
        }
        
        satisfied = true;
        for( int i = 0; i < nrooms; i++ ) {
            if( brightness[i] != lights_off[s][i] ) { //Si falta llum a una habitacio
                satisfied = false;
                break;
            }
        }

        if( satisfied ) {
            minimum = min(nswitched+1,minimum);
            return 1;//Switching the last one will work
        }
        
        return -1; //no way to do it

    }

    //Recursive case
    //We try without pressing the switch
    bool impossible = false;
    for( int i = 0; i < nrooms; i++ ) {
        brightness[i] -= lights_on[s][i];
        if( brightness[i] < 0 ) {
            impossible = true;
        }
    }
    if(impossible){
        for( int i = 0; i < nrooms; i++ ) {
            brightness[i] += lights_on[s][i];
        }
        return -1;
    }
    int res0 = backtrack(s+1,nswitches,nrooms,brightness,lights_on,lights_off, nswitched, minimum);
    //We press the switch
    for( int i = 0; i < nrooms; i++ ) {
        brightness[i] += lights_on[s][i];
    }

    //----------Never go over the limit-----------------
    if( (nswitched+1) >= minimum ) {//If flipping a switch pushes me over the limit
        return res0;
    }
    //----------Never go over the limit----------------

    for( int i = 0; i < nrooms; i++ ) {
        brightness[i] -= lights_off[s][i];
        if( brightness[i] < 0 ) {
            impossible = true;
        }
    }

    if(impossible){
        for( int i = 0; i < nrooms; i++ ) {
            brightness[i] += lights_off[s][i];
        }
        return res0;
    }

    int res1 = backtrack(s+1,nswitches,nrooms,brightness,lights_on,lights_off, nswitched+1, minimum);
    for( int i = 0; i < nrooms; i++ ) {
        brightness[i] += lights_off[s][i];
    }
    //cout<<"s:"<<s<<" "<<"res0:"<<res0<<" "<<"res1:"<<res1<<endl;
    if( res0 == -1 and res1 == -1 ) return -1;
    if( res0 == -1 ) {
        return res1+1;
    }
    if( res1 == -1 ) {
        return res0;
    }
    
    return min(res0, res1+1);
}

void get_room_light()
{
    int nswitches,nrooms;
    cin>>nswitches>>nrooms;

    vector<int> brightness(nrooms);
    vector<vector<int>> lights_on( nswitches,   vector<int>(nrooms));
    vector<vector<int>> lights_off(nswitches,   vector<int>(nrooms));
    //brightness = vector<int>(nrooms);
    //lights_on = vector<vector<int>>( nrooms,   vector<int>(nswitches));
    //lights_off = vector<vector<int>>(nrooms,   vector<int>(nswitches));


    for( int i = 0; i < nrooms; i++ ) {
        cin>>brightness[i];
    }
    for( int i = 0; i < nswitches; i++ ) {
        for( int j = 0; j < nrooms; j++ ) {
            cin>>lights_on[i][j];
            cin>>lights_off[i][j];
        }
    }

    int minimum = nswitches+1;
    cout<<backtrack(0, nswitches, nrooms, brightness, lights_on, lights_off, 0, minimum)<<endl;
}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_room_light();
    }
}
