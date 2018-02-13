#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

using namespace std;
using namespace boost;


class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

const int MAX_INT = numeric_limits<int>::max();

vector<int> offset_i = {-1,-1,+1,+1,-2,-2,+2,+2};
vector<int> offset_j = {-2,+2,-2,+2,-1,+1,-1,+1};


void run()
{	
    int n;
    cin>>n;

    int N = n*n;
    int src = N;
    int tgt = N+1;
    Graph G(N+2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    vector<vector<bool>> table(n,vector<bool>(n,false));
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ ) {
            int val;
            cin>>val;
            if( val == 1 ) {
                table[i][j] = true;
            }
        }
    }
    
    
    vector<vector<bool>> table_side(n,vector<bool>(n,false));
    bool black = true;
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ ) {
            table_side[i][j] = black;
            if( not table[i][j] ) {
                black = not black;
                continue;
            }
            if( black ) eaG.addEdge(src,i*n+j,1);
            if( not black ) eaG.addEdge(i*n+j,tgt,1);

            if( not black ) {
                black = not black;
                continue;
            }
            for( int k = 0; k < 8; k++ ) {
                int ii = i + offset_i[k];
                int jj = j + offset_j[k];
                if( ii < 0 or ii >= n or jj < 0 or jj >= n ) continue;
                if( not table[ii][jj] ) continue;

                eaG.addEdge(i*n+j,ii*n+jj,MAX_INT);
            }
            black = not black;
        }
        if( (n%2) == 0 ) black = not black;
    }

	int flow = boost::push_relabel_max_flow(G, src, tgt);

	std::vector<int> vis(N, false);
	std::queue<int> Q;
	vis[src] = true;
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

    int count = 0;
    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ ) {
            if( not table[i][j] ) continue;
            if( table_side[i][j] and vis[i*n+j] ) {
                count++;
            }
            if( not table_side[i][j] and not vis[i*n+j] ) {
                count++;
            }
        }
    }
    cout<<count<<endl;
}


int main() {
    int t;
    cin>>t;
    for( int i = 0; i < t; i++ ) run();
}

