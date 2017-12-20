#include <bits/stdc++.h>
using namespace std;
#define MAXN 100010
#define MAXT 100010
#define INF 0x3f3f3f3f
#ifdef debug
#define Plog(...) printf(__VA_ARGS__)
#else
#define Plog(...) 
#endif
int n,m;
int troot;
void make_tree(int now,int v);
void make_chain(int now,int v);
void make_splus(int now,int v);
void make_cplus(int now,int v);
struct toptree_node
{
        int ch[2];
        int pnt;
        bool rev;
        int val;
        int sroot;
        int splus,cplus,smake,cmake;
        int csum,ssum;
        int cmx,cmn;
        int smx,smn;
        int siz;
        int dsiz;
}tpt[MAXT];
int stack[MAXT],tops=-1;
int topt=0;
struct SBTree
{
        int L[MAXT],R[MAXT],S[MAXT],V[MAXT];
        int G[MAXT];
        int vmx[MAXT],vmn[MAXT],sum[MAXT],siz[MAXT],pls[MAXT],mkv[MAXT];
        int stack[MAXN],tops;
        int topt;
        SBTree()
        {
                tops=-1;
                topt=0;
                vmx[0]=-INF;
                vmn[0]=INF;
        }
        void update(int now)
        {
                vmx[now]=max(max(tpt[V[now]].cmx,tpt[V[now]].smx),max(vmx[L[now]],vmx[R[now]]));
                vmn[now]=min(min(tpt[V[now]].cmn,tpt[V[now]].smn),min(vmn[L[now]],vmn[R[now]]));
                sum[now]=sum[L[now]]+sum[R[now]]+tpt[V[now]].ssum+tpt[V[now]].csum;
                siz[now]=siz[L[now]]+siz[R[now]]+tpt[V[now]].siz+tpt[V[now]].dsiz;
                S[now]=S[L[now]]+S[R[now]]+1;
        }
        void Tree_make(int now,int v)
        {
                mkv[now]=v;
                sum[now]=v*siz[now];
                pls[now]=0;
                vmx[now]=vmn[now]=v;
        }
        void Tree_plus(int now,int delta)
        {
                vmx[now]+=delta;
                vmn[now]+=delta;
                sum[now]+=delta*siz[now];
                pls[now]+=delta;
        }
        void down(int now)
        {
                if (mkv[now]!=INF)
                {
                        Plog("SBT DOWN %d\n",G[now]);
                        make_tree(V[now],mkv[now]);
                        make_chain(V[now],mkv[now]);
                        if (L[now])Tree_make(L[now],mkv[now]);
                        if (R[now])Tree_make(R[now],mkv[now]);
                        mkv[now]=INF;
                }
                if (pls[now])
                {
                        make_splus(V[now],pls[now]);
                        make_cplus(V[now],pls[now]);
                        if (L[now])Tree_plus(L[now],pls[now]);
                        if (R[now])Tree_plus(R[now],pls[now]);
                        pls[now]=0;
                }
        }
        void l_rotate(int &now)
        {
                down(now);//!!!
                down(R[now]);//!!!
                int t=R[now];
                R[now]=L[t];update(now);
                L[t]=now;update(t);
                now=t;
        }
        void r_rotate(int &now)
        {
                down(now);//!!!
                down(L[now]);//!!!
                int t=L[now];
                L[now]=R[t];update(now);
                R[t]=now;update(t);
                now=t;
        }
        void maintain(int &now)
        {
                if (S[L[L[now]]]>S[R[now]])
                {
                        r_rotate(now);
                        maintain(L[now]);
                        maintain(R[now]);
                        maintain(now);
                }
                if (S[R[R[now]]]>S[L[now]])
                {
                        l_rotate(now);
                        maintain(L[now]);
                        maintain(R[now]);
                        maintain(now);
                }
                if (S[L[R[now]]]>S[L[now]])
                {
                        r_rotate(R[now]);
                        l_rotate(now);
                        maintain(L[now]);
                        maintain(R[now]);
                        maintain(now);
                }
                if (S[R[L[now]]]>S[R[now]])
                {
                        l_rotate(L[now]);
                        r_rotate(now);
                        maintain(R[now]);
                        maintain(L[now]);
                        maintain(now);
                }
        }
        void Insert(int &now,int val,int id)
        {
                if (!now)
                {
                        if (tops==-1)
                                now=++topt;
                        else
                                now=stack[tops--];
                        L[now]=R[now]=0;
                        V[now]=val;
                        G[now]=id;
                        mkv[now]=INF;
                        pls[now]=0;
                        update(now);
                        return ;
                }
                assert(val!=V[now]);
                down(now);
                if (val<V[now])
                        Insert(L[now],val,id);
                else 
                        Insert(R[now],val,id);
                update(now);
                maintain(now);
        }
        void Erase(int &now,int val)
        {
                assert(now);
                down(now);
                if (val==V[now])
                {
                        if (!L[now])
                        {
                                stack[++tops]=now;
                                now=R[now];
                                //if (now)update(now);
                        }else if (!R[now])
                        {
                                stack[++tops]=now;
                                now=L[now];
                                //if (now)update(now);
                        }else
                        {
                                l_rotate(now);
                                Erase(L[now],val);
                                update(now);
                                maintain(now);
                        }
                        return ;
                }
                if (val<V[now])
                        Erase(L[now],val);
                else
                        Erase(R[now],val);
                update(now);
                maintain(now);
        }
        int Find(int &now,int val)
        {
                assert(now);
                if (val==V[now])
                {
                        down(now);
                        return now;
                }
                down(now);
                if (val<V[now])
                        return Find(L[now],val);
                else
                        return Find(R[now],val);
        }
        void Scan(int now)
        {
                if (!now)return ;
                Scan(L[now]);
                printf("%d[%d] ",V[now],mkv[now]);
                Scan(R[now]);
        }
}SBT;
bool is_root(int now)
{
        return !tpt[now].pnt || (tpt[tpt[now].pnt].ch[0]!=now && tpt[tpt[now].pnt].ch[1]!=now);
}
void update(int now)
{
        tpt[now].cmx=max(tpt[now].val,max(tpt[tpt[now].ch[0]].cmx,tpt[tpt[now].ch[1]].cmx));
        tpt[now].cmn=min(tpt[now].val,min(tpt[tpt[now].ch[1]].cmn,tpt[tpt[now].ch[0]].cmn));
        tpt[now].csum=tpt[now].val+tpt[tpt[now].ch[0]].csum+tpt[tpt[now].ch[1]].csum;

        tpt[now].smx=max(SBT.vmx[tpt[now].sroot],
                        max(tpt[tpt[now].ch[0]].smx,tpt[tpt[now].ch[1]].smx));
        tpt[now].smn=min(SBT.vmn[tpt[now].sroot],
                        min(tpt[tpt[now].ch[1]].smn,tpt[tpt[now].ch[0]].smn));
        tpt[now].ssum=SBT.sum[tpt[now].sroot]+
                tpt[tpt[now].ch[0]].ssum+tpt[tpt[now].ch[1]].ssum;

        tpt[now].siz=SBT.siz[tpt[now].sroot]+tpt[tpt[now].ch[0]].siz+tpt[tpt[now].ch[1]].siz;
        tpt[now].dsiz=1+tpt[tpt[now].ch[0]].dsiz+tpt[tpt[now].ch[1]].dsiz;
}
void make_reverse(int now)
{
        tpt[now].rev^=1;
        swap(tpt[now].ch[0],tpt[now].ch[1]);
}
void make_tree(int now,int v)
{
        tpt[now].smake=v;
        tpt[now].splus=0;
        //printf("Make Tree:%d %d\n",now,v);
        if (tpt[now].siz)
        {
                if (tpt[now].sroot)SBT.Tree_make(tpt[now].sroot,v);
                tpt[now].smx=tpt[now].smn=v;
                tpt[now].ssum=v*tpt[now].siz;
        }
}
void make_splus(int now,int v)
{
        tpt[now].splus+=v;
        tpt[now].ssum+=v*tpt[now].siz;
        tpt[now].smx+=v;
        tpt[now].smn+=v;
        if (tpt[now].sroot)
                SBT.Tree_plus(tpt[now].sroot,v);
}
void make_chain(int now,int v)
{
        tpt[now].val=v;
        tpt[now].cmake=v;
        tpt[now].cplus=0;
        tpt[now].cmx=tpt[now].cmn=v;
        tpt[now].csum=v*tpt[now].dsiz;
}
void make_cplus(int now,int v)
{
        tpt[now].val+=v;
        tpt[now].cplus+=v;
        tpt[now].cmx+=v;
        tpt[now].cmn+=v;
        tpt[now].csum+=v*tpt[now].dsiz;
}

