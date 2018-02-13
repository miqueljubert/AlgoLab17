#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<int> deck_of_cards(int n, int k){
    
    vector<int> deck(n);
    for(int i=0; i<n; i++){
        cin>>deck[i];
    }
    
    vector<int> result(2,0);
    int S=deck[0];
    int best=abs(k-S);
    int i=0,j=0;
    bool not_found = true;
    while(not_found){
        if(best==0) return result;
        if(i==j){
            j++;
            S += deck[j];
        }
        if(i<j and k<=S){
            S -= deck[i];
            i++;
            if(abs(k-S)<best){
                best = abs(k-S);
                result[0] = i;
                result[1] = j;
            }
        }
        if(i<j and k>=S){
            j++;
            S += deck[j];
            if(abs(k-S)<best){
                best = abs(k-S);
                result[0] = i;
                result[1] = j;
            }
        }
    if(i==n or j==n+1) break;
    }
    return result;
}

int main(){
    int n_cases;
    cin>>n_cases;
    for(int i=0; i<n_cases; i++){
            int n,k;
            cin>>n>>k;
            vector<int> result(2); 
            result = deck_of_cards(n,k);
            cout<<result[0]<<" "<<result[1]<<endl;
    }
}