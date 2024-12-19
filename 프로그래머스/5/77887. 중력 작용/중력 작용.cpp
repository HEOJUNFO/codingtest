#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
using ll=long long;
const int maxv=100'010;
int sz[maxv],dep[maxv],par[maxv],top[maxv],in[maxv],out[maxv],num[maxv];
vector<int> arr[maxv];
ll tree[262241];
int pv;
struct node{
    node* l;
    node* r;
    node* p;
    long long key;
    long long sum;
    long long cnt;
    node(long long k){
        l=r=p=nullptr;
        key=k,sum=k;
        cnt=1;
    }
}*tops[100'010];
void update(node *x){
    x->sum=x->key;
    x->cnt=1;
    if(x->l)x->sum+=x->l->sum,x->cnt+=x->l->cnt;
    if(x->r)x->sum+=x->r->sum,x->cnt+=x->r->cnt;
}

void rotate(node* x){//x to be parent
    node* p=x->p;
    node* b=nullptr;
    if(!p)return;
    if(x==p->l) p->l=b=x->r,x->r=p;
    else p->r=b=x->l,x->l=p;
    x->p=p->p;p->p=x;
    if(b) b->p=p;
    if(x->p){
        if(x->p->l==p)
            x->p->l=x;
        else x->p->r=x;
    }
    update(p);update(x);
}
void splay(node *x,node *g=nullptr){//x to be g's child
    while(x->p!=g){
        node* p=x->p;
        if(p->p==g){rotate(x);break;}
        node* pp = p->p;
        if((p->l==x)^(pp->l==p)) {rotate(x);rotate(x);}
        else {rotate(p);rotate(x);}
    }
}
node* kth(long long k,node* x){
    while(1){
        while(x->l&&x->l->cnt>k)x=x->l;
        if(x->l)k-=x->l->cnt;
        if(!k--)break;
        x=x->r;
    }
    splay(x);
    return x;
}
void update(int n, int s, int e,int l,int r,ll x){
    if(s>r||e<l) return;
    if(l<=s&&e<=r){
        tree[n]+=x;
        return;
    }
    int mid=s+e>>1;
    update(n<<1,s,mid,l,r,x);
    update(n<<1|1,mid+1,e,l,r,x);
}
ll query(int n,int s,int e,int t){
    if(e<t||t<s)return 0;
    if(s==e)return tree[n];
    int mid=s+e>>1;
    return query(n<<1,s,mid,t)+query(n<<1|1,mid+1,e,t)+tree[n];
}
void dfs1(int v=1){
    sz[v]=1;
    for(auto &i:arr[v]){
        if(par[v]==i) continue;
        dep[i]=dep[v]+1;
        par[i]=v;
        dfs1(i);
        sz[v]+=sz[i];
        if(sz[i] > sz[arr[v][0]]||par[v]==arr[v][0]) swap(i,arr[v][0]);
    }
}
void dfs2(int v=1){
    in[v]=++pv;
    num[pv]=v;
    for(auto i:arr[v]){
        if(in[i]) continue;
        top[i]=i==arr[v][0]?top[v]:i;
        dfs2(i);
    }
    out[v]=pv;
}
int val[100'010];
pair<ll,ll> dfs(int n,node* x){
    ll sum=val[n];
    ll zig=val[n];
    node* t=new node(val[n]);
    if(top[n]==n)tops[n]=t;
    else {
        x->r=t;
        t->p=x;
    }
    for(auto i:arr[n]){
        if(i==par[n])continue;
        auto k=dfs(i,t);
        sum+=k.first;
        zig+=k.second;
    }
    update(t);
    update(1,1,pv,in[n],in[n],sum-zig);
    if(top[n]==n)return {sum,0};
    return {sum,zig};
}
void update(int a,ll w){
    tops[top[a]]=kth(dep[a]-dep[top[a]],tops[top[a]]);
    ll d=0;
    while(a != 0){
        int st = top[a];
        int t = par[st];
        node *k;
        if(top[a]!=1){
            tops[top[t]]=kth(dep[t]-dep[top[t]],tops[top[t]]);
            k=new node(tops[top[t]]->key);
            d+=k->key;
        }else{
            k=new node(w);
            d+=w;
        }
        node *r=tops[st]->r;
        node *l=tops[st]->l;
        tops[st]->l=tops[st]->r=nullptr;
        d-=tops[st]->key;
        delete tops[st];
        if(r)r->p=nullptr;
        if(l)l->p=nullptr;
        if(r){
            if(l){
                while(l->r)l=l->r;
                l->r=r;
                r->p=l;
                splay(r);
                tops[st]=r;
            }else{
                tops[st]=r;
            }
            while(r->l)r=r->l;
            r->l=k;
            k->p=r;
            splay(k);
            tops[st]=k;
        }else{
            if(l){
                while(l->l)l=l->l;
                l->l=k;
                k->p=l;
                splay(k);
                tops[st]=k;
            }else{
                tops[st]=k;
            }
        }
        a=par[st];
        if(a!=0)update(1,1,pv,in[top[a]],in[a],d);
    }
}
ll que(int a){
    node* p=kth(dep[a]-dep[top[a]],tops[top[a]]);
    tops[top[a]]=p;
    ll sum=query(1,1,pv,in[a])+p->key;
    if(p->r)sum+=p->r->sum;
    return sum;
}
vector<ll> solution(vector<int> values, vector<vector<int>> edges, vector<vector<int>> queries) {
    vector<ll> ans;
    int n=values.size()+1;
    for(auto i:edges){
        arr[i[0]].push_back(i[1]);
        arr[i[1]].push_back(i[0]);
    }top[1]=1;dfs1(1);dfs2(1);
    int p=0;
    for(auto i:values){
        p++;
        val[p]=i;
    }dfs(1,nullptr);
    for(auto i:queries){
        if(i[1]==-1){
            ans.push_back(que(i[0]));
        }else{
            update(i[0],i[1]);
        }
    }
    return ans;
}
int main(){
    auto k=solution(    {1, 10, 100, 1000, 10000}, {{1, 2}, {1, 3}, {2, 4}, {2, 5}}, {{1, -1}, {2, -1}, {3, -1}, {4, -1}, {5, -1}, {4, 1000}, {1, -1}, {2, -1}, {3, -1}, {4, -1}, {5, -1}, {2, 1}, {1, -1}, {2, -1}, {3, -1}, {4, -1}, {5, -1}});
    for(auto i:k)cout<<i<<'\n';
}

