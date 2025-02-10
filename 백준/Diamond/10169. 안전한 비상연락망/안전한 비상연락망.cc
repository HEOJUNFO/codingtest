#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
const ll INF = 1LL<<60; // 충분히 큰 값
 
// DSU (Disjoint Set Union) -- MST 구성에 사용
struct DSU {
    int n;
    vector<int> parent, rank;
    DSU(int n): n(n), parent(n), rank(n,0) {
        for (int i=0; i<n; i++) parent[i]=i;
    }
    int find(int a){
        return parent[a] = (parent[a]==a ? a : find(parent[a]));
    }
    bool merge(int a, int b){
        a = find(a), b = find(b);
        if(a==b)return false;
        if(rank[a]<rank[b]) swap(a,b);
        parent[b] = a;
        if(rank[a]==rank[b]) rank[a]++;
        return true;
    }
};
 
// 간선 정보 구조체
struct Edge {
    int u, v;
    ll w;
    int idx;
    bool used; // MST에 사용되었으면 true
};
 
// MST 구성 후 MST 트리 상에서의 간선 정보를 저장할 구조체
struct MSTEdge {
    int to;
    ll w;
    int origIdx; // 원래 입력 간선 번호
};
 
// 전역 MST 트리 (인접리스트)
vector<vector<MSTEdge>> tree;
 
// Heavy-Light Decomposition 관련 전역 변수
int N; // 정점 수
vector<int> parent, depth, heavy, head, pos, subSize;
 
// DFS를 통해 각 정점의 부모, 깊이, 서브트리 크기 및 heavy child 계산
int dfs(int v, int p) {
    parent[v] = p;
    depth[v] = (p==-1 ? 0 : depth[p]+1);
    subSize[v] = 1;
    heavy[v] = -1;
    int maxSize = 0;
    for(auto &edge: tree[v]){
        int to = edge.to;
        if(to==p) continue;
        int sz = dfs(to,v);
        if(sz > maxSize){
            maxSize = sz;
            heavy[v] = to;
        }
        subSize[v] += sz;
    }
    return subSize[v];
}
 
int curPos = 0;
void decompose(int v, int h) {
    head[v] = h;
    pos[v] = curPos++;
    if(heavy[v]!=-1)
        decompose(heavy[v], h);
    for(auto &edge: tree[v]){
        int to = edge.to;
        if(to==parent[v] || to==heavy[v]) continue;
        decompose(to, to);
    }
}
 
// 세그먼트 트리 – 각 원소에 대해 “현재값과 val의 min”으로 업데이트하는 range update를 지원
struct SegTree {
    int n;
    vector<ll> mn, mx, lazy;
    SegTree(int n): n(n) {
        mn.assign(4*n, INF);
        mx.assign(4*n, INF);
        lazy.assign(4*n, INF);
    }
    // lazy값 전파
    void pushDown(int idx, int l, int r){
        if(lazy[idx]==INF)return;
        int mid = (l+r)/2;
        int left = idx*2, right = idx*2+1;
        if(mx[left] > lazy[idx]){
            mn[left] = min(mn[left], lazy[idx]);
            mx[left] = min(mx[left], lazy[idx]);
            lazy[left] = min(lazy[left], lazy[idx]);
        }
        if(mx[right] > lazy[idx]){
            mn[right] = min(mn[right], lazy[idx]);
            mx[right] = min(mx[right], lazy[idx]);
            lazy[right] = min(lazy[right], lazy[idx]);
        }
        lazy[idx] = INF;
    }
    // 구간 [ql,qr]에 대해 각 원소 x에 x = min(x, val) 적용
    void update(int idx, int l, int r, int ql, int qr, ll val){
        if(ql>r || qr<l) return;
        if(ql<=l && r<=qr){
            if(mx[idx] <= val) return;
            if(mn[idx] > val){
                mn[idx] = val;
                mx[idx] = val;
                lazy[idx] = min(lazy[idx], val);
                return;
            }
        }
        pushDown(idx,l,r);
        int mid = (l+r)/2;
        update(idx*2, l, mid, ql,qr,val);
        update(idx*2+1, mid+1, r, ql,qr,val);
        mn[idx] = min(mn[idx*2], mn[idx*2+1]);
        mx[idx] = max(mx[idx*2], mx[idx*2+1]);
    }
    // posQuery 위치의 값을 반환
    ll query(int idx, int l, int r, int posQuery){
        if(l==r)return mn[idx];
        pushDown(idx,l,r);
        int mid = (l+r)/2;
        if(posQuery<=mid)return query(idx*2,l,mid,posQuery);
        else return query(idx*2+1,mid+1,r,posQuery);
    }
    void build(vector<ll>& base, int idx, int l, int r){
        if(l==r){
            mn[idx] = base[l];
            mx[idx] = base[l];
            return;
        }
        int mid = (l+r)/2;
        build(base, idx*2, l, mid);
        build(base, idx*2+1, mid+1, r);
        mn[idx] = min(mn[idx*2], mn[idx*2+1]);
        mx[idx] = max(mx[idx*2], mx[idx*2+1]);
    }
};
 
