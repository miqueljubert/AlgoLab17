#include <queue>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;


void time_to_move()
{
	//weight, friend
	//priority_queue<pair<int,int>,vector<pair<int,int>>,less<pair<int,int>>> smallest;
	
	int nfriends,nboxes;
	cin>>nfriends>>nboxes;
	
	vector<int> strengths(nfriends);
	//vector<pair<int,int>> boxes(nboxes);
	vector<int> boxes(nboxes);

	vector<int> friend_loads(nfriends,0);

	for( int i = 0; i < nfriends; i++ ) {
		cin>>strengths[i];
	}

	for( int i = 0; i < nboxes; i++ ) {
		int weight;
		cin>>weight;

		//Box weight, index number
		//boxes[i] = pair<int,int>(weight,i);
		boxes[i] = weight,i;


	}
	
	sort( boxes.begin(), boxes.end(), greater<int>() );
	sort( strengths.begin(), strengths.end(), greater<int>() );
	
	
	//for( int i = 0; i < nboxes; i++ ) {
		//cout<<boxes[i].first<<" "<<boxes[i].second<<endl;
		//cout<<boxes[i]<<endl;
	//}
	if( boxes[0] > strengths[0] ) {//The strongest friend
		cout<<"impossible"<<endl;
		return;
	}
	
	int chosen = 0;
	int largest_load = -1;
	for( int i = 0; i < nboxes; i++ ) {

		int box_weight = boxes[i];
		for( int j = chosen; j < nfriends; j++ ) {
			//He cannot carry it, it is a task for the first one
			if( strengths[j] < box_weight ) {
				chosen = 0;
				break;
			}
			
			//He is strong enough and not as busy he is the chosen
			if( largest_load > friend_loads[j] ) {
				chosen = j;
				break;
			}

			if( j == (nfriends-1) ) {
				chosen = 0;
				break;
			}
			
		}
		friend_loads[chosen]++;
		largest_load = max(largest_load,friend_loads[chosen]);
	}

	int workload = friend_loads[0];
	for( int i = 0; i < friend_loads.size(); i++ ) {
		workload = max(workload,friend_loads[i]);
	}

	cout<<workload*3-1<<endl;



}


int main()
{
	std::ios_base::sync_with_stdio(false);
	int ntests;
	cin>>ntests;
	for( int i = 0; i < ntests; i++ ) {
		time_to_move();
	}
}
