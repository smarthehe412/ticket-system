#include "bpt.cpp"
#include <cstring>
struct str
{
    char name[65];
    int len;
    str(){memset(name,0,65);len=0;}
    str(char *s)
    {
        memcpy(name,s,65);
        len=std::strlen(s);
    }
    bool operator < (const str &a) const
    {
        int tlen=std::min(len,a.len);
        for(int i=0;i<tlen;i++) if(name[i]!=a.name[i]) return name[i]<a.name[i];
        return len<a.len;
    }
    bool operator == (const str &a) const
    {
        if(len!=a.len) return false;
        return memcmp(name,a.name,len)==0;
    }
};
sjtu::multiBPT<str,int,25> bpt("init","data");
char s[65],t[65];

int main()
{
    freopen("in","r",stdin);
    freopen("out","w",stdout);
    int n;scanf("%d",&n);
    for(int i=1;i<=n;i++)
    {
        scanf("%s%s",s,t);
        if(s[0]=='i')
        {
            int va;scanf("%d",&va);
            bpt.insert(str(t),va);
        }
        else if(s[0]=='f')
        {
            sjtu::vector<int> vec=bpt.query(str(t));
            if(vec.size()==0) printf("null");
            for(sjtu::vector<int>::iterator it=vec.begin();it!=vec.end();it++) printf("%d ",*it);
            printf("\n");
        }
        else
        {
            int va;scanf("%d",&va); 
            bpt.erase(str(t),va);
        }
    }
    return 0;
}