// 전역 세그먼트 트리 포인터 (후에 HLD 경로 업데이트에 사용)
SegTree* seg = nullptr;
 
// [l,r] 구간 업데이트 (세그먼트 트리)
void segUpdate(int l, int r, ll val, SegTree* segTree, int n) {
    if(l>r)return;
    segTree->update(1, 0, n-1, l, r, val);
}
 
// HLD 업데이트: MST상에서 u와 v를 잇는 경로상의 모든 간선(각 간선는 더 깊은 쪽 정점에 대응)에 대해 candidate값을 min(candidate, val)로 갱신
void hld_update(int u, int v, ll val, int n) {
    while(head[u]!=head[v]){
        if(depth[head[u]] < depth[head[v]]) swap(u,v);
        segUpdate(pos[head[u]], pos[u], val, seg, n);
        u = parent[head[u]];
    }
    if(u==v)return;
    if(depth[u]>depth[v]) swap(u,v);
    segUpdate(pos[u]+1, pos[v], val, seg, n);
}
 
// MST 간선의 비용을 저장 (각 MST 간선은 더 깊은 정점에 대응)
vector<ll> mstEdgeWeight;
 
// 메인 – 전체 알고리즘 수행
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for(int i=0;i<m;i++){
        int u,v; ll w;
        cin >> u >> v >> w;
        u--; v--;
        edges[i] = {u,v,w,i,false};
    }
 
    // Kruskal으로 MST 구성
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){
        return a.w < b.w;
    });
    DSU dsu(n);
    ll baseCost = 0;
    tree.assign(n, vector<MSTEdge>());
    for(auto &e : edges){
        if(dsu.merge(e.u, e.v)){
            e.used = true;
            baseCost += e.w;
            tree[e.u].push_back({e.v, e.w, e.idx});
            tree[e.v].push_back({e.u, e.w, e.idx});
        }
    }
 
    // HLD 준비 (0번을 루트로)
    parent.assign(n, -1);
    depth.assign(n, 0);
    heavy.assign(n, -1);
    head.assign(n, 0);
    pos.assign(n, 0);
    subSize.assign(n, 0);
    dfs(0, -1);
    decompose(0,0);
 
    // base 배열: 각 정점의 pos에 대해, (부모->정점) MST 간선에 대한 후보값 (초기 INF)
    vector<ll> baseArr(n, INF);
    mstEdgeWeight.assign(n, -1);
    for (int v=0; v<n; v++){
        if(v==0) continue;
        // 부모[v]와의 간선의 가중치
        for(auto &ed : tree[parent[v]]){
            if(ed.to==v){
                mstEdgeWeight[v] = ed.w;
                break;
            }
        }
        baseArr[pos[v]] = INF;
    }
    seg = new SegTree(n);
    seg->build(baseArr, 1, 0, n-1);
 
    // MST에 속하지 않는 각 간선에 대해 HLD 경로 업데이트
    for(auto &e : edges){
        if(e.used) continue;
        hld_update(e.u, e.v, e.w, n);
    }
 
    // 각 MST 간선에 대해, (더 깊은 정점 v에 대응) candidate값을 세그먼트 트리에서 얻음
    vector<ll> candidate(n, INF);
    for (int v=1; v<n; v++){
        candidate[v] = seg->query(1, 0, n-1, pos[v]);
    }
 
    // 원래 입력 순서에 맞춰 답을 채움:
    // - MST 간선: 만약 candidate가 INF이면 대체 불가능하므로 -1, 그렇지 않으면 baseCost - 간선비용 + candidate.
    // - 비 MST 간선: baseCost 그대로.
    vector<ll> rep(m, -1);
    for(auto &e : edges){
        if(e.used){
            int u = e.u, v = e.v;
            int deeper = (depth[u]>depth[v] ? u : v);
            ll cand = candidate[deeper];
            if(cand==INF) rep[e.idx] = -1;
            else rep[e.idx] = baseCost - e.w + cand;
        } else {
            rep[e.idx] = baseCost;
        }
    }
 
    for (int i=0; i<m; i++){
        cout << rep[i] << "\n";
    }
 
    return 0;
}
