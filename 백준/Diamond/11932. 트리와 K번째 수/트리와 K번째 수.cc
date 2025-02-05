#include <bits/stdc++.h>
using namespace std;
 
// persistent segment tree에 사용될 노드 구조체
struct Node {
    int left, right, cnt;
};
 
// 최대 노드 수 (n 최대 1e5, 각 업데이트 당 O(log n) = 약 18개 노드 생성 → 4e6 정도 안전)
const int MAX_NODE = 4000000;
Node seg[MAX_NODE];
int segSize = 0; // 인덱스 0은 기본(빈) 노드로 사용 (전역 배열은 0으로 초기화됨)
 
// persistent segtree의 업데이트 함수
// 기존 버전(prev)를 복사하여 [l, r] 구간에서 pos 위치에 count를 1증가시킴.
int update(int prev, int l, int r, int pos) {
    int cur = ++segSize;
    seg[cur] = seg[prev];  // 기존 노드 정보를 복사
    seg[cur].cnt++;        // 현재 구간의 count 증가
    if(l == r) return cur;
    int mid = (l + r) >> 1;
    if(pos <= mid)
        seg[cur].left = update(seg[prev].left, l, mid, pos);
    else
        seg[cur].right = update(seg[prev].right, mid+1, r, pos);
    return cur;
}
 
// persistent segtree에서 네 노드를 이용해 [l, r] 구간에서 k번째 수를 찾는 함수
// a = root[u], b = root[v], c = root[lca], d = root[parent(lca)] (lca가 루트이면 d는 0)
int query(int a, int b, int c, int d, int l, int r, int k) {
    if(l == r) return l;
    int mid = (l + r) >> 1;
    int cntLeft = seg[ seg[a].left ].cnt + seg[ seg[b].left ].cnt 
                  - seg[ seg[c].left ].cnt - seg[ seg[d].left ].cnt;
    if(k <= cntLeft)
        return query(seg[a].left, seg[b].left, seg[c].left, seg[d].left, l, mid, k);
    else
        return query(seg[a].right, seg[b].right, seg[c].right, seg[d].right, mid+1, r, k - cntLeft);
}
 
// 최대 정점 수
const int NMAX = 100001;
 
// 트리 관련 전역 변수들
int root[NMAX];             // 각 정점에 대해 persistent segtree의 루트 (루트부터 해당 정점까지의 정보)
vector<int> adj[NMAX];      // 트리의 인접리스트
int par[NMAX];              // DFS 트리에서 부모 정점
int depth[NMAX];            // 각 정점의 깊이 (루트의 깊이는 1)
 
// 가중치 압축 관련 배열
int compWeight[NMAX];       // 각 정점의 압축된 가중치 (1 ~ n 사이의 정수)
int origWeight[NMAX];       // 각 정점의 원래 가중치 (필요 시 사용)
 
// DFS를 통해 persistent segtree 구축 (루트를 1로 가정)
// 각 정점 v에 대해, 부모의 segtree에서 v의 가중치 위치를 업데이트하여 root[v]를 생성함.
void dfs(int v, int parent, int n) {
    par[v] = parent;
    depth[v] = (parent == 0 ? 1 : depth[parent] + 1);
    // v의 persistent segtree = parent's segtree (없으면 0)에서 v의 압축된 가중치 위치 업데이트
    root[v] = update(parent == 0 ? 0 : root[parent], 1, n, compWeight[v]);
    for (int nxt : adj[v]) {
        if(nxt == parent) continue;
        dfs(nxt, v, n);
    }
}
 
// LCA (Lowest Common Ancestor) 처리를 위한 binary lifting 테이블
int up[NMAX][20]; // up[v][i] : 정점 v의 2^i 번째 조상
 
// LCA 테이블 구축: 각 정점의 up[v][0] 은 바로 부모
void buildLCA(int n) {
    for(int i = 1; i <= n; i++){
        up[i][0] = par[i];
    }
    for(int j = 1; j < 20; j++){
        for(int i = 1; i <= n; i++){
            int mid = up[i][j-1];
            up[i][j] = (mid == 0 ? 0 : up[mid][j-1]);
        }
    }
}
 
// 두 정점 a, b의 LCA를 계산하는 함수
int lca_query(int a, int b) {
    if(depth[a] < depth[b]) swap(a, b);
    int d = depth[a] - depth[b];
    for(int i = 0; d; i++){
        if(d & 1)
            a = up[a][i];
        d >>= 1;
    }
    if(a == b) return a;
    for(int i = 19; i >= 0; i--){
        if(up[a][i] != up[b][i]){
            a = up[a][i];
            b = up[b][i];
        }
    }
    return par[a];
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m;
    cin >> n >> m;
    
    // 입력: 각 정점의 가중치 (정점 번호 1부터 n까지)
    vector<int> weights(n+1);
    vector<int> sortedWeights(n+1);
    for (int i = 1; i <= n; i++){
        cin >> weights[i];
        sortedWeights[i] = weights[i];
    }
 
    // 가중치 압축: 정렬 후 각 원소의 순위를 매김 (모두 서로 다른 값임)
    sort(sortedWeights.begin() + 1, sortedWeights.end());
    unordered_map<int,int> compMap;
    for (int i = 1; i <= n; i++){
        compMap[ sortedWeights[i] ] = i;
    }
    for (int i = 1; i <= n; i++){
        compWeight[i] = compMap[weights[i]];
        origWeight[i] = weights[i];
    }
 
    // 트리의 간선 입력 (무방향)
    for (int i = 1; i <= n-1; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    // DFS를 통해 persistent segtree 구축 (루트는 1, 부모가 없으면 0을 사용)
    dfs(1, 0, n);
    
    // LCA 위한 binary lifting 테이블 구축
    buildLCA(n);
 
    // 각 질의 처리
    // 질의: X Y K → X와 Y를 잇는 경로 상에서 K번째 작은 가중치를 출력
    for (int i = 0; i < m; i++){
        int u, v, k;
        cin >> u >> v >> k;
        int l = lca_query(u, v);
        int parentL = par[l]; // lca의 부모 (l이 루트라면 0)
        // 경로에 있는 정점들의 분포는
        //   persistent[u] + persistent[v] - persistent[l] - persistent[parent(l)]
        // 의 형태로 표현됨.
        int pos = query(root[u], root[v], root[l], (parentL == 0 ? 0 : root[parentL]), 1, n, k);
        // pos는 압축된 값이므로, 원래 값은 sortedWeights[pos] (sortedWeights는 1-indexed)
        cout << sortedWeights[pos] << "\n";
    }
 
    return 0;
}
