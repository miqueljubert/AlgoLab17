#include <iostream>
#include <vector>
using namespace std;


int compute_matrix( int n )
{
	vector<vector<int>> matrix(n,vector<int>(n,0));

	for( int i = 0; i < n; i++ ) {
		for( int j = 0; j < n; j++ ) {
			cin>>matrix[i][j];
		}
	}

	vector<vector<int>> partial_sum(n,vector<int>(n,0));

	for( int i = 0; i < n; i++ ) {
		int row_sum = 0;

		for( int j = 0; j < n; j++ ) {

			row_sum += matrix[i][j];

			int full_sum;
		        if( i > 0 )
				full_sum = (row_sum+partial_sum[i-1][j]);
			else
				full_sum = row_sum;


			partial_sum[i][j] = full_sum;
		}

	}
	
	//cout<<"evens:"<<evens<<endl;
	//cout<<"odds:"<<odds<<endl;
	int evens = 0;
	int odds = 0;

	for( int i1 = 0; i1 < n; i1++ ) {
		for( int j1 = 0; j1 < n; j1++ ) {
			for( int i2 = 0; i2 <= i1; i2++ ) {
				for( int j2 = 0; j2 <= j1; j2++ ) {
					int parity = 0;
					if()
						partial_sum[i2][j2];
						partial_sum[i1-1][j2];
						partial_sum[i2][j1-1];
						partial_sum[i1-1][j1-1];
					if( (parity%2) == 0 ) {
						evens++;
					}
				}
			}
		}
	}
	return evens;
	

}

int main()
{
	int ntests;
	cin>>ntests;
	for( int i = 0; i < ntests; i++ ) {
		int n;
		cin>>n;
		cout<<compute_matrix(n)<<endl;
	}
}
