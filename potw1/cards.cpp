#include <iostream>
#include <vector>
using namespace std;






vector<int> card;


int deck_of_cards(int n, int k)
{
	int i = 0;
	int j = 0;
	
	int mscore = 0;
	int mi = 0;
	int mj = 0;

	bool positive;

	int score = k;

	//Get new card
	cin>>card[0];
	
	//First score
	score = k-card[0];
	positive = (score) > 0;

	//The first smaller numbers
	mscore = abs(score);
	mi = 0;
	mj = 0;

	for( j = 1; j < n; j++ ) {
		cin>>card[j];
		//Now we are sure we have a positive number
		
		score -= card[j];
		positive = (score) > 0;

		if( abs(score) < mscore ) {
			mi = i;
			mj = j;
			mscore = abs(score);
		}

		//If negative we want to remove numbers from the back
		//until positive (move index i forward)
		while( not positive and i != j) {
			score += card[i];//Remove i-th card
			positive = (score) > 0;

			i++;

			if( abs(score) < mscore ) {
				mi = i;
				mj = j;
				mscore = abs(score);
			}
		}
	}
	cout<<mi<<" "<<mj<<endl;
}






int main()
{
	int ntestos;
	cin>>ntestos;
	for( int i = 0; i < ntestos; i++ ) {
		int n,k;
		cin>>n>>k;
		card = vector<int>(n);
		deck_of_cards(n,k);
	}
}
