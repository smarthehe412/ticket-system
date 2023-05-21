#include "bpt.cpp"
#include <cstring>
struct kv
{
    char name[65];
    int len,va;
    kv(){memset(name,0,65);len=va=0;}
    kv(char *s,int t): va(t)
    {
        memcpy(name,s,65);
        len=std::strlen(s);
    }
    bool operator < (const kv &a) const
    {
        int tlen=std::min(len,a.len);
        for(int i=0;i<tlen;i++) if(name[i]!=a.name[i]) return name[i]<a.name[i];
        if(len!=a.len) return len<a.len;
        return va<a.va;
    }
    bool operator == (const kv &a) const
    {
        if(len!=a.len||va!=a.va) return false;
        return memcmp(name,a.name,len)==0;
    }
};
sjtu::BPT<kv,int,25> bpt("init","data","value");
char s[65],t[65];

int main()
{
    // freopen("in","r",stdin);
    // freopen("out","w",stdout);
    int n;scanf("%d",&n);
    for(int i=1;i<=n;i++)
    {
        scanf("%s%s",s,t);
        if(s[0]=='i')
        {
            int va;scanf("%d",&va);
            bpt.insert(kv(t,va),va);
        }
        else if(s[0]=='f')
        {
            sjtu::vector<int> vec=bpt.find_range(kv(t,-2e9),kv(t,2e9));
            if(vec.size()==0) printf("null");
            for(sjtu::vector<int>::iterator it=vec.begin();it!=vec.end();it++) printf("%d ",*it);
            printf("\n");
        }
        else
        {
            int va;scanf("%d",&va); 
            bpt.erase(kv(t,va));
        }
    }
    return 0;
}