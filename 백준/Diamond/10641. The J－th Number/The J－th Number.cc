#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;
 
struct SegTree {
    int n;
    vector<long long> tree, lazy, baseSum;
    SegTree(int n): n(n), tree(4*n,0), lazy(4*n,0), baseSum(4*n,0) {}
    void build(vector<long long>& base, int idx, int l, int r) {
        if(l==r){ baseSum[idx]=base[l]; tree[idx]=0; return; }
        int mid=(l+r)/2;
        build(base, idx*2, l, mid);
        build(base, idx*2+1, mid+1, r);
        baseSum[idx]=baseSum[idx*2]+baseSum[idx*2+1];
        tree[idx]=tree[idx*2]+tree[idx*2+1];
    }
    void init(vector<long long>& base){ build(base,1,0,n-1); }
    void pushDown(int idx, int l, int r){
        if(lazy[idx]){
            int mid=(l+r)/2, L=idx*2, R=idx*2+1;
            lazy[L]+=lazy[idx]; lazy[R]+=lazy[idx];
            tree[L]+= lazy[idx]*baseSum[L];
            tree[R]+= lazy[idx]*baseSum[R];
            lazy[idx]=0;
        }
    }
    void update(int idx, int l, int r, int ql, int qr, int d){
        if(ql>r || qr<l)return;
        if(ql<=l && r<=qr){ tree[idx]+= (long long)d*baseSum[idx]; lazy[idx]+=d; return; }
        pushDown(idx,l,r);
        int mid=(l+r)/2;
        update(idx*2, l, mid, ql, qr, d);
        update(idx*2+1, mid+1, r, ql, qr, d);
        tree[idx]=tree[idx*2]+tree[idx*2+1];
    }
    long long query(int idx, int l, int r, int ql, int qr){
        if(ql>r || qr<l)return 0;
        if(ql<=l && r<=qr)return tree[idx];
        pushDown(idx,l,r);
        int mid=(l+r)/2;
        return query(idx*2, l, mid, ql,qr)+query(idx*2+1, mid+1, r, ql,qr);
    }
    void updateRange(int l, int r, int d){ if(l>r)return; update(1,0,n-1,l,r,d); }
    long long queryRange(int l, int r){ if(l>r)return 0; return query(1,0,n-1,l,r); }
};
 
SegTree *seg = nullptr;
 
struct Update { int L, R, vIndex; };
struct Query { int Lq, Rq, idx; long long j; };
 
vector<Update> updatesGlobal;
vector<long long> sortedVals, ans;
 
bool cmpUpdate(const Update &a, const Update &b){ return a.vIndex<b.vIndex; }
bool cmpUpdateInt(const Update &u, int x){ return u.vIndex<x; }
 
void parallelBS(int lo, int hi, vector<Query>& qs){
    if(qs.empty()) return;
    if(lo==hi){ for(auto &q: qs) ans[q.idx]=sortedVals[lo]; return; }
    int mid=(lo+hi)/2;
    auto st = lower_bound(updatesGlobal.begin(), updatesGlobal.end(), lo, cmpUpdateInt);
    auto ed = lower_bound(updatesGlobal.begin(), updatesGlobal.end(), mid+1, cmpUpdateInt);
    for(auto it = st; it != ed; ++it)
        seg->updateRange(it->L, it->R-1, 1);
    vector<Query> leftQs, rightQs;
    for(auto &q: qs){
        long long cnt = seg->queryRange(q.Lq, q.Rq-1);
        if(cnt>=q.j) leftQs.push_back(q);
        else { q.j-=cnt; rightQs.push_back(q); }
    }
    for(auto it = st; it != ed; ++it)
        seg->updateRange(it->L, it->R-1, -1);
    parallelBS(lo, mid, leftQs);
    parallelBS(mid+1, hi, rightQs);
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, Q; cin >> N >> M >> Q;
    vector<tuple<int,int,int>> upd; upd.reserve(M);
    for(int i=0;i<M;i++){
        int a,b,v; cin >> a >> b >> v;
        upd.push_back({a,b,v});
    }
    vector<tuple<int,int,long long>> qs; qs.reserve(Q);
    for(int i=0;i<Q;i++){
        int x,y; long long j; cin >> x >> y >> j;
        qs.push_back({x,y,j});
    }
    vector<int> coords;
    for(auto &t: upd){ int a,b,v; tie(a,b,v)=t; coords.push_back(a); coords.push_back(b+1); }
    for(auto &t: qs){ int x,y; long long j; tie(x,y,j)=t; coords.push_back(x); coords.push_back(y+1); }
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    int nLeaves = coords.size()-1;
    vector<long long> baseArr(nLeaves);
    for(int i=0;i<nLeaves;i++)
        baseArr[i] = (long long)coords[i+1]-coords[i];
    vector<int> vals; vals.reserve(M);
    for(auto &t: upd){ int a,b,v; tie(a,b,v)=t; vals.push_back(v); }
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    sortedVals.resize(vals.size());
    for(size_t i=0;i<vals.size();i++) sortedVals[i]=vals[i];
    for(auto &t: upd){
        int a,b,v; tie(a,b,v)=t;
        int L = lower_bound(coords.begin(), coords.end(), a)-coords.begin();
        int R = lower_bound(coords.begin(), coords.end(), b+1)-coords.begin();
        int vIndex = lower_bound(vals.begin(), vals.end(), v)-vals.begin();
        updatesGlobal.push_back({L,R,vIndex});
    }
    sort(updatesGlobal.begin(), updatesGlobal.end(), cmpUpdate);
    vector<Query> queryList; queryList.reserve(Q);
    for(int i=0;i<Q;i++){
        int x,y; long long j; tie(x,y,j)=qs[i];
        int Lq = lower_bound(coords.begin(), coords.end(), x)-coords.begin();
        int Rq = lower_bound(coords.begin(), coords.end(), y+1)-coords.begin();
        queryList.push_back({Lq,Rq,i,j});
    }
    ans.resize(Q,0);
    seg = new SegTree(nLeaves);
    seg->init(baseArr);
    parallelBS(0, sortedVals.size()-1, queryList);
    for(int i=0;i<Q;i++) cout << ans[i] << "\n";
    return 0;
}
