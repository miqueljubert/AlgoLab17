#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
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



void play_tetris()
{
    int w,n;
    std::cin>>w>>n;
    std::vector<std::pair<int,int>> bricks(0);
    int sure_length = 0;
    for( int i = 0; i < n; i++ ) {
        int start,end;
        std::cin>>start>>end;
        if( end < start ) {
            int temp = end;
            end = start;
            start = temp;
        }

        if( start == 0 and end == w ) {
            sure_length+=1;
        } else if( start != end ){
            //bricks[i] = (std::pair<int,int>(start,end));
            //if( start==end ) std::cout<<start<<","<<end<<std::endl;
            //else 
            bricks.push_back(std::pair<int,int>(start,end));
        }
    }
    //std::cout<<n<<"--"<<bricks.size()<<std::endl;
    n = bricks.size();

    Graph G(n+2+2*(w-1));
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    //int s = n+2*w;
    //int t = n+2*w+1;
    int s = n;
    int t = n+1;
    //std::cout<<"w:"<<w<<std::endl;
    for( int i = 1; i < w; i++ ) {
        int e1 = n+2*i;
        int e2 = n+2*i+1;
        eaG.addEdge(e1,e2,1);
        //std::cout<<(e1-n)/2<<" "<<(e2-n)/2<<std::endl;
    }

    for( int i = 0; i < n; i++ ) {
        int start,end;
        start = bricks[i].first;
        end = bricks[i].second;

        //Add left-edge
        if( start == 0 ) {
            eaG.addEdge(s,i,1);
        } else {
            eaG.addEdge(n+2*start+1,i,1);
        }

        //Add right-edge
        if( end == w ) {
            eaG.addEdge(i,t,1);
        } else {
            eaG.addEdge(i,n+2*end,1);
        }
    }
    
    long flow1 = boost::push_relabel_max_flow(G, s, t);
    std::cout<<sure_length+flow1<<std::endl;
}


int main()
{
    int ntests;
    std::cin>>ntests;
    for( int i = 0; i < ntests; i++ ) {
        play_tetris();
    }

}
