#include <iostream>
#include <vector>
using namespace std;


int max_tiles( int row, int column, int h, int w, int prev_mask, int mask, const vector<int> &states, vector<vector<int>> &table )
{
    if( row == h ) return 0;
    if( column >= w-1 ) return max_tiles( row+1, 0, h, w, mask, 0, states, table );

    int row_state = states[row] & ~prev_mask & ~mask;

    if( table[row][row_state] != -1 ) return table[row][row_state];

    int n_row_state = states[row+1] & ~mask;
    bool b1 = (bool)(row_state   & (0x01<<(w-column-1)));
    bool b2 = (bool)(row_state   & (0x01<<(w-column-2)));
    bool b3 = (bool)(n_row_state & (0x01<<(w-column-1)));
    bool b4 = (bool)(n_row_state & (0x01<<(w-column-2)));

    bool can_place = b1 and b2 and b3 and b4;

    if( not can_place ) return max_tiles( row, column+1, h, w, prev_mask, mask, states, table );

    //If we can place
    int n_mask = mask   | (0x01<<(w-column-1));
        n_mask = n_mask | (0x01<<(w-column-2));

    return table[row][row_state] = std::max(
                    max_tiles(row, column+2, h, w, prev_mask, n_mask, states, table)+1,
                    max_tiles(row, column+1, h, w, prev_mask, mask, states, table) 
                );
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
    std::cout<<max_tiles(1,1,h,w,0,0,states,table)<<std::endl;
}

int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) run();

}
