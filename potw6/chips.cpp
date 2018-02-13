#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
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
    
    unordered_map<int,int> top_colors;
    unordered_map<int,vector<int>> color_locations;
    for( int i = 0; i < n_stacks; i++ ) {
        if( current_positions[i] != 0 ) {
            int color = stacks[i][current_positions[i]];
            if( top_colors.find(color) == top_colors.end() ) {
                top_colors[color] = 1;
                color_locations[color].push_back(i);
            } else {
                top_colors[color]++;
                color_locations[color].push_back(i);
            }
        }
    }

    long best_score = -1;
    for( auto e = top_colors.begin(); e != top_colors.end(); ++e ) {
        int color = e->first;
        int color_amount = e->second;
        vector<int> locations = color_locations[color];
        for( int i = 1; i < (0x01<<locations.size()); i++ ) {
            vector<long> try_positions = current_positions;
            int n_removed = 0;
            for( int j = 0; j < locations.size(); j++ ) {
                if( i & (0x01<<j) ) {
                    try_positions[locations[j]]--;
                    n_removed++;
                }
            }
            long score = 0;
            if( n_removed > 1 ) {
                score = (1<<(n_removed-2));
            }

            //std::cout<<"coses: ";
            //for( int i = 0; i < try_positions.size(); i++ ) std::cout<<try_positions[i];
            //std::cout<<endl;

            best_score = max(best_score, score + max_score( try_positions, n_stacks, stacks, stack_sizes, optimal_scores ) );
            //std::cout<<"best_score:"<<best_score<<std::endl;
        }
    }


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
