#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <map>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

typedef Triangulation::Point Point;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_circulator Vertex_circulator;


void play()
{
    // read number of points
    long n_players,n_lights;
    std::cin>>n_players>>n_lights;


    std::vector<Point> player(n_players);
    std::vector<long> player_radius(n_players);
    std::vector<Point> light(n_lights);

    for( int i = 0; i < n_players; i++ ) {
        long x,y,r;
        std::cin>>x>>y>>r;
        player[i] = Point(x,y);
        player_radius[i] = r;
    }

    long h;
    std::cin>>h;

    std::map<Point,long> light_index;//Relates the point to the corresponding index. Can I have overlaps??
    for( int i = 0; i < n_lights; i++ ) {
        long x,y;

        std::cin>>x>>y;
        light[i] = Point(x,y);

        if( light_index.find(light[i]) == light_index.end() ) {//If there are two on the same position, only the first one will have an effect
            light_index[light[i]] = i;
        }
    }
    Triangulation t;

    t.insert(light.begin(),light.end());

    long winner_round = -1;
    std::vector<long> largest_winners;
    for( int i = 0; i < n_players; i++ ) {
        
        Vertex_handle nn = t.nearest_vertex(player[i]);

        K::FT d = CGAL::squared_distance(nn->point(),player[i]);
        K::FT rh = K::FT(player_radius[i])*K::FT(player_radius[i]) + 2*K::FT(h)*K::FT(player_radius[i]) + K::FT(h)*K::FT(h);

        bool hit = d < rh;
        //std::cout<<"winner round:"<<winner_round<<std::endl;
        if( not hit ) { //If not hit he is a winner for sure
            //std::cout<<i<<" ";
            //std::cout<<"winner_round: "<<winner_round<<" nlights: "<<n_lights<<std::endl;
            //std::cout<<(winner_round<n_lights)<<std::endl;
            if( winner_round < n_lights ) {
                largest_winners = std::vector<long>();
                largest_winners.push_back(i);
                winner_round = n_lights;
                //std::cout<<"new winner round:"<<winner_round<<std::endl;
            } else {
                largest_winners.push_back(i);
            }
            continue;
        }

        long round = light_index[nn->point()];
        

        //If hit check when which is the round he gets eliminated
        //long round = light_index[nn->point()];//Get the round of the nearest light
        for( int j = 0; j <= round; j++ ) {
            Point l = light[j];
            K::FT d = CGAL::squared_distance(l,player[i]);
            K::FT rh = K::FT(player_radius[i])*K::FT(player_radius[i]) + 2*K::FT(h)*K::FT(player_radius[i]) + K::FT(h)*K::FT(h);
            bool hit = d < rh;
            if( hit ) {//if hit he is eliminated at this round
                //std::cout<<i<<" ";
                if( winner_round < j ) {
                    largest_winners = std::vector<long>();
                    largest_winners.push_back(i);
                    winner_round = j;
                } else if (winner_round == j){
                    largest_winners.push_back(i);
                }
                break;
            }
        }
        
        //Get the smallest lamp that kills him
        /*Vertex_circulator vi = t.incident_vertices(nn);
        do {

        } while(++vi != t.incident_vertices(nn));*/
    }
    for( int i = 0; i < largest_winners.size(); i++ ) std::cout<<largest_winners[i]<<" ";
    std::cout<<std::endl;

    /*
    // construct triangulation
    for (std::size_t i = 0; i < n; ++i) {
        Triangulation::Point p;
        std::cin >> p;
        t.insert(p);
    }
    // output all triangles
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
        std::cout << t.triangle(f) << "\n";
    */
}
int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) play();
}
