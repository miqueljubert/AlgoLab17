#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int search_snippets()
{
	int nwords;
	cin>>nwords;

	vector<queue<int>> words(nwords,queue<int>());
	vector<int> word_num(nwords);
	vector<int> segment_right(nwords);
	//std::cout<<nwords<<std::endl;
	for( int i = 0; i < nwords; i++ ) {

		int awords;
		cin>>awords;
		
		word_num[i] = awords;
	}

	for( int i = 0; i < nwords; i++ ) {
		for( int j = 0; j < word_num[i]; j++ ) {
			int word;
			cin>>word;
			words[i].push(word);
		}
	}

	int minlength,start,end,start_word;
	start = words[0].front();
	start_word = 0;
	end = words[0].front();
	//initialization
	for(int i = 0; i < nwords; i++) {
		int front_word = words[i].front();
		words[i].pop();
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
		if( words[start_word].empty()) {
			return minlength+1;
		}

		int front_word = words[start_word].front();
		words[start_word].pop();

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