void down(int now)
{
        if (tpt[now].rev)
        {
                if (tpt[now].ch[0])make_reverse(tpt[now].ch[0]);
                if (tpt[now].ch[1])make_reverse(tpt[now].ch[1]);
                tpt[now].rev=0;
        }
        if (tpt[now].smake!=INF)
        {
                if (tpt[now].ch[0])make_tree(tpt[now].ch[0],tpt[now].smake);
                if (tpt[now].ch[1])make_tree(tpt[now].ch[1],tpt[now].smake);
                tpt[now].smake=INF;
        }
        if (tpt[now].splus)
        {
                if (tpt[now].ch[0])make_splus(tpt[now].ch[0],tpt[now].splus);
                if (tpt[now].ch[1])make_splus(tpt[now].ch[1],tpt[now].splus);
                tpt[now].splus=0;
        }
        if (tpt[now].cmake!=INF)
        {
                Plog("TPT DOWN:%d\n",now);
                if (tpt[now].ch[0])make_chain(tpt[now].ch[0],tpt[now].cmake);
                if (tpt[now].ch[1])make_chain(tpt[now].ch[1],tpt[now].cmake);
                tpt[now].cmake=INF;
        }
        if (tpt[now].cplus)
        {
                if (tpt[now].ch[0])make_cplus(tpt[now].ch[0],tpt[now].cplus);
                if (tpt[now].ch[1])make_cplus(tpt[now].ch[1],tpt[now].cplus);
                tpt[now].cplus=0;
        }
}
void rotate(int now)
{
        int p=tpt[now].pnt,anc=tpt[p].pnt;
        int dir=tpt[p].ch[0]==now;
        if (!is_root(p))
                tpt[anc].ch[tpt[anc].ch[1]==p]=now;
        tpt[now].pnt=anc;
        tpt[tpt[now].ch[dir]].pnt=p;
        tpt[p].ch[1-dir]=tpt[now].ch[dir];
        tpt[p].pnt=now;
        tpt[now].ch[dir]=p;
        update(p);
        update(now);
}


