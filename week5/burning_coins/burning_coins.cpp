#include <iostream>
#include <vector>
using namespace std;



vector<vector<int>> game_score_v;
vector<int> coin_values;
int ncoins;


int game_score( int i, int j )
{
    //cout<<"game_score i:"<<i<<" j:"<<j<<endl;

    //cas base
    if( i > j ) {
        //cout<<"0 base"<<endl;
        return 0;
    }
    if( i == j ) {
        //cout<<"1 base"<<endl;
        return coin_values[i];
    }
    //cout<<"score:"<<game_score_v[i][j]<<endl;
    if( game_score_v[i][j] == -1 ) {
        // I pick ith coin
        game_score_v[i][j] =
            max(coin_values[i] + min(game_score(i+2,j),  game_score(i+1,j-1)),
                coin_values[j] + min(game_score(i+1,j-1),game_score(i,j-2)));
        return game_score_v[i][j];
    }
    return game_score_v[i][j];
}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        cin>>ncoins;
        
        coin_values = vector<int>(ncoins);
        for( int j = 0; j < ncoins; j++ ) {
            cin>>coin_values[j];
        }
        game_score_v = vector<vector<int>>(ncoins,vector<int>(ncoins,-1));

        cout<<game_score(0,coin_values.size()-1)<<endl;

    }
}
