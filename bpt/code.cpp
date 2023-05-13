#include "bpt.cpp"
struct str
{
    char name[65];
    str(){memset(name,0,65);}
    str(char *s){memcpy(name,s,65);}
    bool operator < (const str &a) const
    {
        for(int i=0;i<65;i++) if(name[i]!=a.name[i]) return name[i]<a.name[i];
        return false;
    }
    bool operator == (const str &a) const
    {
        return memcmp(name,a.name,65)==0;
    }
};
char f1[5]="init",f2[5]="data";
BPT<str,int,25> bpt(f1,f2);
char s[65],t[65];

int main()
{
    //freopen("in","r",stdin);
    //freopen("out","w",stdout);
    int n;scanf("%d",&n);
    for(int i=1;i<=n;i++)
    {
        scanf("%s%s",s,t);
        if(s[0]=='i')
        {
            int va;scanf("%d",&va);
            bpt.ins(sjtu::make_pair(str(t),va));
        }
        else if(s[0]=='f')
        {
            sjtu::vector<sjtu::pair<str,int>> vec=bpt.get_all(str(t));
            if(vec.size()==0) printf("null");
            for(sjtu::vector<sjtu::pair<str,int>>::iterator it=vec.begin();it!=vec.end();it++) printf("%d ",(*it).second);
            printf("\n");
        }
        else
        {
            int va;scanf("%d",&va); 
            bpt.erase(sjtu::make_pair(str(t),va));
        }
        memset(s,0,65);
        memset(t,0,65);
    }
    return 0;
}