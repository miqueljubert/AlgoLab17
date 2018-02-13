#include <iostream>
#include <vector>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
using namespace boost;


// Graph Type with nested interior edge properties for Flow Algorithms
typedef     boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef     boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
            boost::property<boost::edge_capacity_t, long,
            boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >   Graph;
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type            EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type   ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type             ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor                       Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor                         Edge;


typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    ReverseEdgeMap  &revedgemap;

    public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
    G(G), capacitymap(capacitymap), revedgemap(revedgemap){}
    //to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
                    capacitymap[e] = capacity;
                    capacitymap[rev_e] = 0; // reverse edge has no capacity!
                    revedgemap[e] = rev_e;
                    revedgemap[rev_e] = e;
    }
};



void get_tournament_results()
{
    int nplayers,nrounds;
    std::cin>>nplayers>>nrounds;
    
    std::vector<std::pair<int,int>> matches(0);
    std::vector<int> scores(nplayers,0);
    int known_score = 0;
    for( int i = 0; i < nrounds; i++ ) {
        int a,b,c;
        std::cin>>a>>b>>c;
        if( c==1 ) {
            scores[a] += 1;
            known_score++;
        } else if( c==2 ) {
            scores[b] += 1;
            known_score++;
        } else {
            matches.push_back(std::pair<int,int>(a,b));
        }

    }
    
    int final_score = 0;
    std::vector<int> scoreboard(nplayers);
    for( int i = 0; i < nplayers; i++ ) {
        std::cin>>scoreboard[i];
        final_score += scoreboard[i];
    }

    // Create Graph and Maps
    Graph G(0);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    
    int nunknowns = matches.size();
    Vertex s = nunknowns+nplayers;
    Vertex t = nunknowns+nplayers+1;

    for( int i = 0; i < nunknowns; i++ ) {
        eaG.addEdge(s,i,1);
    }
    for( int i = 0; i < nunknowns; i++ ) {
        int a,b;
        a = matches[i].first;
        b = matches[i].second;
        eaG.addEdge(i,a+nunknowns,1);
        eaG.addEdge(i,b+nunknowns,1);
    }
    long resulting=0;
    for( int i = 0; i < nplayers; i++ ) {
        //std::cout<<scoreboard[i]-scores[i]<<std::endl;
        if( scoreboard[i]-scores[i] < 0 ) {
            std::cout<<"no"<<std::endl;
            return;
        }
        resulting+= scoreboard[i]-scores[i];
        eaG.addEdge(i+nunknowns,t,scoreboard[i]-scores[i]);
    }
    //long resulting = final_score - known_score; 
    long flow1 = boost::push_relabel_max_flow(G, s, t);
    if( nunknowns != resulting ) {
        std::cout<<"no"<<std::endl;
        return;
    }
    //EdgeIt ie,ee;
    //std::cout<<"residual capacities matches"<<std::endl;
    //for( int i = 0; i < nunknowns; i++ ) {
    //    Edge e;bool b;
    //    tie(e,b) = edge(s,i, G);
    //    std::cout<<b<<":"<<rescapacitymap[e]<<"/"<<capacitymap[e]<<std::endl;
    //}

    //std::cout<<"residual capacities players"<<std::endl;
    //for( int i = 0; i < nplayers; i++ ) {
    //    Edge e;bool b;
    //    tie(e,b) = edge(i+nunknowns, t, G);
    //    std::cout<<b<<":"<<rescapacitymap[e]<<"/"<<capacitymap[e]<<std::endl;
    //}

    //std::cout<<resulting<<" "<<flow1<<std::endl;
    if( resulting == flow1 ) std::cout<<"yes"<<std::endl;
    else std::cout<<"no"<<std::endl;

}


int main()
{
    int ntests;
    std::cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        get_tournament_results();
    }
}
