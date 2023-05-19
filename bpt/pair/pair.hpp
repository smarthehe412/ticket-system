#include <iostream>
#include <cstdio>

namespace sjtu
{
template<class T1,class T2>
class pair
{
public:
    T1 first;
    T2 second;
    pair(){first=T1();second=T2();}
    pair(T1 key,T2 value): first(key),second(value) {}
    bool operator < (const pair<T1,T2> &a) const
    {
        return first<a.first;
    }
    bool operator == (const pair<T1,T2> &a) const
    {
        return first==a.first&&second==a.second;
    }
};
template<class T1,class T2>
pair<T1,T2> make_pair(T1 key,T2 value){return pair<T1,T2>(key,value);}
}