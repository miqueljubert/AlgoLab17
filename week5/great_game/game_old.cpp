#include <iostream>
#include <vector>
using namespace std;


#define OUT_OF_BOUNDS 10000000


int play_single_meeple( int meeple, bool mine, const int target, vector<vector<int>> &distance, vector<vector<int>> &transitions, int limit_distance )
{
    if( meeple == target ) {
        return 0;
    }


    //cout<<"meeple:"<<meeple<<" "<<target<<" "<<transitions.size()<<endl;
    //If memorized
    if( not mine and distance[meeple][0] != -1 )
        return distance[meeple][0]+1;
    if( mine and distance[meeple][1] != -1 )
        return distance[meeple][1]+1;
    
    if( transitions[meeple].size() == 0 ){
        //cout<<"careful!"<<endl;
        if( mine ) distance[meeple][0] = OUT_OF_BOUNDS;
        else distance[meeple][1] = OUT_OF_BOUNDS;
        return OUT_OF_BOUNDS;
    }
    //cout<<"-"<<endl;

    int dist = play_single_meeple( transitions[meeple][0], not mine, target, distance, transitions, limit_distance);
    for( int i = 1; i < transitions[meeple].size(); i++ ) {
        if( mine )  min(dist,play_single_meeple( transitions[meeple][i], not mine, target, distance, transitions, limit_distance ));
        else        max(dist,play_single_meeple( transitions[meeple][i], not mine, target, distance, transitions, limit_distance ));
    }

    if( (limit_distance != -1) and ((dist + 1) > limit_distance ) ) {
        if(mine) distance[meeple][0] = OUT_OF_BOUNDS;
        else distance[meeple][1] = OUT_OF_BOUNDS;
    }

    if( mine ) distance[meeple][0] = dist+1;
    else distance[meeple][1] = dist+1;

    return dist+1;
}



bool play_game()
{
    int npositions,ntransitions;
    cin>>npositions>>ntransitions;

    int red,black;
    cin>>red>>black;
    red--;black--;

    vector<vector<int>> transitions(npositions,vector<int>(0));
    //Transition vector
    for( int i = 0; i < ntransitions; i++ ) {
        int u,v;
        cin>>u>>v;
        u-=1;v-=1;
        transitions[u].push_back(v);
    }

    vector<vector<int>> distance(npositions,vector<int>(2,-1));
    //cout<<"red:"<<red<<endl;
    int red_steps   = play_single_meeple( red, true , npositions-1, distance, transitions, -1 );
    //cout<<"black:"<<black<<endl;
    int black_steps =  play_single_meeple( black, true , npositions-1, distance, transitions, red_steps );

    return red_steps <= black_steps;

}


int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) {
        cout<<play_game()<<endl;
    }
}