int get_prv(int now)
{
        int x=now;
        stack[++tops]=x;
        while (!is_root(x))
        {
                x=tpt[x].pnt;
                stack[++tops]=x;
        }
        while (~tops)down(stack[tops--]);
        if (tpt[now].ch[0])
        {
                now=tpt[now].ch[0];
                down(now);
                while (now && tpt[now].ch[1])
                {
                        now=tpt[now].ch[1];
                        down(now);
                }
                return now;
        }else
        {
                while (now && tpt[tpt[now].pnt].ch[0]==now)
                        now=tpt[now].pnt;
                now=tpt[now].pnt;
                return now;
        }
}
void splay(int now)
{
        int x=now;
        stack[++tops]=x;
        while (!is_root(x))
        {
                x=tpt[x].pnt;
                stack[++tops]=x;
                if (x==troot)troot=now;
        }
        if (tpt[x].pnt)
        {
                int p=tpt[x].pnt;
                SBT.Erase(tpt[p].sroot,x);
        }
        while (~tops)
                down(stack[tops--]);
        while (!is_root(now))
        {
                int p=tpt[now].pnt;
                int anc=tpt[p].pnt;
                if (is_root(p))
                        rotate(now);
                else if ((tpt[anc].ch[0] == p) == (tpt[p].ch[0] == now))
                        rotate(p),rotate(now);
                else
                        rotate(now),rotate(now);
        }
        if (tpt[now].pnt)
        {
                int p=tpt[now].pnt;
                SBT.Insert(tpt[p].sroot,now,p);
        }
}
int access(int now)
{
        int x=now;
        while (x)
        {
                stack[++tops]=x;
                x=tpt[x].pnt;
        }
        while (~tops)
        {
                down(stack[tops]);
                if (tops && is_root(stack[tops-1]))
                        SBT.Find(tpt[stack[tops]].sroot,stack[tops-1]);
                tops--;
        }
        int son=0;
        while (now)
        {
                splay(now);
                if (son)SBT.Erase(tpt[now].sroot,son);
                if (tpt[now].ch[1])SBT.Insert(tpt[now].sroot,tpt[now].ch[1],now);
                tpt[now].ch[1]=son;
                update(now);
                son=now;
                now=tpt[now].pnt;
        }
        return son;
}
void make_root(int now)
{
        troot=now;
        make_reverse(access(now));
}
void link(int x,int y)
{
        make_root(x);
        access(x);
        make_root(y);
        access(y);
        tpt[x].pnt=y;
        tpt[y].ch[1]=x;
        update(y);
}
void tree_make(int now,int val)
{
        access(now);
        int t=get_prv(now);
        if (t)splay(t);
        make_tree(now,val);
        make_chain(now,val);
        if (t)update(t);
}
void chain_make(int x,int y,int val)
{
        make_root(x);
        make_chain(access(y),val);
}
void tree_plus(int now,int val)
{
        access(now);
        splay(now);
        int t=get_prv(now);
        if (t)splay(t);
        make_splus(now,val);
        make_cplus(now,val);
        if (t)update(t);
}
void chain_plus(int x,int y,int val)
{
        make_root(x);
        make_cplus(access(y),val);
}
int chain_sum(int x,int y)
{
        make_root(x);
        return tpt[access(y)].csum;
}
int chain_max(int x,int y)
{
        make_root(x);
        return tpt[access(y)].cmx;
}
int chain_min(int x,int y)
{
        make_root(x);
        return tpt[access(y)].cmn;
}
int tree_sum(int x)
{
        access(x);
        int t=get_prv(x);
        if (t)splay(t);
        return tpt[x].ssum+tpt[x].csum;
}
int tree_max(int x)
{
        access(x);
        int t=get_prv(x);
        if (t)splay(t);
        return max(tpt[x].cmx,tpt[x].smx);
}
int tree_min(int x)
{
        access(x);
        int t=get_prv(x);
        if (t)splay(t);
        return min(tpt[x].cmn,tpt[x].smn);
}
bool same_tree(int x,int y)
{
        while (tpt[x].pnt)x=tpt[x].pnt;
        while (tpt[y].pnt)y=tpt[y].pnt;
        return x==y;
}
void Scan_chain(int now)
{
        if (!now)return ;
        down(now);
        Scan_chain(tpt[now].ch[0]);
        printf("%d ",now);
        Scan_chain(tpt[now].ch[1]);
}
int edge[MAXN][2];
void Print()
{
        printf("-------------------------\n");
        for (int j=1;j<=n;j++)
        {
                printf("Node #%d#\n",j);
                if (tpt[j].smake==16)printf("Taged\n");
                if (!is_root(j))continue;
                printf("Root :%d\n",j);
                printf("TreeA:\n");
                Scan_chain(j);
                printf("\n");
                printf("TreeB:\n");
                SBT.Scan(tpt[j].sroot);
                printf("\n");
                printf("\n");
        }
        printf("\n");
}
int main()
{
        int croot;
        scanf("%d%d",&n,&m);
        tpt[0].cmx=tpt[0].smx=-INF;
        tpt[0].cmn=tpt[0].smn=INF;
        int x,y,z;
        for (int i=1;i<n;i++)
        {
                scanf("%d%d",&x,&y);
                edge[i][0]=x;
                edge[i][1]=y;
        }
        for (int i=1;i<=n;i++)
        {
                scanf("%d",&x);
                tpt[i].val=x;
                update(i);
        }
        for (int i=1;i<n;i++)
                link(edge[i][0],edge[i][1]);
        scanf("%d",&croot);
        int opt;
        int cnt=0;
        for (int i=0;i<m;i++)
        {
                scanf("%d",&opt);
                if (opt==0) //Change node in subtree x value to y
                {
                        scanf("%d%d",&x,&y);
                        make_root(croot);
                        tree_make(x,y);
                }else if (opt==1) // change the root to x
                {
                        scanf("%d",&x);
			croot=x;
                }else if (opt==2) // change every node on simple path x-y value to z  
                {
                        scanf("%d%d%d",&x,&y,&z);
                        chain_make(x,y,z);
                }else if (opt==3) // Find min value of subtree x
                {
                        scanf("%d",&x);
                        make_root(croot);
                        cnt++;
                        printf("%d\n",tree_min(x));
                }else if (opt==4) // find max value of subtree x
                {
                        scanf("%d",&x);
                        make_root(croot);
                        cnt++;
                        printf("%d\n",tree_max(x));
                }else if (opt==5) // Add every node on the subtree x value z
                {
                        scanf("%d%d",&x,&y);
                        make_root(croot);
                        tree_plus(x,y);
                }else if (opt==6) // Add every node on the simple path x-y value z
                {
                        scanf("%d%d%d",&x,&y,&z);
                        chain_plus(x,y,z);
                }else if (opt==7) // Find min value of simple path x-y
                {
                        scanf("%d%d",&x,&y);
                        cnt++;
                        printf("%d\n",chain_min(x,y));
                }else if (opt==8) // Find max value of simple path x-y
                {
                        scanf("%d%d",&x,&y);
                        cnt++;
                        printf("%d\n",chain_max(x,y));
                }else if (opt==9) // Change x's parent to y
                {
                        scanf("%d%d",&x,&y);
                        if (croot==x)continue;
                        make_root(croot);
                        access(x);
                        z=get_prv(x);
                        if (z)
                        {
                                splay(z);
                                assert(tpt[x].pnt==z);
                                assert(tpt[z].ch[1]==x);
                                tpt[z].ch[1]=0;
                                tpt[x].pnt=0;
                                update(z);
                        }
                        if (same_tree(x,y))
                        {
                                link(x,z);
                        }else
                        {
                                link(x,y);
                        }
                }else if (opt==10) // Find sum of simple path x-y
                {
                        scanf("%d %d",&x,&y);
                        cnt++;
                        printf("%d\n",chain_sum(x,y));
                }else if (opt==11) // Find sum of subtree x 
                {
                        scanf("%d",&x);
                        make_root(croot);
                        cnt++;
                        printf("%d\n",tree_sum(x));
                }
                //    Print();
        }
        return 0;
}
