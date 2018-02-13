#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;








void compute_age()
{
	int nspecies,nqueries;
	cin>>nspecies>>nqueries;

	vector<pair<string,int>> info_species(nspecies);
	unordered_map<string,int> species_key;
	//vector<vector<int>> descendants(nspecies,vector<int>(0));
	vector<int> antecessor(nspecies,-1);

	
	//Read species
	for( int i = 0; i < nspecies; i++ ) {
		string name;
		int age;
		cin>>name>>age;

		info_species[i] = pair<string,int>(name,age);
		species_key[name] = i;
	}


	for( int i = 0; i < nspecies-1; i++ ) {
		string name1,name2;
		cin>>name1>>name2;

		int key1,key2;
		key1 = species_key[name1];
		key2 = species_key[name2];

		//descendants[key2].push_back(key1);
		antecessor[key1] = key2;
	}
	vector<string> query_names(nqueries);
	vector<int> query_ages(nqueries);

	for( int i = 0; i < nqueries; i++ ) {
		cin>>query_names[i]>>query_ages[i];
	}

	//Time do da queries
	for( int i = 0; i < nqueries; i++ ) {
		string name;
		int target_age;
		//cin>>name>>target_age;
		name = query_names[i];
		target_age = query_ages[i];

		
		int current_species = species_key[name];
		int current_age = info_species[current_species].second;

		while( true ) {
			//Go up the tree
			if( antecessor[current_species] == -1 ) break;
			int next_species = antecessor[current_species];
			int next_age = info_species[next_species].second;

			if( next_age > target_age ) break;
			current_species = next_species;
			current_age = next_age;

		}

		cout<<info_species[current_species].first<<" ";

	}

	cout<<endl;



}









int main()
{
	std::ios_base::sync_with_stdio(false);
	int ntests;
	cin>>ntests;
	for( int i = 0; i < ntests; i++ ) {
		compute_age();
	}

}
