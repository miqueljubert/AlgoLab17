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
	vector<pair<int,int>> rightmost_positions(nwizards-1);

	int nboats = 1;//Always take the leftmost one
	int current_limit = pow(10,6)+1;//upper limit
	int first_boat = -1;
	
	for( int i = 0; i < nwizards; i++ ) {
		cin>>boat_lengths[i];
		cin>>ring_positions[i];
		if( current_limit > ring_positions[i]) {
			current_limit = ring_positions[i];
			first_boat = i;
		}
	}
	int j = 0;
	for( int i = 0; i < nwizards; i++ ) {
		if( i != first_boat ) {
			rightmost_positions[j] =
				pair<int,int>(ring_positions[i]+boat_lengths[i],i);
			j++;
		}
	}

	sort(rightmost_positions.begin(),rightmost_positions.end(),less<pair<int,int>>());


	//for( int i = 0; i < rightmost_positions.size(); i++ ) {
	//	cout<<rightmost_positions[i].first<<endl;
	//	cout<<rightmost_positions[i].second<<endl<<endl;
	//}
	
	for( int i = 0; i < rightmost_positions.size(); i++ ) {
		int wizard = rightmost_positions[i].second;
		int boat_length = boat_lengths[wizard];
		int ring_position = ring_positions[wizard];

		//cout<<"wizard: "<<wizard<<endl;
		//cout<<"current_limit: "<<current_limit<<endl;
		//cout<<"boat_length: "<<boat_length<<endl;
		//cout<<"ring_position: "<<ring_position<<endl;

		if( current_limit <= ring_position ) {
			nboats++;
			current_limit = max(ring_position,current_limit+boat_length);
		}
		//cout<<"new_limit: "<<current_limit<<endl;
		//cout<<endl;
	
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
