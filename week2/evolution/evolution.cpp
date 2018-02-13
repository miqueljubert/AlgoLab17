#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
using namespace std;


typedef vector<vector<int>> Tree;
typedef pair<string,int> query;



//int binary_search( vector<int> &elements, vector<int>



void query_dfs( int current_node,
                vector<int> &predecessors,
                const Tree &tree,
                const vector<int> &age,
                const vector<query> &queries,
                vector<string> &query_answer,
                unordered_map<string,int> &name_index,
                unordered_map<int,string> &index_name,
                unordered_map<int,vector<int>> &name_queries
              )
{
    //std::cout<<index_name[current_node]<<std::endl;
    predecessors.push_back(current_node);
    for( int i = 0; i < name_queries[current_node].size(); i++ ) {
        int q = name_queries[current_node][i];
        int target = queries[q].second;
        /*
        for( int j = predecessors.size()-1; j >= 0; j-- ) {
            if( age[predecessors[j]] > target ) {
                query_answer[q] = index_name[predecessors[j+1]];
                break;
            }
        }
        if( not assigned ) query_answer[q] = index_name[predecessors[0]];
        */
        int start = 0;
        int end = predecessors.size()-1;
        int valid = 0;
        while( start <= end ) {
            int current = (start+end)/2;
            if( age[predecessors[current]] <= target ) {
                end = current-1;
                valid = current;
            } else {
                start = current+1;
            }

        }
        query_answer[q] = index_name[predecessors[valid]];

    }


    for( int i = 0; i < tree[current_node].size(); i++ ) {
        int node = tree[current_node][i];
        query_dfs(node,predecessors,tree,age,queries,query_answer,name_index,index_name,name_queries);
    }
    predecessors.pop_back();
}


void run()
{
    int n,q;
    cin>>n>>q;

    vector<string> names(n);
    vector<int> age(n);

    Tree tree(n);

    int root = -1;
    int root_age = -1;
    unordered_map<string,int> name_index;
    unordered_map<int,string> index_name;
    for( int i = 0; i < n; i++ ) {
        cin>>names[i];
        cin>>age[i];
        name_index[names[i]] = i;
        index_name[i] = names[i];
        if( age[i] > root_age ) {
            root_age = age[i];
            root = i;
        }
        //tree[i] = vector<int>();
    }

    for( int i = 0; i < n-1; i++ ) {
        string a1,a2;
        cin>>a1>>a2;
        tree[name_index[a2]].push_back(name_index[a1]);
    }

    unordered_map<int,vector<int>> name_queries;
    vector<query> queries(q);
    for( int i = 0; i < q; i++ ) {
        string s;
        int a;
        cin>>s>>a;
        queries[i] = query(s,a);

        name_queries[name_index[s]].push_back(i);
    }

    //for( auto it = name_queries.begin(); it != name_queries.end(); ++it ) {
    //    sort(it->second.begin(),it->second.end(),[&age] ( const int i1, const int i2 ) {
    //                return age[i1] < age[i2];
    //            });
    //    for( int i = 0; i < it->second.size(); i++ ) std::cout<<age[it->second[i]]<<std::endl; std::cout<<endl;
    //}


    vector<string> query_answer(q);
    vector<int> p;
    query_dfs(root, p, tree,
            age,
            queries,
            query_answer,
            name_index,
            index_name,
            name_queries );

    for( int i = 0; i < q; i++ ) {
        std::cout<<query_answer[i]<<" ";
    }
    std::cout<<endl;

}


int main()
{
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}

