#include <iostream>
#include <cassert>
#include <queue>
#include <algorithm>
#include <utility>
#include <vector>

typedef std::pair<int,int> Pair;

int length( const Pair &p ) {
    return p.second-p.first + 1;
}

// p1 ends before p2
int overlap( const Pair &p1, const Pair &p2 ) {
    int o = p2.first - p1.second;
    return o > 0? 0 : o;
}

int max_coverage( std::vector<Pair> &intervals, int i, std::vector<int> &m_coverage, std::vector<bool> &taken, std::vector<int> &next_element )
{
    if( i >= intervals.size() ) return 0;
    if( m_coverage[i] != -1 ) return m_coverage[i];

    int end = intervals[i].second;
    int non_conflicting = i;
    while( non_conflicting < intervals.size() and intervals[non_conflicting].first <= end ) non_conflicting++;


    int v1,v2;
    m_coverage[i] = std::max( v1 = max_coverage(intervals, non_conflicting, m_coverage, taken, next_element) + length(intervals[i]),
                              v2 = max_coverage(intervals, i+1, m_coverage, taken, next_element) );
    if( v1 > v2 ) {
        taken[i] = true;
        next_element[i] = non_conflicting;
    } else {
        taken[i] = false;
        next_element[i] = -1;
    }
    //std::cout<<"vs:"<<v1<<","<<v2<<std::endl;

   


    return m_coverage[i];
}


int defensive_line()
{
    int n_def, n_att, k;

    std::cin>>n_def>>n_att>>k;

    std::vector<int> values(n_def);
    for( int i = 0; i < n_def; i++ ) std::cin>>values[i];

    //Sliding window to detect the intervals where we do have a sum of k
    std::vector<Pair> k_intervals;

    int i_s = 0;
    int i_e = 0;
    int val = values[0];
    bool stop = false;
    
    while( i_e < n_def ) {
        if( val == k ) k_intervals.push_back(Pair(i_s,i_e));
        if( val < k and i_e < n_def ) {
            i_e++;
            val += values[i_e];
        } else {
            val -= values[i_s];
            i_s++;
        }
    }

    //std::cerr<<"out"<<std::endl;
    //std::cout<<"n:"<<k_intervals.size()<<std::endl;
    
    //std::cout<<k_intervals.size()<<std::endl;
    if( k_intervals.size() < n_att) {
        std::cout<<"fail"<<std::endl;
        return 0 ;
    }
    //std::cout<<"success"<<std::endl;

    
    std::sort(k_intervals.begin(),k_intervals.end(),
            [] ( const Pair &i1, const Pair &i2 ) {
                if( i1.second == i2.second) return i1.first < i2.first;
                return i1.second < i2.second;
            });

    //std::cout<<"k:"<<k<<std::endl;
    //for( int i = 0; i < k_intervals.size(); i++ ) {
    //    std::cout<<k_intervals[i].first<<","<<k_intervals[i].second<<std::endl;
    //}

    std::vector<bool> taken(k_intervals.size());
    std::vector<int> next_element(k_intervals.size());
    std::vector<int> m_coverage(k_intervals.size(),-1);

    int cover = max_coverage(k_intervals,0,m_coverage,taken,next_element);

    //for( int i = 0; i < taken.size(); i++ ) std::cout<<taken[i]<<std::endl;


    std::priority_queue<int,std::vector<int>,std::greater<int>> ss;
    int start = 0;
    while( start < next_element.size() ) {
        int el = next_element[start];
        if( el == -1 ) {
            start++;
        } else {
            //std::cout<<start<<",";
            ss.push(length(k_intervals[start]));
            start = el;
        }
    }
    //std::cout<<std::endl;
    //
    if( ss.size() < n_att ) {
        std::cout<<"fail"<<std::endl;
        return 0;
    }
    while( ss.size() > n_att ) {
        cover -= ss.top();
        ss.pop();
    }
    std::cout<<cover<<std::endl;

    
}


int main()
{
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) {
        defensive_line();
    }
}
