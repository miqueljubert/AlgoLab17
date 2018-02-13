#include <iostream>
#include <vector>
#include <utility>
#include <limits>
using namespace std;

const int MAX_INT =  numeric_limits<int>::max();



void print_binary( int num, int k )
{
    for( int i = 0; i < k; i++ ) {
        int p = num & (0x01<<(k-1));
        cout<<(p>>(k-1));
        num = num<<1;
    }
}


//Get minimum amount of time required to turn integer a into integer b
//Return true if an odd number of "left-bit-flips" was required
int edit_distance( const int a, const int b, const int nbits, vector<vector<int>> &distances )
{
    if( a == b ) return 0;
    if( distances[a][b] != MAX_INT ) return distances[a][b];
    int min_distance = MAX_INT;
    for( int i = 0; i < nbits; i++ ) {
        //cout<<"nbits:"<<nbits<<" i:"<<i<<" a:"<<a<<" b:"<<b<<" nbits:"<<nbits<<endl;
        //print_binary(a,nbits);
        //cout<<"->";
        //print_binary(a xor 1,nbits);
        //cout<<endl;
        if( (a & (0x01<<i) ) != (b & (0x01<<i) )) min_distance = min(min_distance, edit_distance(a xor (0x01<<i),b,nbits,distances));
    }
    distances[a][b] = min_distance+1;
    return min_distance+1;
}

//int best_distance( int current, int pattern, int nbits, vector<int> &patterns, vector<vector<int>> &distances, vector<int> &best_dist )
//{
//    if( current >= patterns.size() ) return 0;
//    cout<<current<<"-"<<patterns.size()<<endl;
//    if( best_dist[current] < MAX_INT ) return best_dist[current];
//
//    int distance = edit_distance(patterns[current],pattern,nbits,distances);
//    int flipped_distance = nbits-distance;
//
//    int mask = (0x01 << (nbits)) - 1;
//    //print_binary(mask,nbits);cout<<endl;
//
//      
//
//    int opt = min( distance + best_distance(current+1,pattern,nbits,patterns,distances, best_dist),
//                flipped_distance + 1 + best_distance(current+1,pattern xor mask, nbits, patterns, distances, best_dist));
//    best_dist[current] = opt;
//    return opt;
//    
//}


int best_distance( int start, int pattern, int nbits, vector<int> &patterns, vector<vector<int>> &distances, vector<int> &best_dist )
{
    int mask = (0x01<<(nbits))-1;
    int total_dist = 0;
    for( int i = start; i < patterns.size(); i++ ) {
        int current = patterns.size()-1-i;

        int distance = edit_distance(patterns[current],pattern,nbits,distances);
        int flipped_distance = nbits-distance;

        if( flipped_distance < distance ) {
            pattern = pattern xor mask;
            total_dist += 1 + flipped_distance;

        } else {
            total_dist += distance;
        }
    }
    return total_dist;
}


void get_pattern()
{
    int nlights, nbits, pattern;
    cin>>nlights>>nbits>>pattern;
    int npatterns = nlights/nbits;

    vector<int> patterns(npatterns,0);

    vector<vector<int>> distances( 0x01<<(nbits+1), vector<int>(0x01<<(nbits+1), MAX_INT));

    vector<int> best_dist(npatterns,MAX_INT);

    for( int i = 0; i < npatterns; i++ ) {
        for( int j = 0; j < nbits; j++ ) {
            int b;
            cin>>b;
            patterns[i] |=  b<<(nbits-j-1);
        }
    }

    //for( int i = 0; i < npatterns; i++ ) {
        //print_binary(patterns[i],nbits);
    //}
    bool flipped = false;
    bool igual;
    int total_distance = 0;
    //for( int i = npatterns-1; i >= 0; i-- ) {
    //    //cout<<i<<" "<<npatterns<<" "<<patterns.size()<<" "<<patterns[i]<<endl;
    //    int distance = edit_distance(patterns[i],pattern,nbits,distances);
    //    int flipped_distance = nbits-distance;
    //    
    //}
    cout<<best_distance(0,pattern,nbits,patterns,distances,best_dist)<<endl;



}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_pattern();
    }
}
