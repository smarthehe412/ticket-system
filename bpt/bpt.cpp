#include <iostream>
#include <cstring>
#include <cstdlib>
#include "filesystem.cpp"
#include "vector/vector.hpp"

namespace sjtu
{
template<int N>
struct HASH_MAP
{
private:
    int bg[N],nx[N],id[N],val[N],w[N],ndc,wt;
public:
    HASH_MAP()
    {
        memset(bg,0,sizeof(bg));
        memset(nx,0,sizeof(nx));
        memset(id,0,sizeof(id));
        memset(val,0,sizeof(val));
        memset(w,0,sizeof(w));
        ndc=wt=0;
    }
    void clear()
    {
        memset(bg,0,sizeof(bg));
        memset(nx,0,sizeof(nx));
        memset(id,0,sizeof(id));
        memset(val,0,sizeof(val));
        memset(w,0,sizeof(w));
        ndc=wt=0;
    }
    void ins(int x,int v)
    {
        int tmp=x%N;
        int now=wt?w[wt--]:++ndc;
        nx[now]=bg[tmp];bg[tmp]=now;
        id[now]=x,val[now]=v;
    }
    void del(int x,int y)
    {
        int tmp=x%N,las=0;
        for(int i=bg[tmp];i;i=nx[i])
        {
            if(id[i]==x&&val[i]==y)
            {
                if(las) nx[las]=nx[i];
                else bg[tmp]=nx[i];
                w[++wt]=i;
                return;
            }
            las=i;
        }
    }
    int query(int x)
    {
        int tmp=x%N;
        for(int i=bg[tmp];i;i=nx[i]) if(id[i]==x) return val[i];
        return -1;
    }
};
template<class T,int CN,int N>
class LRU_CACHE
{
private:
    HASH_MAP<N> hashmap;
    FILE_SYSTEM<T> f;
    T va[CN];
    int hd,tl,siz,bf[CN],nx[CN],pos[CN];
public:
    LRU_CACHE(const char* dat): f(dat)
    {
        hashmap.clear();
        for(int i=0;i<CN;i++) va[i]=T(),bf[i]=nx[i]=-1,pos[i]=0;
        hd=tl=-1;
        siz=0;
    }
    ~LRU_CACHE()
    {
        for(int i=0;i<CN;i++) if(pos[i]) f.write(pos[i],va[i]);
    }
    void clear()
    {
        for(int i=0;i<CN;i++) if(pos[i]) f.write(pos[i],va[i]);
        hashmap.clear();
        for(int i=0;i<CN;i++) va[i]=T(),bf[i]=nx[i]=-1,pos[i]=0;
        hd=tl=-1;
        siz=0;
    }
    T query(int x)
    {
        int tx=hashmap.query(x);
        if(tx==-1)
        {
            int now;
            if(siz==CN)
            {
                hashmap.del(pos[tl],tl);
                f.write(pos[tl],va[tl]);
                now=tl,tl=bf[tl];nx[tl]=-1;
                bf[hd]=now,nx[now]=hd;
                bf[now]=-1,hd=now;
            }
            else
            {
                now=siz++;
                if(~hd) bf[hd]=now,nx[now]=hd;
                else tl=now;
                hd=now;
            }
            pos[now]=x,va[now]=f.read(x);
            hashmap.ins(x,now);
            return va[now];
        }
        else
        {
            if(tx==hd) return va[tx]; 
            if(~bf[tx]) nx[bf[tx]]=nx[tx];
            if(~nx[tx]) bf[nx[tx]]=bf[tx];
            if(tx==tl) tl=bf[tx];
            bf[tx]=-1,nx[tx]=hd;
            bf[hd]=tx,hd=tx;
            return va[tx];
        }
    }
    void modify(int x,T tmp)
    {
        int tx=hashmap.query(x);
        if(tx==-1)
        {
            int now;
            if(siz==CN)
            {
                hashmap.del(pos[tl],tl);
                f.write(pos[tl],va[tl]);
                now=tl,tl=bf[tl];nx[tl]=-1;
                bf[hd]=now,nx[now]=hd;
                bf[now]=-1,hd=now;
            }
            else
            {
                now=siz++;
                if(~hd) bf[hd]=now,nx[now]=hd;
                else tl=now;
                hd=now;
            }
            pos[now]=x,va[now]=tmp;
            hashmap.ins(x,now);
        }
        else
        {
            if(tx==hd)
            {
                va[tx]=tmp;
                return;
            }
            if(~bf[tx]) nx[bf[tx]]=nx[tx];
            if(~nx[tx]) bf[nx[tx]]=bf[tx];
            if(tx==tl) tl=bf[tx];
            bf[tx]=-1,nx[tx]=hd,va[tx]=tmp;
            bf[hd]=tx,hd=tx;
        }
    }
};
template<class KEY,class VALUE,int B=32,int CN=16384,int N=100003>
class BPT
{
    typedef pair<KEY,int> T;
private:
    int root,ndc,vac;
    struct node
    {
        int id,cnt,son[B+2];
        bool is_leaf;int bef_leaf,nxt_leaf;
        T val[B+2];
        node()
        {
            id=cnt=0;
            is_leaf=false,bef_leaf=nxt_leaf=0;
            for(int i=0;i<=B+1;i++) val[i]=T();
            memset(son,0,sizeof(son));
        }
    };
    FILE_SYSTEM<int> f1;
    FILE_SYSTEM<node> f2;
    FILE_SYSTEM<VALUE> f3;
    LRU_CACHE<node,CN,N> node_cache;
    node find_leaf(int now,const KEY &k)
    {
        node tmp=node_cache.query(now);
        if(tmp.cnt==0) return tmp;
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid].first||k==tmp.val[mid].first) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(ret==tmp.cnt&&tmp.nxt_leaf) return node_cache.query(tmp.nxt_leaf);
            return tmp;
        }
        int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(k<tmp.val[mid].first) r=mid-1,ret=mid;
            else l=mid+1;
        }
        return find_leaf(tmp.son[ret],k);
    }
    void split_leaf(node &tmp,node &fa,int ret)
    {
        node oth;
        tmp.cnt=(B+1)/2;
        oth.id=++ndc,oth.cnt=B-(B+1)/2;
        oth.is_leaf=true;
        oth.bef_leaf=tmp.id,oth.nxt_leaf=tmp.nxt_leaf;
        if(tmp.nxt_leaf)
        {
            node nxt=node_cache.query(tmp.nxt_leaf);
            nxt.bef_leaf=oth.id;
            node_cache.modify(nxt.id,nxt);
        }
        tmp.nxt_leaf=oth.id;
        memcpy(oth.val,tmp.val+tmp.cnt,oth.cnt*sizeof(T));
        if(fa.id==0)
        {
            node rt;
            rt.id=++ndc;rt.cnt=2;
            rt.son[0]=tmp.id,rt.son[1]=oth.id;
            rt.val[0]=oth.val[0];
            root=rt.id;
            node_cache.modify(oth.id,oth);
            node_cache.modify(root,rt);
            return;
        }
        memmove(fa.val+ret+1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
        memmove(fa.son+ret+2,fa.son+ret+1,(fa.cnt-ret-1)*sizeof(int));
        fa.val[ret]=oth.val[0],fa.son[ret+1]=oth.id;
        fa.cnt++;
        node_cache.modify(fa.id,fa);
        node_cache.modify(oth.id,oth);
    }
    void split_node(node &tmp,node &fa,int ret)
    {
        node oth;
        tmp.cnt=(B+1)/2;
        oth.id=++ndc,oth.cnt=B+1-(B+1)/2;
        memcpy(oth.val,tmp.val+tmp.cnt,(oth.cnt-1)*sizeof(T));
        memcpy(oth.son,tmp.son+tmp.cnt,oth.cnt*sizeof(int));
        if(fa.id==0)
        {
            node rt;
            rt.id=++ndc;rt.cnt=2;
            rt.son[0]=tmp.id,rt.son[1]=oth.id;
            rt.val[0]=tmp.val[tmp.cnt-1];
            root=rt.id;
            node_cache.modify(oth.id,oth);
            node_cache.modify(root,rt);
            return;
        }
        memmove(fa.val+ret+1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
        memmove(fa.son+ret+2,fa.son+ret+1,(fa.cnt-ret-1)*sizeof(int));
        fa.val[ret]=tmp.val[tmp.cnt-1],fa.son[ret+1]=oth.id;
        fa.cnt++;
        node_cache.modify(fa.id,fa);
        node_cache.modify(oth.id,oth);
    }
    void merge_leaf(node &tmp,node &fa,int ret)
    {
        node lf,rt;
        if(ret) lf=node_cache.query(fa.son[ret-1]);
        if(ret<fa.cnt-1) rt=node_cache.query(fa.son[ret+1]);
        if(lf.id&&lf.cnt>B/2)
        {
            memmove(tmp.val+1,tmp.val,tmp.cnt*sizeof(T));
            tmp.val[0]=lf.val[lf.cnt-1];
            tmp.cnt++,lf.cnt--;
            fa.val[ret-1]=tmp.val[0];
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else if(rt.id&&rt.cnt>B/2)
        {
            tmp.val[tmp.cnt]=rt.val[0];
            memmove(rt.val,rt.val+1,(rt.cnt-1)*sizeof(T));
            tmp.cnt++,rt.cnt--;
            fa.val[ret]=rt.val[0];
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
        else if(lf.id)
        {
            memmove(tmp.val+lf.cnt,tmp.val,tmp.cnt*sizeof(T));
            memcpy(tmp.val,lf.val,lf.cnt*sizeof(T));
            tmp.cnt+=lf.cnt;
            tmp.bef_leaf=lf.bef_leaf;
            if(lf.bef_leaf)
            {
                node bef=node_cache.query(lf.bef_leaf);
                bef.nxt_leaf=tmp.id;
                node_cache.modify(bef.id,bef);
            }
            memmove(fa.val+ret-1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
            memmove(fa.son+ret-1,fa.son+ret,(fa.cnt-ret)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else
        {
            memcpy(tmp.val+tmp.cnt,rt.val,rt.cnt*sizeof(T));
            tmp.cnt+=rt.cnt;
            tmp.nxt_leaf=rt.nxt_leaf;
            if(rt.nxt_leaf)
            {
                node nxt=node_cache.query(rt.nxt_leaf);
                nxt.bef_leaf=tmp.id;
                node_cache.modify(nxt.id,nxt);
            }
            memmove(fa.val+ret,fa.val+ret+1,(fa.cnt-ret-2)*sizeof(T));
            memmove(fa.son+ret+1,fa.son+ret+2,(fa.cnt-ret-2)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
    }
    void merge_node(node &tmp,node &fa,int ret)
    {
        node lf,rt;
        if(ret) lf=node_cache.query(fa.son[ret-1]);
        if(ret<fa.cnt-1) rt=node_cache.query(fa.son[ret+1]);
        if(lf.id&&lf.cnt>(B+1)/2)
        {
            memmove(tmp.val+1,tmp.val,(tmp.cnt-1)*sizeof(T));
            memmove(tmp.son+1,tmp.son,tmp.cnt*sizeof(int));
            tmp.val[0]=fa.val[ret-1],tmp.son[0]=lf.son[lf.cnt-1];
            fa.val[ret-1]=lf.val[lf.cnt-2];
            tmp.cnt++,lf.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else if(rt.id&&rt.cnt>(B+1)/2)
        {
            tmp.val[tmp.cnt-1]=fa.val[ret],tmp.son[tmp.cnt]=rt.son[0];
            fa.val[ret]=rt.val[0];
            memmove(rt.val,rt.val+1,(rt.cnt-2)*sizeof(T));
            memmove(rt.son,rt.son+1,(rt.cnt-1)*sizeof(int));
            tmp.cnt++,rt.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
        else if(lf.id)
        {
            memmove(tmp.val+lf.cnt,tmp.val,(tmp.cnt-1)*sizeof(T));
            memmove(tmp.son+lf.cnt,tmp.son,tmp.cnt*sizeof(int));
            memcpy(tmp.val,lf.val,(lf.cnt-1)*sizeof(T));
            memcpy(tmp.son,lf.son,lf.cnt*sizeof(int));
            tmp.val[lf.cnt-1]=fa.val[ret-1];
            tmp.cnt+=lf.cnt;
            memmove(fa.val+ret-1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
            memmove(fa.son+ret-1,fa.son+ret,(fa.cnt-ret)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else
        {
            memcpy(tmp.val+tmp.cnt,rt.val,(rt.cnt-1)*sizeof(T));
            memcpy(tmp.son+tmp.cnt,rt.son,rt.cnt*sizeof(int));
            tmp.val[tmp.cnt-1]=fa.val[ret];
            tmp.cnt+=rt.cnt;
            memmove(fa.val+ret,fa.val+ret+1,(fa.cnt-ret-2)*sizeof(T));
            memmove(fa.son+ret+1,fa.son+ret+2,(fa.cnt-ret-2)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
    }
public:
    BPT(const char *ini,const char *dat,const char *val): node_cache(dat),f1(ini),f2(dat),f3(val)
    {
        if(f1.empty())
        {
            root=1,ndc=1,vac=0;
            node tmp;
            tmp.id=1,tmp.cnt=0;
            tmp.is_leaf=true;
            f2.write(1,tmp);
        }
        else
        {
            root=f1.read(1);
            ndc=f1.read(2);
            vac=f1.read(3);
        }
    }
    ~BPT()
    {
        f1.write(1,root);
        f1.write(2,ndc);
        f1.write(3,vac);
    }
    void clear()
    {
        node_cache.clear();
        root=1,ndc=1,vac=0;
        node tmp;
        tmp.id=1,tmp.cnt=0;
        tmp.is_leaf=true;
        f2.write(1,tmp);
    }
    bool empty() {return vac==0;}
    VALUE query(const KEY &k)
    {
        node tmp=find_leaf(root,k);
        if(tmp.cnt==0) return VALUE();
        int l=0,r=tmp.cnt-1,ret=tmp.cnt;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(k<tmp.val[mid].first) r=mid-1,ret=mid;
            else l=mid+1;
        }
        if(!(ret&&k==tmp.val[ret-1].first)) return VALUE();
        return f3.read(tmp.val[ret-1].second);
    }
    node insert(const KEY &k,const VALUE &v,node tmp=node(),node fa=node(),int fret=-1)
    {
        if(!tmp.id) tmp=node_cache.query(root);
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid].first) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(ret&&k==tmp.val[ret-1].first) return fa;
            memmove(tmp.val+ret+1,tmp.val+ret,(tmp.cnt-ret)*sizeof(T));
            tmp.val[ret]=T(k,++vac),tmp.cnt++;
            f3.write(vac,v);
            if(tmp.cnt==B) split_leaf(tmp,fa,fret);
            node_cache.modify(tmp.id,tmp);
        }
        else
        {
            int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid].first) r=mid-1,ret=mid;
                else l=mid+1;
            }
            tmp=insert(k,v,node_cache.query(tmp.son[ret]),tmp,ret);
            if(tmp.cnt==B+1) split_node(tmp,fa,fret);
            node_cache.modify(tmp.id,tmp);
        }
        return fa;
    }
    node erase(const KEY &k,node tmp=node(),node fa=node(),int fret=-1)
    {
        if(!tmp.id) tmp=node_cache.query(root);
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid].first) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(!(ret&&k==tmp.val[ret-1].first)) return fa;
            memmove(tmp.val+ret-1,tmp.val+ret,(tmp.cnt-ret)*sizeof(T));
            tmp.cnt--;
            if(tmp.id==root)
            {
                node_cache.modify(tmp.id,tmp);
                return fa;
            }
            if(tmp.cnt<B/2) merge_leaf(tmp,fa,fret);
            node_cache.modify(tmp.id,tmp);
        }
        else
        {
            int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid].first) r=mid-1,ret=mid;
                else l=mid+1;
            }
            tmp=erase(k,node_cache.query(tmp.son[ret]),tmp,ret);
            if(tmp.id==root)
            {
                if(tmp.cnt==1) root=tmp.son[0];
                return fa;
            }
            if(tmp.cnt<(B+1)/2)
            {
                merge_node(tmp,fa,fret);
                node_cache.modify(tmp.id,tmp);
            }
        }
        return fa;
    }
};
template<class KEY,class VALUE,int B=32,int CN=16384,int N=100003>
class multiBPT
{
private:
    struct T
    {
        KEY first;
        VALUE second;
        T(){first=KEY(),second=VALUE();}
        T(const KEY &first,const VALUE &second): first(first),second(second) {}
        bool operator < (const T &a)
        {
            if(first==a.first) return second<a.second;
            return first<a.first;
        }
        bool operator == (const T &a) {return first==a.first&&second==a.second;}
    };
    int root,ndc;
    struct node
    {
        int id,cnt,son[B+2];
        bool is_leaf;int bef_leaf,nxt_leaf;
        T val[B+2];
        node()
        {
            id=cnt=0;
            is_leaf=false,bef_leaf=nxt_leaf=0;
            for(int i=0;i<=B+1;i++) val[i]=T();
            memset(son,0,sizeof(son));
        }
    };
    FILE_SYSTEM<int> f1;
    FILE_SYSTEM<node> f2;
    LRU_CACHE<node,CN,N> node_cache;
    node find_leaf(int now,const KEY &k)
    {
        node tmp=node_cache.query(now);
        if(tmp.cnt==0) return tmp;
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid].first||k==tmp.val[mid].first) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(ret==tmp.cnt&&tmp.nxt_leaf) return node_cache.query(tmp.nxt_leaf);
            return tmp;
        }
        int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(k<tmp.val[mid].first||k==tmp.val[mid].first) r=mid-1,ret=mid;
            else l=mid+1;
        }
        return find_leaf(tmp.son[ret],k);
    }
    void split_leaf(node &tmp,node &fa,int ret)
    {
        node oth;
        tmp.cnt=(B+1)/2;
        oth.id=++ndc,oth.cnt=B-(B+1)/2;
        oth.is_leaf=true;
        oth.bef_leaf=tmp.id,oth.nxt_leaf=tmp.nxt_leaf;
        if(tmp.nxt_leaf)
        {
            node nxt=node_cache.query(tmp.nxt_leaf);
            nxt.bef_leaf=oth.id;
            node_cache.modify(nxt.id,nxt);
        }
        tmp.nxt_leaf=oth.id;
        memcpy(oth.val,tmp.val+tmp.cnt,oth.cnt*sizeof(T));
        if(fa.id==0)
        {
            node rt;
            rt.id=++ndc;rt.cnt=2;
            rt.son[0]=tmp.id,rt.son[1]=oth.id;
            rt.val[0]=oth.val[0];
            root=rt.id;
            node_cache.modify(oth.id,oth);
            node_cache.modify(root,rt);
            return;
        }
        memmove(fa.val+ret+1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
        memmove(fa.son+ret+2,fa.son+ret+1,(fa.cnt-ret-1)*sizeof(int));
        fa.val[ret]=oth.val[0],fa.son[ret+1]=oth.id;
        fa.cnt++;
        node_cache.modify(fa.id,fa);
        node_cache.modify(oth.id,oth);
    }
    void split_node(node &tmp,node &fa,int ret)
    {
        node oth;
        tmp.cnt=(B+1)/2;
        oth.id=++ndc,oth.cnt=B+1-(B+1)/2;
        memcpy(oth.val,tmp.val+tmp.cnt,(oth.cnt-1)*sizeof(T));
        memcpy(oth.son,tmp.son+tmp.cnt,oth.cnt*sizeof(int));
        if(fa.id==0)
        {
            node rt;
            rt.id=++ndc;rt.cnt=2;
            rt.son[0]=tmp.id,rt.son[1]=oth.id;
            rt.val[0]=tmp.val[tmp.cnt-1];
            root=rt.id;
            node_cache.modify(oth.id,oth);
            node_cache.modify(root,rt);
            return;
        }
        memmove(fa.val+ret+1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
        memmove(fa.son+ret+2,fa.son+ret+1,(fa.cnt-ret-1)*sizeof(int));
        fa.val[ret]=tmp.val[tmp.cnt-1],fa.son[ret+1]=oth.id;
        fa.cnt++;
        node_cache.modify(fa.id,fa);
        node_cache.modify(oth.id,oth);
    }
    void merge_leaf(node &tmp,node &fa,int ret)
    {
        node lf,rt;
        if(ret) lf=node_cache.query(fa.son[ret-1]);
        if(ret<fa.cnt-1) rt=node_cache.query(fa.son[ret+1]);
        if(lf.id&&lf.cnt>B/2)
        {
            memmove(tmp.val+1,tmp.val,tmp.cnt*sizeof(T));
            tmp.val[0]=lf.val[lf.cnt-1];
            tmp.cnt++,lf.cnt--;
            fa.val[ret-1]=tmp.val[0];
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else if(rt.id&&rt.cnt>B/2)
        {
            tmp.val[tmp.cnt]=rt.val[0];
            memmove(rt.val,rt.val+1,(rt.cnt-1)*sizeof(T));
            tmp.cnt++,rt.cnt--;
            fa.val[ret]=rt.val[0];
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
        else if(lf.id)
        {
            memmove(tmp.val+lf.cnt,tmp.val,tmp.cnt*sizeof(T));
            memcpy(tmp.val,lf.val,lf.cnt*sizeof(T));
            tmp.cnt+=lf.cnt;
            tmp.bef_leaf=lf.bef_leaf;
            if(lf.bef_leaf)
            {
                node bef=node_cache.query(lf.bef_leaf);
                bef.nxt_leaf=tmp.id;
                node_cache.modify(bef.id,bef);
            }
            memmove(fa.val+ret-1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
            memmove(fa.son+ret-1,fa.son+ret,(fa.cnt-ret)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else
        {
            memcpy(tmp.val+tmp.cnt,rt.val,rt.cnt*sizeof(T));
            tmp.cnt+=rt.cnt;
            tmp.nxt_leaf=rt.nxt_leaf;
            if(rt.nxt_leaf)
            {
                node nxt=node_cache.query(rt.nxt_leaf);
                nxt.bef_leaf=tmp.id;
                node_cache.modify(nxt.id,nxt);
            }
            memmove(fa.val+ret,fa.val+ret+1,(fa.cnt-ret-2)*sizeof(T));
            memmove(fa.son+ret+1,fa.son+ret+2,(fa.cnt-ret-2)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
    }
    void merge_node(node &tmp,node &fa,int ret)
    {
        node lf,rt;
        if(ret) lf=node_cache.query(fa.son[ret-1]);
        if(ret<fa.cnt-1) rt=node_cache.query(fa.son[ret+1]);
        if(lf.id&&lf.cnt>(B+1)/2)
        {
            memmove(tmp.val+1,tmp.val,(tmp.cnt-1)*sizeof(T));
            memmove(tmp.son+1,tmp.son,tmp.cnt*sizeof(int));
            tmp.val[0]=fa.val[ret-1],tmp.son[0]=lf.son[lf.cnt-1];
            fa.val[ret-1]=lf.val[lf.cnt-2];
            tmp.cnt++,lf.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else if(rt.id&&rt.cnt>(B+1)/2)
        {
            tmp.val[tmp.cnt-1]=fa.val[ret],tmp.son[tmp.cnt]=rt.son[0];
            fa.val[ret]=rt.val[0];
            memmove(rt.val,rt.val+1,(rt.cnt-2)*sizeof(T));
            memmove(rt.son,rt.son+1,(rt.cnt-1)*sizeof(int));
            tmp.cnt++,rt.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
        else if(lf.id)
        {
            memmove(tmp.val+lf.cnt,tmp.val,(tmp.cnt-1)*sizeof(T));
            memmove(tmp.son+lf.cnt,tmp.son,tmp.cnt*sizeof(int));
            memcpy(tmp.val,lf.val,(lf.cnt-1)*sizeof(T));
            memcpy(tmp.son,lf.son,lf.cnt*sizeof(int));
            tmp.val[lf.cnt-1]=fa.val[ret-1];
            tmp.cnt+=lf.cnt;
            memmove(fa.val+ret-1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
            memmove(fa.son+ret-1,fa.son+ret,(fa.cnt-ret)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(lf.id,lf);
        }
        else
        {
            memcpy(tmp.val+tmp.cnt,rt.val,(rt.cnt-1)*sizeof(T));
            memcpy(tmp.son+tmp.cnt,rt.son,rt.cnt*sizeof(int));
            tmp.val[tmp.cnt-1]=fa.val[ret];
            tmp.cnt+=rt.cnt;
            memmove(fa.val+ret,fa.val+ret+1,(fa.cnt-ret-2)*sizeof(T));
            memmove(fa.son+ret+1,fa.son+ret+2,(fa.cnt-ret-2)*sizeof(int));
            fa.cnt--;
            node_cache.modify(fa.id,fa);
            node_cache.modify(rt.id,rt);
        }
    }
public:
    multiBPT(const char *ini,const char *dat): node_cache(dat),f1(ini),f2(dat)
    {
        if(f1.empty())
        {
            root=1,ndc=1;
            node tmp;
            tmp.id=1,tmp.cnt=0;
            tmp.is_leaf=true;
            f2.write(1,tmp);
        }
        else
        {
            root=f1.read(1);
            ndc=f1.read(2);
        }
    }
    ~multiBPT()
    {
        f1.write(1,root);
        f1.write(2,ndc);
    }
    void clear()
    {
        node_cache.clear();
        root=1,ndc=1;
        node tmp;
        tmp.id=1,tmp.cnt=0;
        tmp.is_leaf=true;
        f2.write(1,tmp);
    }
    bool empty() {return node_cache.query(root).cnt==0;}
    vector<VALUE> query(const KEY &k)
    {
        vector<VALUE> vec;
        node tmp=find_leaf(root,k);
        if(tmp.cnt==0) return vec;
        int l=0,r=tmp.cnt-1,ret=tmp.cnt;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(k<tmp.val[mid].first||k==tmp.val[mid].first) r=mid-1,ret=mid;
            else l=mid+1;
        }
        if(ret==tmp.cnt) return vec;
        KEY tt=tmp.val[ret].first;
        while(tt==k)
        {
            vec.push_back(tmp.val[ret].second);
            if(ret==tmp.cnt-1)
            {
                int nx=tmp.nxt_leaf;
                if(!nx) return vec;
                tmp=node_cache.query(nx);ret=-1;
            }
            tt=tmp.val[++ret].first;
        }
        return vec;
    }
    node insert(const KEY &k,const VALUE &v,node tmp=node(),node fa=node(),int fret=-1)
    {
        if(!tmp.id) tmp=node_cache.query(root);
        if(tmp.is_leaf)
        {
            T tk(k,v);
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(tk<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(ret&&tk==tmp.val[ret-1]) return fa;
            memmove(tmp.val+ret+1,tmp.val+ret,(tmp.cnt-ret)*sizeof(T));
            tmp.val[ret]=tk,tmp.cnt++;
            if(tmp.cnt==B) split_leaf(tmp,fa,fret);
            node_cache.modify(tmp.id,tmp);
        }
        else
        {
            T tk(k,v);
            int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(tk<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            tmp=insert(k,v,node_cache.query(tmp.son[ret]),tmp,ret);
            if(tmp.cnt==B+1) split_node(tmp,fa,fret);
            node_cache.modify(tmp.id,tmp);
        }
        return fa;
    }
    node erase(const KEY &k,const VALUE &v,node tmp=node(),node fa=node(),int fret=-1)
    {
        if(!tmp.id) tmp=node_cache.query(root);
        if(tmp.is_leaf)
        {
            T tk(k,v);
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(tk<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(!(ret&&tk==tmp.val[ret-1])) return fa;
            memmove(tmp.val+ret-1,tmp.val+ret,(tmp.cnt-ret)*sizeof(T));
            tmp.cnt--;
            if(tmp.id==root)
            {
                node_cache.modify(tmp.id,tmp);
                return fa;
            }
            if(tmp.cnt<B/2) merge_leaf(tmp,fa,fret);
            node_cache.modify(tmp.id,tmp);
        }
        else
        {
            T tk(k,v);
            int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(tk<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            tmp=erase(k,v,node_cache.query(tmp.son[ret]),tmp,ret);
            if(tmp.id==root)
            {
                if(tmp.cnt==1) root=tmp.son[0];
                return fa;
            }
            if(tmp.cnt<(B+1)/2)
            {
                merge_node(tmp,fa,fret);
                node_cache.modify(tmp.id,tmp);
            }
        }
        return fa;
    }
};
}