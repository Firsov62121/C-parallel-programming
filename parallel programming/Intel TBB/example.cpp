#include <iostream>
#include "tbb/tbb.h"
//threading building blocks
namespace tbb{
//https://russianblogs.com/article/7850473772/
//https://github.com/oneapi-src/oneTBB
float Foo(float b)
{
    return b * b;
}

void SerialApplyFoo( float a[], size_t n )
{
    for(size_t i=0; i!=n; ++i )
        Foo(a[i]);
}


class ApplyFoo
{
    float *const my_a;
public:
    void operator ()(const blocked_range<size_t>& r) const
    {
        float *a = my_a;
        for (size_t i = r.begin(); i != r.end(); ++i) 
            Foo(a[i]);
    }
    ApplyFoo(float a[]) : my_a(a) {}
};

void ParallelApplyFoo(float a[], size_t n)
{
    parallel_for(blocked_range<size_t>(0, n), ApplyFoo(a));
}

#pragma warning( disable: 588)
void ParallelApplyFoo2(float *a, size_t n)
{
    parallel_for(blocked_range<size_t>(0, n),[=](const blocked_range<size_t>& r)
    {
        for (size_t i = r.begin(); i != r.end(); ++i)
            Foo(a[i]);
    });
}

void ParallelApplyFoo( float a[], size_t n )
{
    //G/2 <= размер разбиения <= G
    int G = 5;
    parallel_for(blocked_range<size_t>(0, n, G), ApplyFoo(a), simple_partitioner());
}

//parallel_reduce
float SerialSumFoo(float a[], size_t n)
{
    float sum = 0;
    for (size_t i = 0; i != n; ++i)
        sum += Foo(a[i]);
    return sum;
}

class SumFoo
{
    const float* my_a;
public:
    float my_sum;
    void operator()( const blocked_range<size_t>& r )
    {
        const float *a = my_a;
        float sum = my_sum;
        size_t end = r.end();
        for( size_t i=r.begin(); i!=end; ++i )
        sum += Foo(a[i]);
        my_sum = sum;
    }
    SumFoo( SumFoo& x, split ) : my_a(x.my_a), my_sum(0) {}
    void join( const SumFoo& y ) {my_sum+=y.my_sum;}
    SumFoo(const float a[] ) : my_a(a), my_sum(0) {}
};

float ParallelSumFoo( const float a[], size_t n )
{
    SumFoo sf(a);
    parallel_reduce(blocked_range<size_t>(0,n), sf );
    return sf.my_sum;
}

//parallel do
template<typename Item>
void SerialApplyFooToList( const std::list<Item>& list ) {
    for( std::list<Item>::const_iterator i=list.begin(); i!=list.end(); ++i )
        Foo(*i);
}

template<typename Item>
class ApplyFoo2 {
public:
    void operator()( Item& item ) const {
        Foo(item);
    }
};

template<typename Item>
void ParallelApplyFooToList( const std::list<Item>& list ) {
    parallel_for_each(list.begin(), list.end(), ApplyFoo2() );
}

}

int main()
{
    return 0;
}