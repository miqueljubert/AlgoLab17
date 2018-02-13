#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <functional>
#include <algorithm>
using namespace std;








void compute_age()
{
	int nspecies,nqueries;
	cin>>nspecies>>nqueries;

	vector<string> name_species(nspecies);
	vector<int> age_species(nspecies);

	unordered_map<string,int> species_key;

	//First id of query, second age of query
	vector<vector<int>> descendants(nspecies,vector<int>(0));
	vector<int> antecessor(nspecies,-1);

	
	//Read species
	for( int i = 0; i < nspecies; i++ ) {
		string name;
		int age;
		cin>>name>>age;

		name_species[i] = name;
		age_species[i] = age;

		species_key[name] = i;
	}


	for( int i = 0; i < nspecies-1; i++ ) {
		string name1,name2;
		cin>>name1>>name2;

		int key1,key2;
		key1 = species_key[name1];
		key2 = species_key[name2];

		descendants[key2].push_back(key1);
		antecessor[key1] = key2;
	}



	vector<string> query_names(nqueries);
	vector<int> query_ages(nqueries);

	vector<int> query_answer(nqueries,-1);
	vector<vector<pair<int,int>>> species_queries(nqueries);

	for( int i = 0; i < nqueries; i++ ) {
		cin>>query_names[i]>>query_ages[i];

		species_queries[species_key[query_names[i]]]
			.push_back(pair<int,int>(i,query_ages[i]));
	}
	//Sort the queries for each leaf
	for( int i = 0; i < nqueries; i++ ) {
		sort( species_queries[i].begin(),species_queries[i].end(),
				less<pair<int,int>>() );
	}
	

	for( int i = 0; i < nspecies; i++ ) {
		int current_node = i;
			
		int j = 0; //Current query being solved
		//Recorrem cap enrere l'arbre!
		while( current_query < species_queries[j].size()
				and antecessor[current_node] != -1 ) {

			next_node = antecessor[current_query];
			
			while(  < query_ages[j] )

		}
	}





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
