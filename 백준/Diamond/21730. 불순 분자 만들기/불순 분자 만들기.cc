#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;
 
const int LEN = 100001;
 
int N, M, Q;
int S[LEN], E[LEN]; // 각 전자 현미경이 감시하는 두 원자
 
// Lazy propagation을 지원하는 구간합 세그먼트 트리
struct SegTree {
    ll tree[LEN * 4] = {0};
    ll lazy[LEN * 4] = {0};
    
    void propagate(int s, int e, int i) {
        if(lazy[i] != 0) {
            tree[i] += lazy[i] * (e - s + 1);
            if(s != e) {
                lazy[i*2] += lazy[i];
                lazy[i*2+1] += lazy[i];
            }
            lazy[i] = 0;
        }
    }
    
    void update(int l, int r, ll d, int s, int e, int i) {
        propagate(s, e, i);
        if(e < l || r < s) return;
        if(l <= s && e <= r) {
            tree[i] += d * (e - s + 1);
            if(s != e) {
                lazy[i*2] += d;
                lazy[i*2+1] += d;
            }
            return;
        }
        int m = (s + e) >> 1;
        update(l, r, d, s, m, i*2);
        update(l, r, d, m+1, e, i*2+1);
        tree[i] = tree[i*2] + tree[i*2+1];
    }
    
    ll query(int l, int r, int s, int e, int i) {
        propagate(s, e, i);
        if(e < l || r < s) return 0;
        if(l <= s && e <= r) return tree[i];
        int m = (s + e) >> 1;
        return query(l, r, s, m, i*2) + query(l, r, m+1, e, i*2+1);
    }
};
 
SegTree segVert, segEdge; // 정점와 간선에 대해 각각 관리 (업데이트 시 둘 다 적용)
 
// 트리 및 HLD 관련 전역 변수
vector<int> graph[LEN];
int parent[LEN], level[LEN], sz[LEN];
int heavy[LEN] = {0};
int order[LEN], ord; // Euler Tour 상의 순서
int chain[LEN], chainTop[LEN], chainSize[LEN], chainIdx; // 각 정점이 속한 체인, 체인별 top 등
 
// 일반 dfs: 각 정점의 서브트리 크기, heavy child 결정, level, parent 설정
int dfs(int u, int p) {
    parent[u] = p;
    level[u] = level[p] + 1;
    sz[u] = 1;
    for (int v : graph[u]) {
        if(v == p) continue;
        int subSize = dfs(v, u);
        if(heavy[u] == 0 || subSize > sz[heavy[u]])
            heavy[u] = v;
        sz[u] += subSize;
    }
    return sz[u];
}
 
// HLD를 위한 dfs: Euler Tour 순서 및 체인 번호 배정
void dfsHLD(int u, int p) {
    order[u] = ++ord;
    chain[u] = chainIdx;
    if(chainSize[chainIdx] == 0)
        chainTop[chainIdx] = u;
    chainSize[chainIdx]++;
    
    if(heavy[u] != 0)
        dfsHLD(heavy[u], u);
    for (int v : graph[u]) {
        if(v == p || v == heavy[u]) continue;
        chainIdx++;
        dfsHLD(v, u);
    }
}
 
// u에서 v로 이동하는 전자 이동 이벤트에 대해, 
// 경로 상의 모든 정점와 간선 구간에 +1 업데이트 (정점 업데이트와 간선 업데이트를 동시에)
void updatePath(int u, int v) {
    while(chain[u] != chain[v]) {
        if(level[ chainTop[ chain[u] ] ] > level[ chainTop[ chain[v] ] ])
            swap(u, v);
        // 체인 상 [chainTop[chain[v]], v] 구간 업데이트
        segVert.update(order[ chainTop[ chain[v] ] ], order[v], 1, 1, N, 1);
        segEdge.update(order[ chainTop[ chain[v] ] ], order[v], 1, 1, N, 1);
        v = parent[ chainTop[ chain[v] ] ];
    }
    if(level[u] > level[v]) swap(u, v);
    // u와 v가 같은 체인에 있을 때 u에서 v까지 업데이트
    // 단, 간선 업데이트는 정점 u를 제외한 구간 (자식 간선만 적용)
    if(u != v)
        segEdge.update(order[ heavy[u] ], order[v], 1, 1, N, 1);
    segVert.update(order[u], order[v], 1, 1, N, 1);
}
 
// 두 정점 u와 v 사이의 경로에 대해, 업데이트한 정점 값의 총합에서 간선 값들을 빼면 
// (겹치는 업데이트가 한 번만 반영되도록) 실제 전자 이동 횟수가 계산됨.
ll queryPath(int u, int v) {
    ll res = 0;
    while(chain[u] != chain[v]) {
        if(level[ chainTop[ chain[u] ] ] > level[ chainTop[ chain[v] ] ])
            swap(u, v);
        res += segVert.query(order[ chainTop[ chain[v] ] ], order[v], 1, N, 1);
        res -= segEdge.query(order[ chainTop[ chain[v] ] ], order[v], 1, N, 1);
        v = parent[ chainTop[ chain[v] ] ];
    }
    if(level[u] > level[v]) swap(u, v);
    res += segVert.query(order[u], order[v], 1, N, 1);
    if(u != v)
        res -= segEdge.query(order[ heavy[u] ], order[v], 1, N, 1);
    return res;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M >> Q;
    for (int i = 1; i < N; i++){
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    dfs(1, 0);
    dfsHLD(1, 0);
    
    for (int i = 1; i <= M; i++){
        cin >> S[i] >> E[i];
    }
    
    while(Q--){
        int t;
        cin >> t;
        if(t == 1){
            int x, y;
            cin >> x >> y;
            updatePath(x, y);
        }
        else if(t == 2){
            int k;
            cin >> k;
            cout << queryPath(S[k], E[k]) << "\n";
        }
    }
    return 0;
}
