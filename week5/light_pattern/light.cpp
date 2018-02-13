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
int edit_distance( const int a, const int b, const int nbits, vector<int> &distances )
{
    if( a == b ) return 0;
    if( distances[a] != MAX_INT ) return distances[a];
    int min_distance = MAX_INT;
    for( int i = 0; i < nbits; i++ ) {
        if( (a & (0x01<<i) ) != (b & (0x01<<i) )) min_distance = min(min_distance, edit_distance(a xor (0x01<<i),b,nbits,distances));
    }
    distances[a] = min_distance+1;
    return min_distance+1;
}

void get_pattern()
{
    int nlights, nbits, pattern;
    cin>>nlights>>nbits>>pattern;
    int npatterns = nlights/nbits;

    vector<int> patterns(npatterns,0);
    //distance to the pattern
    vector<int> distances( 0x01<<(nbits+1), MAX_INT);

    for( int i = 0; i < npatterns; i++ ) {
        for( int j = 0; j < nbits; j++ ) {
            int b;
            cin>>b;
            patterns[i] |=  b<<(nbits-j-1);
        }
    }
    int mask = (0x01<<nbits)-1;
    //int total_distance = 0;
    
    bool flipped = false;
    bool igual = false;
    int total_dist = 0;
    for( int i = npatterns-1; i >= 0; i-- ) {
        int dist        = edit_distance(patterns[i],pattern,nbits,distances);
        //int flip_dist = edit_distance(patterns[i] xor mask,pattern,nbits,distances);
        int flipped_dist   = nbits-dist;
        //Equivalent
        //cout<<"--------------------"<<endl;
        //print_binary(patterns[i],nbits);cout<<"->";print_binary(pattern,nbits);cout<<endl;
        //cout<<"flipped: "<<flipped<<endl;
        //cout<<"igual: "<<igual<<endl;
        //cout<<"dist: "<<dist<<" flipped_dist:"<<flipped_dist<<endl;
        //cout<<"total_dist: "<<total_dist<<endl;
        if( igual ) {
            if( dist < flipped_dist) {
                flipped = false;
                igual = false;
                //cout<<"i1"<<endl;
            } else if( dist > flipped_dist ) {
                flipped = true;
                igual = false;
                //cout<<"i2"<<endl;
            } else if( dist == flipped_dist ) {
                igual = true;
                total_dist += dist;
                //cout<<"i3"<<endl;
                continue;
            }

        }
        if( igual ) cout<<"IGUAL, nor right!"<<endl;

        if( not flipped ) {

            if( dist < (flipped_dist+1) ) {
                total_dist += dist;
                flipped = false;
                //cout<<"a1"<<endl;
            } else if( dist > (flipped_dist+1) ) {
                total_dist += flipped_dist+1;
                flipped = true;
                //cout<<"a2"<<endl;
            } else if( dist == (flipped_dist+1) ) {
                total_dist += dist;
                igual = true;
                //cout<<"a3"<<endl;
            }

        } else {//Flipped
            if( (dist+1) < flipped_dist ) {
                total_dist += dist+1;
                flipped = false;
                //cout<<"b1"<<endl;
            } else if( (dist+1) > flipped_dist ) {
                total_dist += flipped_dist;
                flipped = true;
                //cout<<"b2"<<endl;
            } else if( (dist+1) == flipped_dist ) {
                total_dist += dist+1;
                igual = true;
                //cout<<"b3"<<endl;
            }

        }

    }
    cout<<total_dist<<endl;
    //cout<<endl<<endl<<endl;


}


int main()
{
    int ntests;
    cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_pattern();
    }
}
