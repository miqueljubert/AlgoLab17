#include <iostream>
#include <vector>
#include <limits>
using namespace std;




int distance(int meeple, bool even, int goal, const vector<vector<int>> &transitions, vector<int> &distance_min, vector<int> &distance_max) {
    //Base case, he reaches the goal
    if( meeple == goal ) return 1;

    if( even and distance_min[meeple] != numeric_limits<int>::max() ) return distance_min[meeple];
    if( not even and distance_max[meeple] != -1 ) return distance_max[meeple];

    if( even ) {
        int min_distance = numeric_limits<int>::max();
        for( int i = 0; i < transitions[meeple].size(); i++ ) {
            int dist = distance( transitions[meeple][i], not even, goal, transitions, distance_min, distance_max );
            min_distance = min(min_distance, dist);

        }
        if( min_distance == numeric_limits<int>::max()) cout<<"m1"<<endl;
        distance_min[meeple] = min_distance+1;
        return min_distance+1;

    } else {
        int max_distance = -1;
        for( int i = 0; i < transitions[meeple].size(); i++ ) {
            int dist = distance( transitions[meeple][i], not even, goal, transitions, distance_min, distance_max );
            max_distance = max(max_distance, dist);
        }
        if( max_distance == -1) cout<<"m2"<<endl;
        distance_max[meeple] = max_distance+1;
        return max_distance+1;


    }
}


bool play_game()
{
    int npositions,ntransitions;
    cin>>npositions>>ntransitions;

    int red,black;
    cin>>red>>black;
    //red;black;

    vector<vector<int>> transitions(npositions,vector<int>(0));
    for( int i = 0; i < ntransitions; i++ ) {
        int u,v;
        cin>>u>>v;
        //u;v;
        transitions[u-1].push_back(v-1);
    }
    
    vector<int> distance_min(npositions,numeric_limits<int>::max());
    vector<int> distance_max(npositions,-1);

    int d1 = distance(red-1,true,npositions-1,transitions,distance_min,distance_max);
    int d2 = distance(black-1,true,npositions-1,transitions,distance_min,distance_max);
    if( d1 == d2 ) {
        if( d1%2 ) cout<<"1"<<endl;
        else cout<<"0"<<endl;
    }
    else cout<<(d1>d2)<<endl;

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
