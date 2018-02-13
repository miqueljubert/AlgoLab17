#include <iostream>
#include <vector>
using namespace std;

//Greedy algorithm;
int process_domines( int ndominoes ) {
	int fallen = 1;
	int knock_down_distance;//How many positions 'forward' will be knocked
	bool falling = true;

	cin>>knock_down_distance;
	if(knock_down_distance == 1) falling = false;
	
	int domino_height;
	for( int i = 1; i < ndominoes; i++ ) {
		if (falling) fallen++;
		cin>>domino_height;
		knock_down_distance = max(knock_down_distance-1,domino_height);

		if( knock_down_distance == 1 ) {
			falling = false;
		}
		
	}
	return fallen;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int ntests;
	while( cin>>ntests ) {
		int ndominoes;
		for( int i = 0; i < ntests; i++ ) {
			cin>>ndominoes;
			cout<<process_domines(ndominoes)<<std::endl;
		}
	}
}
