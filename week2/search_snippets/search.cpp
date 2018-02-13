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

	vector<int> word_num(nwords);
	vector<int> segment_right(nwords);
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
	end = words[0][0];
	//initialization
	for(int i = 0; i < nwords; i++) {
		int front_word = words[i][0];
		segment_right[i] = front_word;
		
		next_element.push(pair<int,int>(front_word,i));

		end   = max(end,front_word);

	}

	start = next_element.top().first;
	start_word = next_element.top().second;

	minlength = end-start;

	while( true ) {

		//If the queue is empty we will only make it longer
		if( current_position[start_word] == words[start_word].size() ) {
			return minlength+1;
		}

		int front_word = words[start_word][current_position[start_word]];
		next_element.pop();
		next_element.push(pair<int,int>(front_word,start_word));

		current_position[start_word]++;

		segment_right[start_word] = front_word;
		

		end  = max(end, front_word);
		start = next_element.top().first;
		start_word = next_element.top().second;
		
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

