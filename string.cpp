#include <iostream>
#include <cstdio>
#include <cstring>
#include <ostream>
namespace sjtu
{
template<int N>
class string
{
public:
    char s[N+1];
    int len;
    string(){memset(s,0,sizeof(s)),len=0;}
    string(const std::string &a)
    {
        memset(s,0,sizeof(s));
        len=a.length();
        for(int i=0;i<len;i++) s[i]=a[i];
    }
    string(const char *a)
    {
        memset(s,0,sizeof(s));
        len=strlen(a);
        memcpy(s,a,len);
    }
    int length() const {return len;}
    char& operator [] (int a) {return s[a];}
    const char& operator [] (int a) const {return s[a];} 
    template<int M> 
    bool operator < (const string<M> &a) const
    {
        int mnl=std::min(len,a.len);
        for(int i=0;i<mnl;i++) if(s[i]!=a.s[i]) return s[i]<a.s[i];
        return len<a.len;
    }
    template<int M>
    bool operator == (const string<M> &a) const
    {
        if(len!=a.len) return false;
        for(int i=0;i<len;i++) if(s[i]!=a.s[i]) return false;
        return true;
    }
    template<int M>
    bool operator != (const string<M> &a) const {return !(*this==a);}
    bool operator == (const std::string &a) const
    {
        if(len!=a.length()) return false;
        for(int i=0;i<len;i++) if(s[i]!=a[i]) return false;
        return true;
    }
    bool operator != (const std::string &a) const {return !(*this==a);}
    string<N> operator = (const string<N> &a)
    {
        if(this==&a) return *this;
        len=a.len;
        memcpy(s,a.s,len);
        return *this;
    }
    string<N> operator = (const std::string &a)
    {
        len=a.length();
        for(int i=0;i<len;i++) s[i]=a[i];
        return *this;
    }
    template<int M>
    friend std::ostream& operator << (std::ostream &os,const string<M> &p);
    int hash(int base=263,int MOD=998244353) const
    {
        int ret=0;
        for(int i=0;i<len;i++) ret=((1ll*base*ret+s[i])%MOD+MOD)%MOD;
        return ret;
    }
};
template<int M>
std::ostream& operator << (std::ostream &os,const string<M> &p)
{
    for(int i=0;i<p.len;i++) os<<p[i];
    return os;
}
template<int N>
int stoi(const string<N> &p)
{
    int ret=0;
    for(int i=0;i<p.len;i++) ret=ret*10+p[i]-'0';
    return ret;
}
}