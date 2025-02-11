#include <bits/stdc++.h>
using namespace std;
 
const int MAXN = 100001;
const int LOGN = 18;
 
int N, M;
int weightArr[MAXN];
vector<int> adj[MAXN];
 
// persistent segment tree node
struct Node {
    int left, right, cnt;
} seg[5000000];
int segIdx = 0;
 
// 각 노드의 persistent segtree root
int root[MAXN];
 
// LCA 및 깊이, 부모 정보
int parent[LOGN][MAXN];
int depth[MAXN];
 
// 좌표 압축을 위한 벡터
vector<int> comp;
 
// 세그먼트 트리 초기화: 구간 [s, e]
int build(int s, int e) {
    int idx = ++segIdx;
    seg[idx].cnt = 0;
    if(s == e) return idx;
    int mid = (s + e) >> 1;
    seg[idx].left = build(s, mid);
    seg[idx].right = build(mid+1, e);
    return idx;
}
 
// 이전 persistent 트리(prev)를 복사하여 pos에 1을 추가하는 함수
int update(int prev, int s, int e, int pos) {
    int idx = ++segIdx;
    seg[idx] = seg[prev]; // 이전 노드를 복사
    seg[idx].cnt++;        // 구간내 빈도 증가
    if(s == e) return idx;
    int mid = (s + e) >> 1;
    if(pos <= mid)
        seg[idx].left = update(seg[prev].left, s, mid, pos);
    else
        seg[idx].right = update(seg[prev].right, mid+1, e, pos);
    return idx;
}
 
// persistent 세그먼트 트리 쿼리 : 네 개의 버전을(두 경로의 합에서 lca와 lca의 부모를 빼는) 이용하여 kth 작은 값을 찾는다.
int query(int u, int v, int l, int pl, int s, int e, int k) {
    if(s == e) return s;
    int mid = (s + e) >> 1;
    int cntLeft = seg[ seg[u].left ].cnt + seg[ seg[v].left ].cnt 
                - seg[ seg[l].left ].cnt - seg[ seg[pl].left ].cnt;
    if(k <= cntLeft)
        return query(seg[u].left, seg[v].left, seg[l].left, seg[pl].left, s, mid, k);
    else
        return query(seg[u].right, seg[v].right, seg[l].right, seg[pl].right, mid+1, e, k - cntLeft);
}
 
// DFS로 persistent segtree와 LCA 전처리를 동시에 수행함.
// par == -1이면 루트임을 의미하며, 이 경우 baseTree (초기 empty 트리)를 업데이트한다.
void dfs(int cur, int par, int baseTree, int compSize) {
    parent[0][cur] = par;
    depth[cur] = (par == -1 ? 0 : depth[par] + 1);
    // 만약 루트라면 이전 트리로 baseTree, 아니면 부모의 persistent 트리 root 사용
    if(par == -1)
        root[cur] = update(baseTree, 1, compSize, 
                           (int)(lower_bound(comp.begin(), comp.end(), weightArr[cur]) - comp.begin()) + 1);
    else
        root[cur] = update(root[par], 1, compSize, 
                           (int)(lower_bound(comp.begin(), comp.end(), weightArr[cur]) - comp.begin()) + 1);
    for(auto nxt : adj[cur]) {
        if(nxt == par) continue;
        dfs(nxt, cur, baseTree, compSize);
    }
}
 
// 전형적인 binary lifting을 이용한 LCA 함수
int lca(int a, int b) {
    if(depth[a] < depth[b]) swap(a, b);
    int d = depth[a] - depth[b];
    for(int i = 0; i < LOGN; i++){
        if(d & (1 << i)) a = parent[i][a];
    }
    if(a == b) return a;
    for(int i = LOGN - 1; i >= 0; i--){
        if(parent[i][a] != parent[i][b]){
            a = parent[i][a];
            b = parent[i][b];
        }
    }
    return parent[0][a];
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    // 정점 1부터 N까지 가중치 입력
    for(int i = 1; i <= N; i++){
        cin >> weightArr[i];
        comp.push_back(weightArr[i]);
    }
    // 좌표 압축
    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());
    int compSize = comp.size();
    
    // 트리의 간선 입력 (무방향 그래프)
    for(int i = 1; i < N; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // persistent segtree의 초기 empty 트리 생성 (모든 리프 0)
    int baseTree = build(1, compSize);
    
    // DFS로 persistent segtree, parent, depth 정보 전처리 (루트를 1번으로 가정)
    dfs(1, -1, baseTree, compSize);
    
    // binary lifting을 위한 parent 테이블 채우기
    for (int i = 1; i < LOGN; i++){
        for (int j = 1; j <= N; j++){
            if(parent[i-1][j] == -1)
                parent[i][j] = -1;
            else
                parent[i][j] = parent[i-1][ parent[i-1][j] ];
        }
    }
    
    // 쿼리 처리
    cin >> M;
    while(M--){
        int u, v, k;
        cin >> u >> v >> k;
        int L = lca(u, v);
        // lca의 부모 persistent 트리: 만약 L이 루트라면 baseTree를 사용
        int pl = (parent[0][L] == -1 ? baseTree : root[parent[0][L]]);
        // query 함수는 네 persistent 트리(root[u], root[v], root[L], root[parent(L)])를 사용해 k번째 작은 값(압축 인덱스)를 반환함
        int idxAns = query(root[u], root[v], root[L], pl, 1, compSize, k);
        cout << comp[idxAns - 1] << "\n";  // 압축 인덱스는 1부터 시작하므로 comp[idxAns-1] 출력
    }
    
    return 0;
}
