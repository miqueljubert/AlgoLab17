#include <iostream>
#include <vector>
#include <queue>
#include <utility>
using namespace std;

int search_snippets()
{
	int nwords;
	cin>>nwords;

	vector<vector<int>> words(nwords,vector<int>());
	vector<int> current_position(nwords,1);

	priority_queue<pair<int,int>,vector<pair<int,int>>, std::greater<pair<int,int>>>
		next_element;
	//next_element.push(pair<int,int>(1,3));
	//next_element.push(pair<int,int>(2,2));
	//next_element.push(pair<int,int>(3,1));

	//cout<<next_element.top().first<<endl;
	//next_element.pop();
	//cout<<next_element.top().first<<endl;
	//next_element.pop();
	//cout<<next_element.top().first<<endl;
	//next_element.pop();

	vector<int> word_num(nwords);
	vector<int> segment_right(nwords);
	//std::cout<<nwords<<std::endl;
	for( int i = 0; i < nwords; i++ ) {

		int awords;
		cin>>awords;
		
		word_num[i] = awords;
		words[i] = vector<int>(awords);
	}

	for( int i = 0; i < nwords; i++ ) {
		for( int j = 0; j < word_num[i]; j++ ) {
			int word;
			cin>>words[i][j];
		}
	}

	int minlength,start,end,start_word;
	start = words[0][0];
	start_word = 0;
	end = words[0][0];
	//initialization
	for(int i = 0; i < nwords; i++) {
		int front_word = words[i][0];
		segment_right[i] = front_word;

		if( front_word < start ) {
			start = front_word;
			start_word = i;
		}
		end   = max(end,front_word);

	}
	minlength = end-start;

	while( true ) {

		//If the queue is empty we will only make it longer
		if( current_position[start_word] == words[start_word].size() ) {
			return minlength+1;
		}

		int front_word = words[start_word][current_position[start_word]];
		current_position[start_word]++;

		segment_right[start_word] = front_word;
		
		//Process the chosen element
		//words[nextword].pop();
		//segment_right[nextword] = nextval;

		start = front_word;
		//end = front_word;
		end  = max(end, front_word);
		
		for( int i = 0; i < nwords; i++ ) {
			//start = min(start, segment_right[i]);
			if( segment_right[i] < start ) {
				start = segment_right[i];
				start_word = i;
			}
			//end  = max(end, segment_right[i]);
		}
		
		minlength = min(minlength, end-start);

	}
		






}

int main()
{
	std::ios_base::sync_with_stdio(false);
	int ntests;
	cin>>ntests;
	for( int i = 0; i < ntests; i++ ) {
		cout<<search_snippets()<<endl;
	}
}

