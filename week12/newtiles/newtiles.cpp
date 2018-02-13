#include <iostream>
#include <vector>
using namespace std;


void bit_print(int n, int bits)
{
    for( int i = 0; i < bits; i++ ) {
        std::cout<<((bool) (n & (0x01<<(bits-i-1))));
    }
    cout<<endl;
}

int max_tiles( int row, int h, int w, int mask, const vector<int> &states, vector<vector<int>> &table )
{
    if( row == h-1 ) return 0;

    int row_state = states[row] & ~mask;
    int n_row_state = states[row+1];

    if( table[row][row_state] != -1 ) return table[row][row_state];


    vector<int> valid_indices;
    for( int i = 0; i < w-1; i++ ) {
        bool b1 = (bool)(row_state   & (0x01<<(w-i-1)));
        bool b2 = (bool)(row_state   & (0x01<<(w-i-2)));
        bool b3 = (bool)(n_row_state & (0x01<<(w-i-1)));
        bool b4 = (bool)(n_row_state & (0x01<<(w-i-2)));

        bool can_place = b1 and b2 and b3 and b4;

        if( can_place ) valid_indices.push_back(i);
    }
    
    int max_n = 0;
    for( int i = 0; i < (0x01<<valid_indices.size()); i++ ) {
        int n_mask = 0;
        bool next = false;
        int count = 0;
        for( int j = 0; (not next) and j < valid_indices.size(); j++ ) {
            if( i & (0x01<<j) ) {
                int index = valid_indices[j];
                bool b1 = n_mask & (0x01<<(w-index-1));
                bool b2 = n_mask & (0x01<<(w-index-2));
                if( (not b1) and (not b2) ) {//Check that there is no conflict with another of our tiles
                    count++;
                    n_mask = n_mask | (0x01<<(w-index-1));
                    n_mask = n_mask | (0x01<<(w-index-2));
                } else { //If there is a conflict this permutation is invalid
                   next = true;
                }
            }
        }

        if( not next ) max_n = max(max_n, max_tiles(row+1,h,w,n_mask,states,table)+count);
    }
    return table[row][row_state] = max_n;


}



void run()
{
    int h,w;
    cin>>h>>w;

    vector<int> states(h,0);
    for( int i = 0; i < h; i++ ) {
        for( int j = 0; j < w; j++ ) {
            bool b;
            cin>>b;
            if( b ) states[i] |= 0x01<<(w-j-1);
        }
    }
    //for( int i = 0; i < h; i++ ) {
    //    for( int j = 0; j < w; j++ ) {
    //        cout<<(bool)(states[i] & (0x01<<(w-j-1)))<<" ";
    //    }
    //    cout<<endl;
    //}
    vector<vector<int>> table(h,vector<int>(0x01<<w,-1));
    std::cout<<max_tiles(0,h,w,0,states,table)<<std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) run();

}
