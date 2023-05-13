#include "bpt.cpp"
struct ky
{
    char name[65];
    ky(){memset(name,0,65);}
    bool operator < (const ky &a) const
    {
        for(int i=0;i<65;i++) if(name[i]!=a.name[i]) return name[i]<a.name[i];
        return false;
    }
    bool operator == (const ky &a) const
    {
        return memcmp(name,a.name,65)==0;
    }
};
struct peo
{
    ky key;
    int va;
    peo(){key=ky(); va=0;}
    bool operator < (const peo &a) const
    {
        if(!(key==a.key)) return key<a.key;
        return va<a.va;
    }
    bool operator == (const peo &a) const {return key==a.key&&va==a.va;}
};

char f1[5]="init",f2[5]="data";
BPT<peo,25> bpt(f1,f2);
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
            peo tmp;
            scanf("%d",&tmp.va);
            memcpy(tmp.key.name,t,65); 
            bpt.ins(tmp);
        }
        else if(s[0]=='f')
        {
            peo tmp;
            memcpy(tmp.key.name,t,65); 
            sjtu::vector<peo> vec=bpt.get_all(tmp);
            if(vec.size()==0) {printf("null\n");continue;}
            for(sjtu::vector<peo>::iterator it=vec.begin();it!=vec.end();it++) printf("%d ",(*it).va);printf("\n");
        }
        else
        {
            peo tmp;
            scanf("%d",&tmp.va);
            memcpy(tmp.key.name,t,65); 
            bpt.erase(tmp);
            
        }
        memset(s,0,65);
        memset(t,0,65);
    }
    return 0;
}