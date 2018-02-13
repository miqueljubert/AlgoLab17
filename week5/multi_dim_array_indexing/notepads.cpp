#include <iostream>
#include <vector>
#include <utility>
using namespace std;






void notepad_it()
{
    int nparams, nexperiments, nqueries;
    cin>>nparams>>nexperiments>>nqueries;

    vector<pair<int,int>> bounds(nparams);
    vector<int> length(nparams);
    
    //Read bounds on parameters
    int notepad_size = 1;
    for( int i = 0; i < nparams; i++ ) {
        cin>>bounds[i].first;
        cin>>bounds[i].second;
        length[i] = bounds[i].second - bounds[i].first;
        notepad_size *= length[i];
    }
    
    //cout<<notepad_size<<endl;
    //Build the large matrix as a vector
    vector<int> notepad(notepad_size,-1);


    //Experiments
    vector<vector<int>> experiment_params(nexperiments,vector<int>(nparams));
    vector<int> experiment_result(nexperiments);

    for( int i = 0; i < nexperiments; i++ ) {
        int access_index = 0;
        int current_length = notepad_size;
        for( int j = 0; j < nparams; j++ ) {
            cin>>experiment_params[i][j];
            experiment_params[i][j] -= bounds[j].first;
            
            current_length /= length[j];
            access_index += current_length*experiment_params[i][j];
            //cout<<(access_index < notepad_size);

        }
        cin>>experiment_result[i];
        notepad[access_index] = experiment_result[i];
    }


    //Queries
    vector<vector<int>> query(nqueries,vector<int>(nparams));
    for( int i = 0; i < nqueries; i++ ) {
        int access_index = 0;
        int current_length = notepad_size;
        for( int j = 0; j < nparams; j++ ) {
            cin>>query[i][j];

            current_length /= length[j];
            access_index += current_length*query[i][j];
        }
        cout<<notepad[access_index]<<endl;
    }


}




int main()
{
    int ntest;
    cin>>ntest;
    for( int i = 0; i < ntest; i++ ) {
        notepad_it();
    }
}
