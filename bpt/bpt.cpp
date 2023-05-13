#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "vector/vector.hpp"

const int N=100003;
template<class T,int B,int CN=16384>
class BPT
{
public:
    int root,ndc;
    std::fstream f1,f2;
    BPT(const char *ini,const char *dat): cache(dat)
    {
        std::ifstream tf(ini,std::ios::in);
        f2.open(dat,std::ios::in|std::ios::out|std::ios::binary);
        if(!tf.good())
        {
            std::ofstream of(ini,std::ios::out);
            root=1,ndc=1;
            node tmp;
            tmp.id=1,tmp.cnt=0;
            tmp.is_leaf=true;
            f2.seekp(sizeof(node));
            f2.write(reinterpret_cast<char*>(&tmp),sizeof(node));
        }
        else tf>>root>>ndc;
        f1.open(ini,std::ios::in|std::ios::out);
    }
    ~BPT()
    {
        f1<<root<<" "<<ndc;
    }
    struct node
    {
        int id,cnt,son[B+2];
        bool is_leaf;int bef_leaf,nxt_leaf;
        T val[B+2];
        node()
        {
            id=cnt=0;
            is_leaf=false;bef_leaf=nxt_leaf=0;
            for(int i=0;i<=B;i++) val[i]=T();
            memset(son,0,sizeof(son));
        }
    };
    class LRU_CACHE
    {
    public:
        struct HASH_MAP
        {
            int bg[N],nx[N],id[N],val[N],w[N],ndc,wt;
            HASH_MAP()
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
        } hashmap;
        std::fstream f;
        node va[CN];
        int hd,tl,siz,bf[CN],nx[CN],pos[CN];
        LRU_CACHE(const char* dat)
        {
            std::ifstream df(dat,std::ios::in|std::ios::binary);
            if(!df.good()) std::ofstream of(dat,std::ios::out|std::ios::binary);
            f.open(dat,std::ios::in|std::ios::out|std::ios::binary);
            hashmap=HASH_MAP();
            for(int i=0;i<CN;i++) va[i]=node(),bf[i]=nx[i]=-1,pos[i]=0;
            hd=tl=-1;
            siz=0;
        }
        ~LRU_CACHE()
        {
            for(int i=0;i<CN;i++) if(pos[i]) write_node(pos[i],va[i]);
        }
        node read_node(int pos)
        {
            node tmp;
            f.seekg(pos*sizeof(node));
            f.read(reinterpret_cast<char*>(&tmp),sizeof(node));
            return tmp;
        }
        void write_node(int pos,node tmp)
        {
            f.seekp(pos*sizeof(node));
            f.write(reinterpret_cast<char*>(&tmp),sizeof(node));
        }
        node query(int x)
        {
            int tx=hashmap.query(x);
            if(tx==-1)
            {
                int now;
                if(siz==CN)
                {
                    hashmap.del(pos[tl],tl);
                    write_node(pos[tl],va[tl]);
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
                pos[now]=x,va[now]=read_node(x);
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
        void modify(int x,node tmp)
        {
            int tx=hashmap.query(x);
            if(tx==-1)
            {
                int now;
                if(siz==CN)
                {
                    hashmap.del(pos[tl],tl);
                    write_node(pos[tl],va[tl]);
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
    } cache;
    node find_leaf(int now,const T &k)
    {
        node tmp=cache.query(now);
        if(tmp.cnt==0) return tmp;
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k.key<tmp.val[mid].key||k.key==tmp.val[mid].key) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(ret==tmp.cnt&&tmp.nxt_leaf) tmp=cache.query(tmp.nxt_leaf);
            return tmp;
        }
        int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(k.key<tmp.val[mid].key||k.key==tmp.val[mid].key) r=mid-1,ret=mid;
            else l=mid+1;
        }
        return find_leaf(tmp.son[ret],k);
    }
    sjtu::vector<T> get_all(const T &k)
    {
        sjtu::vector<T> vec;
        node tmp=find_leaf(root,k);
        if(tmp.cnt==0) return vec;
        int l=0,r=tmp.cnt-1,ret=0;
        while(l<=r)
        {
            int mid=(l+r)>>1;
            if(k.key<tmp.val[mid].key||k.key==tmp.val[mid].key) r=mid-1,ret=mid;
            else l=mid+1;
        }
        T tt=tmp.val[ret];
        if(!(k.key==tt.key)) return vec;
        while(k.key==tt.key)
        {
            vec.push_back(tt);
            if(ret==tmp.cnt-1)
            {
                int nx=tmp.nxt_leaf;
                if(!nx) return vec;
                tmp=cache.query(nx);ret=-1;
            }
            tt=tmp.val[++ret];
        }
        return vec;
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
            node nxt=cache.query(tmp.nxt_leaf);
            nxt.bef_leaf=oth.id;
            cache.modify(nxt.id,nxt);
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
            cache.modify(oth.id,oth);
            cache.modify(root,rt);
            return;
        }
        memmove(fa.val+ret+1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
        memmove(fa.son+ret+2,fa.son+ret+1,(fa.cnt-ret-1)*sizeof(int));
        fa.val[ret]=oth.val[0],fa.son[ret+1]=oth.id;
        fa.cnt++;
        cache.modify(fa.id,fa);
        cache.modify(oth.id,oth);
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
            cache.modify(oth.id,oth);
            cache.modify(root,rt);
            return;
        }
        memmove(fa.val+ret+1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
        memmove(fa.son+ret+2,fa.son+ret+1,(fa.cnt-ret-1)*sizeof(int));
        fa.val[ret]=tmp.val[tmp.cnt-1],fa.son[ret+1]=oth.id;
        fa.cnt++;
        cache.modify(fa.id,fa);
        cache.modify(oth.id,oth);
    }
    void merge_leaf(node &tmp,node &fa,int ret)
    {
        node lf,rt;
        if(ret) lf=cache.query(fa.son[ret-1]);
        if(ret<fa.cnt-1) rt=cache.query(fa.son[ret+1]);
        if(lf.id&&lf.cnt>B/2)
        {
            memmove(tmp.val+1,tmp.val,tmp.cnt*sizeof(T));
            tmp.val[0]=lf.val[lf.cnt-1];
            tmp.cnt++,lf.cnt--;
            fa.val[ret-1]=tmp.val[0];
            cache.modify(fa.id,fa);
            cache.modify(lf.id,lf);
        }
        else if(rt.id&&rt.cnt>B/2)
        {
            tmp.val[tmp.cnt]=rt.val[0];
            memmove(rt.val,rt.val+1,(rt.cnt-1)*sizeof(T));
            tmp.cnt++,rt.cnt--;
            fa.val[ret]=rt.val[0];
            cache.modify(fa.id,fa);
            cache.modify(rt.id,rt);
        }
        else if(lf.id)
        {
            memmove(tmp.val+lf.cnt,tmp.val,tmp.cnt*sizeof(T));
            memcpy(tmp.val,lf.val,lf.cnt*sizeof(T));
            tmp.cnt+=lf.cnt;
            tmp.bef_leaf=lf.bef_leaf;
            if(lf.bef_leaf)
            {
                node bef=cache.query(lf.bef_leaf);
                bef.nxt_leaf=tmp.id;
                cache.modify(bef.id,bef);
            }
            memmove(fa.val+ret-1,fa.val+ret,(fa.cnt-ret-1)*sizeof(T));
            memmove(fa.son+ret-1,fa.son+ret,(fa.cnt-ret)*sizeof(int));
            fa.cnt--;
            cache.modify(fa.id,fa);
            cache.modify(lf.id,lf);
        }
        else
        {
            memcpy(tmp.val+tmp.cnt,rt.val,rt.cnt*sizeof(T));
            tmp.cnt+=rt.cnt;
            tmp.nxt_leaf=rt.nxt_leaf;
            if(rt.nxt_leaf)
            {
                node nxt=cache.query(rt.nxt_leaf);
                nxt.bef_leaf=tmp.id;
                cache.modify(nxt.id,nxt);
            }
            memmove(fa.val+ret,fa.val+ret+1,(fa.cnt-ret-2)*sizeof(T));
            memmove(fa.son+ret+1,fa.son+ret+2,(fa.cnt-ret-2)*sizeof(int));
            fa.cnt--;
            cache.modify(fa.id,fa);
            cache.modify(rt.id,rt);
        }
    }
    void merge_node(node &tmp,node &fa,int ret)
    {
        node lf,rt;
        if(ret) lf=cache.query(fa.son[ret-1]);
        if(ret<fa.cnt-1) rt=cache.query(fa.son[ret+1]);
        if(lf.id&&lf.cnt>(B+1)/2)
        {
            memmove(tmp.val+1,tmp.val,(tmp.cnt-1)*sizeof(T));
            memmove(tmp.son+1,tmp.son,tmp.cnt*sizeof(int));
            tmp.val[0]=fa.val[ret-1],tmp.son[0]=lf.son[lf.cnt-1];
            fa.val[ret-1]=lf.val[lf.cnt-2];
            tmp.cnt++,lf.cnt--;
            cache.modify(fa.id,fa);
            cache.modify(lf.id,lf);
        }
        else if(rt.id&&rt.cnt>(B+1)/2)
        {
            tmp.val[tmp.cnt-1]=fa.val[ret],tmp.son[tmp.cnt]=rt.son[0];
            fa.val[ret]=rt.val[0];
            memmove(rt.val,rt.val+1,(rt.cnt-2)*sizeof(T));
            memmove(rt.son,rt.son+1,(rt.cnt-1)*sizeof(int));
            tmp.cnt++,rt.cnt--;
            cache.modify(fa.id,fa);
            cache.modify(rt.id,rt);
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
            cache.modify(fa.id,fa);
            cache.modify(lf.id,lf);
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
            cache.modify(fa.id,fa);
            cache.modify(rt.id,rt);
        }
    }
    node ins(const T &k,node tmp=node(),node fa=node(),int fret=-1)
    {
        if(!tmp.id) tmp=cache.query(root);
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(ret&&k==tmp.val[ret-1]) return fa;
            memmove(tmp.val+ret+1,tmp.val+ret,(tmp.cnt-ret)*sizeof(T));
            tmp.val[ret]=k,tmp.cnt++;
            if(tmp.cnt==B) split_leaf(tmp,fa,fret);
            cache.modify(tmp.id,tmp);
        }
        else
        {
            int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            tmp=ins(k,cache.query(tmp.son[ret]),tmp,ret);
            if(tmp.cnt==B+1) split_node(tmp,fa,fret);
            cache.modify(tmp.id,tmp);
        }
        return fa;
    }
    node erase(const T &k,node tmp=node(),node fa=node(),int fret=-1)
    {
        if(!tmp.id) tmp=cache.query(root);
        if(tmp.is_leaf)
        {
            int l=0,r=tmp.cnt-1,ret=tmp.cnt;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            if(!(ret&&k==tmp.val[ret-1])) return fa;
            memmove(tmp.val+ret-1,tmp.val+ret,(tmp.cnt-ret)*sizeof(T));
            tmp.cnt--;
            if(tmp.id==root)
            {
                cache.modify(tmp.id,tmp);
                return fa;
            }
            if(tmp.cnt<B/2) merge_leaf(tmp,fa,fret);
            cache.modify(tmp.id,tmp);
        }
        else
        {
            int l=0,r=tmp.cnt-2,ret=tmp.cnt-1;
            while(l<=r)
            {
                int mid=(l+r)>>1;
                if(k<tmp.val[mid]) r=mid-1,ret=mid;
                else l=mid+1;
            }
            tmp=erase(k,cache.query(tmp.son[ret]),tmp,ret);
            if(tmp.id==root)
            {
                if(tmp.cnt==1) root=tmp.son[0];
                return fa;
            }
            if(tmp.cnt<(B+1)/2)
            {
                merge_node(tmp,fa,fret);
                cache.modify(tmp.id,tmp);
            }
        }
        return fa;
    }
};