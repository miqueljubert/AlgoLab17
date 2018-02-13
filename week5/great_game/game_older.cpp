#include <iostream>
#include <vector>
#include <limits>
using namespace std;




int distance(int meeple, bool even, int goal, const vector<vector<int>> &transitions, vector<int> &distance_min, vector<int> &distance_max) {
    //Base case, he reaches the goal
    if( meeple == goal ) return 0;

    //If we have memoized it
    if( even && (distance_min[meeple] != -1) ) return distance_min[meeple];
    if( (not even) && (distance_max[meeple] != -1) ) return distance_max[meeple];
    
    //If he cannot transition
    if( transitions[meeple].size() == 0 ) return -1;

    if( even ) {
        int min_distance = distance( transitions[meeple][0], not even, goal, transitions, distance_min, distance_max );
        for( int i = 1; i < transitions[meeple].size(); i++ ) {
            int dist = distance( transitions[meeple][i], not even, goal, transitions, distance_min, distance_max );
            if( dist != -1 ) min_distance = min(min_distance, dist);
        }
        if( min_distance != -1 ) {

            if(distance_min[meeple] != -1) distance_min[meeple] = min(min_distance+1, distance_min[meeple]);
            else distance_min[meeple] = min_distance+1;

            return min_distance+1;
        } else {
            return -1;
        }
    } else {
        int max_distance = distance( transitions[meeple][0], not even, goal, transitions, distance_min, distance_max );
        for( int i = 1; i < transitions[meeple].size(); i++ ) {
            int dist = distance( transitions[meeple][i], not even, goal, transitions, distance_min, distance_max );
            if( dist != -1 ) max_distance = max(max_distance, dist);
        }
        if( max_distance != -1 ) {

            if(distance_max[meeple] != -1) distance_max[meeple] = max(max_distance+1, distance_max[meeple]);
            else distance_max[meeple] = max_distance+1;

            return max_distance+1;
        } else {
            return -1;
        }
    }
}


bool play_game()
{
    int npositions,ntransitions;
    cin>>npositions>>ntransitions;

    int red,black;
    cin>>red>>black;
    red--;black--;

    vector<vector<int>> transitions(npositions,vector<int>(0));
    for( int i = 0; i < ntransitions; i++ ) {
        int u,v;
        cin>>u>>v;
        u--;v--;
        transitions[u].push_back(v);
    }
    
    vector<int> distance_min(npositions,-1);
    vector<int> distance_max(npositions,-1);

    int ret2 = distance(black,true,npositions-1,transitions,distance_min,distance_max);
    int ret1 = distance(red,true,npositions-1,transitions,distance_min,distance_max);
    if( ret1 == -1 ) cout<<"1"<<endl;
    else if( ret2 == -1 ) cout<<"0"<<endl;
    else cout<<(ret1>=ret2)<<endl;
    //cout<<"ret1:"<<ret1<<endl;
    //cout<<"ret2:"<<ret2<<endl;
    //cout<<"--"<<endl;

}


int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) {
        play_game();
    }
}
