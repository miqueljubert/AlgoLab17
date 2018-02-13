#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;










int compute_boats()
{
	int nwizards;
	cin>>nwizards;
	vector<int> boat_lengths(nwizards);
	vector<int> ring_positions(nwizards);
	vector<pair<int,int>> rightmost_positions(nwizards);

	int nboats = 1;//Always take the leftmost one
	int current_limit = pow(10,9)+1;//upper limit
	int first_boat = -1;
	
	for( int i = 0; i < nwizards; i++ ) {
		cin>>boat_lengths[i];
		cin>>ring_positions[i];
        rightmost_positions[i] = pair<int,int>(ring_positions[i],i);
	}


	sort(rightmost_positions.begin(),rightmost_positions.end(),less<pair<int,int>>());


    first_boat = rightmost_positions[0].second;
    current_limit = rightmost_positions[0].first;
    
    for( int i = 1; i < nwizards; i++ ) {
        int current_wizard = rightmost_positions[i].second;
        int current_ring   = rightmost_positions[i].first;
        if( current_ring < current_limit ) {
            //cannot be considered
            continue;
        }
        
        //If we are dealing with the last boat
        if( i == nwizards-1 ) {
            nboats++;
            break;
        }
        int potential_wizard = i;
        int potential_limit = max(current_limit+boat_lengths[current_wizard], current_ring);

        int j = i+1;
        int next_wizard = rightmost_positions[j].second;
        int next_ring   = rightmost_positions[j].first;
        int next_limit = max(current_limit+boat_lengths[next_wizard], next_ring);

        //Mirem si un dels barcos bloquejats es una millor opcio
        int better = -1;
        while( next_ring <= potential_limit and j < nwizards) {
            //Es una opcio millor
            if( next_limit < potential_limit ) {
                better = j;
                break;
            }
            j++;
            next_wizard = rightmost_positions[j].second;
            next_ring   = rightmost_positions[j].first;
            next_limit = max(current_limit+boat_lengths[next_wizard], next_ring);
        }
        //Current boat is good
        if( better == -1 ) {
            nboats++;
            current_limit = potential_limit;
        } else {
            i = better-1;//Consider the next boat
        }
        
    }

	return nboats;
}









int main()
{
	std::ios_base::sync_with_stdio(false);
	int ntests;
	cin>>ntests;
	for( int i = 0; i < ntests; i++ ) {
		cout<<compute_boats()<<endl;	
	}

}
