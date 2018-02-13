#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;








const int max_stacks = 5;



long access_index( const vector<long> &indices, const vector<long> &sizes ) 
{
    long index =    sizes[1]*sizes[2]*sizes[3]*sizes[4]*indices[0] +
                    sizes[2]*sizes[3]*sizes[4]*indices[1] +
                    sizes[3]*sizes[4]*indices[2] + 
                    sizes[4]*indices[3] +
                    indices[4];
    return index;
}


long max_score( const vector<long> &current_positions, const int n_stacks, const vector<vector<int>> &stacks, const vector<long> &stack_sizes, vector<long> &optimal_scores )
{   
    if( optimal_scores[access_index(current_positions, stack_sizes)] != -1 )
        return optimal_scores[access_index(current_positions, stack_sizes)];


    bool finished = true;
    for( int i = 0; i < n_stacks; i++ ) {
        if( current_positions[i] > 0 ) finished = false;
    }
    if( finished ) return 0; //If we have checked all the possibilities
    
    long best_score = -1;
    //vector<bool> tried(current_positions.size(),false);
    for( unsigned int i = 0; i < (2<<5)-1; i++ ) { //try all 25 permutations. not ideal but w/e
        int color = -1;
        int color_amount = 0;
        vector<long> try_positions = current_positions;
        
        bool unvalid_subset = false;
        bool change = false;
        for( int j = 0; j < 5; j++ ) {
            if( i & (0x01<<j) ){
                if( current_positions[j] == 0 ) {
                    unvalid_subset = true;
                    break;
                }
                if( color == -1 ) {
                    change = true;
                    color = stacks[j][current_positions[j]];
                    color_amount++;
                    try_positions[j]--;
                } else {
                    if( color == stacks[j][current_positions[j]] ) {
                        try_positions[j]--;
                        color_amount++;
                    } else {
                        unvalid_subset = true;
                        break;
                    }
                }
            }
        }
        if( unvalid_subset ) continue;
        if( not change ) continue;
        long score = 0;
        if( color_amount > 1 ) score = (1<<(color_amount-2));
        //for( int m = 0; m < 5; m++ ) {
        //  std::cerr<<try_positions[m]<<" ";
        //}
        //cerr<<endl;
        best_score = max(best_score,score+max_score(try_positions, n_stacks, stacks, stack_sizes, optimal_scores));
    }
    /*
    for( int i = 0; i < n_stacks; i++ ) {
        if( current_positions[i] == 0 ) continue; //If it is empty continue

        vector<long> try_positions = current_positions;
        int ncolored = 1;
        color = stacks[i][current_positions[i]];
        try_positions[i]--;

        //Try removing only this, so score 0
        long score;
        score = 0;
        best_score = max(best_score,score+max_score(try_positions, n_stacks, stacks, stack_sizes, optimal_scores));

        for( int j = 0; j < n_stacks; j++ ) { //Choose all the other ones with the same color
            if( j == i ) continue;
            if( current_positions[j] == 0 ) continue;
            if( stacks[j][current_positions[j]] == color ) {
                ncolored++;
                try_positions[j]--;//Try with these two removals
                score = (1<<(ncolored-2)); //Increase in score
                best_score = max(best_score,score+max_score(try_positions, n_stacks, stacks, stack_sizes, optimal_scores));
            }
        }

    }
    */
    optimal_scores[access_index(current_positions, stack_sizes)] = best_score;
    return best_score;
}


void get_chip_score()
{

    int n_stacks;
    cin>>n_stacks;

    vector<long> current_positions(max_stacks,0);//Always of the maximum number for easier memoization
    vector<vector<int>> stacks(max_stacks, vector<int>(1,-1));
    vector<long> stack_sizes(5,1);
    long total_size = 1;
    for( int i = 0; i < n_stacks; i++ ) {
        int n_chips;
        cin>>n_chips;
        stacks[i] = vector<int>(n_chips+1);//Last position to mark that it is empty
        current_positions[i] = n_chips;
        total_size *= (n_chips+1);
        stack_sizes[i] = (n_chips+1);
    }

    for( int i = 0; i < n_stacks; i++ ) {
        stacks[i][0] = -1; //last chip is "invalid color"
        for( int j = 1; j < stacks[i].size(); j++ ) {
            std::cin>>stacks[i][j];
        }
    }
    vector<long> optimal_scores(total_size,-1);
    
    std::cout<<max_score( current_positions, 5/*n_stacks*/ , stacks, stack_sizes, optimal_scores )<<std::endl;
}



int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) {
        get_chip_score();
    }
}
