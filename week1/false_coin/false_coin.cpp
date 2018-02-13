#include <vector>
#include <iostream>
using namespace std;




int get_bad_coin( int n, int k)
{
	vector<char> coins(n,'u');


	int uweights = 0;
	vector<int> rweights(n,0);
	

	vector<int> left_head(1000);
	vector<int> right_head(1000);

	for( int i = 0; i < k; i++ ) {
		int ncoins;
		cin>>ncoins;//(per side)

		for( int j = 0; j < ncoins; j++ ) {
			cin>>left_head[j];
			left_head[j]--;
		}
		for( int j = 0; j < ncoins; j++ ) {
			cin>>right_head[j];
			right_head[j]--;
		}
		
		char result;
		cin>>result;

		if(result == '>') {
			for( int j = 0; j < ncoins; j++ ) {
				int coin = left_head[j];
				rweights[coin]++;
				switch(coins[coin]){
					case 'u':
						coins[coin] = '>';
						break;
					case '<':
						coins[coin] = '=';
						break;
					case '>':
						coins[coin] = '>';
						break;
					case '=':
						coins[coin] = '=';
						break;
				}
			}
			for( int j = 0; j < ncoins; j++ ) {
				int coin = right_head[j];
				rweights[coin]++;
				switch(coins[coin]){
					case 'u':
						coins[coin] = '<';
						break;
					case '<':
						coins[coin] = '<';
						break;
					case '>':
						coins[coin] = '=';
						break;
					case '=':
						coins[coin] = '=';
						break;
				}
			}
			uweights++;
		} else if(result == '<') {
			for( int j = 0; j < ncoins; j++ ) {
				int coin = left_head[j];
				rweights[coin]++;
				switch(coins[coin]){
					case 'u':
						coins[coin] = '<';
						break;
					case '<':
						coins[coin] = '<';
						break;
					case '>':
						coins[coin] = '=';
						break;
					case '=':
						coins[coin] = '=';
						break;
				}
			}
			for( int j = 0; j < ncoins; j++ ) {
				int coin = right_head[j];
				rweights[coin]++;
				switch(coins[coin]){
					case 'u':
						coins[coin] = '>';
						break;
					case '<':
						coins[coin] = '=';
						break;
					case '>':
						coins[coin] = '>';
						break;
					case '=':
						coins[coin] = '=';
						break;
				}
			}
			uweights++;
		} else if(result == '=') {
			for( int j = 0; j < ncoins; j++ ) {
				int coin = left_head[j];
				switch(coins[coin]){
					case 'u':
						coins[coin] = '=';
						break;
					case '<':
						coins[coin] = '=';
						break;
					case '>':
						coins[coin] = '=';
						break;
					case '=':
						coins[coin] = '=';
						break;
				}
			}
			for( int j = 0; j < ncoins; j++ ) {
				int coin = right_head[j];
				switch(coins[coin]){
					case 'u':
						coins[coin] = '=';
						break;
					case '<':
						coins[coin] = '=';
						break;
					case '>':
						coins[coin] = '=';
						break;
					case '=':
						coins[coin] = '=';
						break;
				}
			}
		}
	}
	
	//cout<<n<<"-"<<k<<endl;
	int resultat = -1;
	for(int i = 0; i < n; i++) {
		if(coins[i] != '=' and rweights[i] == uweights){
			//cout<<i<<','<<coins[i]<<','<<rweights[i]<<' ';
			if( resultat == -1 ) {
				resultat = i;
			} else {
				resultat = -1;
				break;
			}
		}
	}
	cout<<resultat+1<<endl;
}



int main()
{
	int ntests;
	cin>>ntests;
	for( int i = 0; i < ntests; i++ ) {
		int n,k;
		cin>>n>>k;
		get_bad_coin(n,k);
	}
}
