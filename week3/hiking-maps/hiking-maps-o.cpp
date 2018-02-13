#include <vector>
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;




int build_hiking_map()
{
    int nlegs,nmaps;
    cin>>nlegs>>nmaps;
    
    vector<K::Segment_2> legs(nlegs-1);
    vector<K::Triangle_2> maps(nmaps);
    
    long int x,y,ox,oy;
    cin>>ox>>oy;
    for( int i = 0; i < nlegs-1; i++ ) {
        cin>>x>>y;

        legs[i] = K::Segment_2(K::Point_2(ox,oy),K::Point_2(x,y));
        ox = x;
        oy = y;
    }

    long int x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
    for( int i = 0; i < nmaps; i++ ) {
        cin>>x1>>y1>>x2>>y2>>x3>>y3>>x4>>y4>>x5>>y5>>x6>>y6;

        K::Line_2 l1(K::Point_2(x1,y1),K::Point_2(x2,y2));
        K::Line_2 l2(K::Point_2(x3,y3),K::Point_2(x4,y4));
        K::Line_2 l3(K::Point_2(x5,y5),K::Point_2(x6,y6));

        auto o1 = CGAL::intersection(l1,l2);
        auto o2 = CGAL::intersection(l2,l3);
        auto o3 = CGAL::intersection(l3,l1);

        K::Point_2 i1 = *boost::get<K::Point_2>(&*o1);
        K::Point_2 i2 = *boost::get<K::Point_2>(&*o2);
        K::Point_2 i3 = *boost::get<K::Point_2>(&*o3);


        maps[i] = K::Triangle_2(i1,i2,i3);
    }
    
    vector<vector<int>> covered_legs(nmaps,vector<int>(0));
    for( int i = 0; i < maps.size(); i++ ) {
        for( int j = 0; j < legs.size(); j++ ) {
            CGAL::Bounded_side pos1 = maps[i].bounded_side(legs[j].vertex(0));
            CGAL::Bounded_side pos2 = maps[i].bounded_side(legs[j].vertex(1));
            bool test = (pos1 == CGAL::Bounded_side::ON_BOUNDARY or pos1 == CGAL::Bounded_side::ON_BOUNDED_SIDE) and
                        (pos2 == CGAL::Bounded_side::ON_BOUNDARY or pos2 == CGAL::Bounded_side::ON_BOUNDED_SIDE);
            if (test) {
                covered_legs[i].push_back(j);
            }
        }
    }

    int ncovered = 0;
    vector<int> times_covered(legs.size(),0);
    //Look for first complete cover
    int first_index = 0;
    int last_index = 0;
    int min_cost = -1;

    bool over = false;
    while( not over ) {

        //Get a valid cover
        //cout<<"make valid"<<endl;fflush(stdout);
        while( ncovered < legs.size() ) {
            if( last_index == nmaps ) return min_cost+1;
            for( int i = 0; i < covered_legs[last_index].size(); i++ ) {
                times_covered[covered_legs[last_index][i]]++;
                if( times_covered[covered_legs[last_index][i]] == 1 ) ncovered++;
            }
            last_index++;
        }

         if(min_cost == -1) min_cost = last_index-first_index;
         else min_cost = min(min_cost, last_index-first_index );
        //cout<<"last index "<<last_index<<endl;
        
        //cout<<"make unvalid"<<endl;fflush(stdout);
        //Reduce it until unvalid
        while( ncovered == legs.size() and first_index < last_index ) {
            for( int i = 0; i < covered_legs[first_index].size(); i++ ) {
                times_covered[covered_legs[first_index][i]]--;
                if( times_covered[covered_legs[first_index][i]] == 0 ) {
                    ncovered--;
                }
            }
            first_index++;
            if(min_cost == -1) min_cost = last_index-first_index;
            else min_cost = min(min_cost, last_index-first_index );
        }
        //cout<<"size:"<<covered_legs.size()<<endl;
        //cout<<"first index:"<<first_index<<" last_index: "<<last_index<<endl;fflush(stdout);
        if( last_index == maps.size()-1 ) break;
    }

    return min_cost+1;

    

}


int main()
{
   std::ios_base::sync_with_stdio(false);
   int ntests;
   cin>>ntests;
   for( int i = 0; i < ntests; i++ ) {
        cout<<build_hiking_map()<<endl;
   }
}
