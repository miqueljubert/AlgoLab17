#include <vector>
#include <algorithm>
#include <iostream>


void count_backtrack( long mask, const std::vector<long> &values, long current, long last, long sum, std::vector<long> &snl )
{
    if( current == last ) {
        snl.push_back(sum);
        return;
    }

    if( not (mask & 0x01<<current) ) {
        count_backtrack( mask, values, current+1, last, sum, snl );
        return;
    }

    count_backtrack( mask, values, current+1, last, sum+values[current], snl);
    count_backtrack( mask, values, current+1, last, sum-values[current], snl);
}


long count_equal_subsets_sl( long mask, const std::vector<long> &values, long n, long k)
{
    long halfpoint = 0;
    long count = 0;
    for( int bit = 0; bit < n; bit++ ) {

        if( mask & 0x01<<bit ) {
            count++;
        }

        if( count > (n-k)/2 ) {
            halfpoint = bit;
            break;
        }
    }

    std::vector<long> l1,l2;
    l1.reserve(2<<(n));
    l2.reserve(2<<(n));
    count_backtrack(mask, values, 0, halfpoint, 0, l1);
    count_backtrack(mask, values, halfpoint, n, 0, l2);
    
    long total = 0;
    std::sort(l1.begin(),l1.end());
    for( int i = 0; i < l2.size(); i++ ) {
        long target = -l2[i];
        auto lb = std::lower_bound(l1.begin(),l1.end(),target);
        auto ub = std::upper_bound(l1.begin(),l1.end(),target);
        total += ub-lb;
    }
    return total;
    
}


long count_equal_subsets( long mask, const std::vector<long> &values, long count )
{
    //return count_backtrack(mask, values, 0, count, 0);
}


long n_take_k( long n, long k, const std::vector<long> &values) {
    //std::cout<<n<<"-"<<k<<std::endl;

    long total_amount = 0;
    long count = 0;
    long mask = 0;
    while( mask < 0x01<<n ) {
        int count = 0;
        for( int bit = 0; bit < n; bit++ ) {
            if( count > (n-k) ) break;
            if( mask & 0x01<<bit ) {
                count++;
            }
        }
        if( count == (n-k) ) {
            total_amount += count_equal_subsets_sl( mask, values, n, k);
        }
        mask++;
    }
    return total_amount;
    //std::cout<<mask<<std::endl;
}

long n_take_k_prem( long n, long k, std::vector<long> &values)
{
    std::vector<int> c(k);
    for( int i = 0; i < k; i++ ) c[i] = i;
    //std::cout<<"pro:"<<n<<" take "<<k<<std::endl;
    long total_amount = 0;
    while(1) {

        //for( int j = 0; j < k; j++ ) {
        //    std::cout<<c[j]<<" ";
        //}
        //std::cout<<std::endl;
        
        long mask = 0;
        for( int j = 0; j < c.size(); j++ ) {
            mask = mask | (0x01<<c[j]);
        }
        total_amount += count_equal_subsets_sl( mask, values, n, k);

        int i = k-1;
        while( i >= 0 and c[i] == (i+(n-k)) ) i--;
        if( i < 0 ) break;
        
        c[i]++;
        i++;
        while( i < k ) {
            c[i] = c[i-1]+1;
            i++;
        }
    }
    return total_amount;
}

void solve()
{
    long n,k;
    std::cin>>n>>k;

    std::vector<long> values(n);
    for( long i = 0; i < n; i++ ) std::cin>>values[i];

    long total = 0;
    //for( long kp = 0; kp <= k; kp++ ) total += n_take_k(n,kp,values);
    for( long kp = 0; kp <= k; kp++ ) total += n_take_k_prem(n,n-kp,values);
    std::cout<<total<<std::endl;

}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin>>t;
    for( int i = 0; i < t; i++ ) solve();
    //n_take_k_prem(5,3);
}
