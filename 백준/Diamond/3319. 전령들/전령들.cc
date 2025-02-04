#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
const ll INF = 9e18;
 
// 압축된 좌표 배열
vector<ll> comp;
 
// 선(Line): f(x) = m*x + b.
struct Line {
    ll m, b;
    Line(ll _m = 0, ll _b = INF): m(_m), b(_b) {}
    inline ll eval(ll x) const { return m * x + b; }
};
 
// LiChao 트리 노드 (정적 메모리 풀 기반)
struct Node {
    int left, right; // 자식 노드 인덱스, -1이면 null
    ll m, b;         // 저장된 선: f(x)= m*x + b.
};
 
// 풀 크기 (입력 크기에 따라 조절)
const int POOL_SIZE = 300000;
vector<Node> pool(POOL_SIZE);
int poolIdx = 0; // 다음에 할당할 인덱스
 
// 새 노드 할당
int newNode(const Line &line) {
    int idx = poolIdx++;
    pool[idx].left = pool[idx].right = -1;
    pool[idx].m = line.m;
    pool[idx].b = line.b;
    return idx;
}
 
// modification 구조체: 롤백을 위해 변경사항 기록
struct Modification {
    int type;   // 0: 노드의 선(line) 수정, 1: left 자식 수정, 2: right 자식 수정
    int node;   // 수정된 노드 인덱스
    ll old_m, old_b; // type 0일 때 원래 선의 m, b
    int old_child;   // type 1,2일 때 원래 자식 인덱스
};
vector<Modification> mods;
 
// 변경 사항 기록 함수
void recordLineMod(int node, ll old_m, ll old_b) {
    mods.push_back({0, node, old_m, old_b, 0});
}
void recordChildMod(int node, int type, int old_child) {
    mods.push_back({type, node, 0, 0, old_child});
}
 
// add_line: 현재 노드 pool[node]가 커버하는 구간 [l, r) 내에 새 선(newline)을 추가
void add_line(int node, int l, int r, Line newline) {
    int mid = (l + r) >> 1;
    // 실제 x값은 comp[l]와 comp[mid]를 사용
    bool leftBetter = newline.eval(comp[l]) < (pool[node].m * comp[l] + pool[node].b);
    bool midBetter  = newline.eval(comp[mid]) < (pool[node].m * comp[mid] + pool[node].b);
    if(midBetter) {
        // swap(newline, Line(pool[node].m, pool[node].b)) 대신 임시 변수 tmp를 사용하여 직접 교환
        Line tmp(pool[node].m, pool[node].b);
        recordLineMod(node, pool[node].m, pool[node].b);
        pool[node].m = newline.m;
        pool[node].b = newline.b;
        newline = tmp;
    }
    if(r - l == 1) return;
    if(leftBetter != midBetter) {
        int &child = pool[node].left;
        if(child == -1) {
            recordChildMod(node, 1, child);
            child = newNode(newline);
            return;
        }
        add_line(child, l, mid, newline);
    } else {
        int &child = pool[node].right;
        if(child == -1) {
            recordChildMod(node, 2, child);
            child = newNode(newline);
            return;
        }
        add_line(child, mid, r, newline);
    }
}
 
// query: 구간 [l, r)에서 압축 좌표 pos에 해당하는 comp[pos]를 x로 평가하여 최소값 반환
ll query(int node, int l, int r, int pos) {
    if(node == -1) return INF;
    int mid = (l + r) >> 1;
    ll cur = pool[node].m * comp[pos] + pool[node].b;
    if(r - l == 1) return cur;
    if(pos < mid)
        return min(cur, query(pool[node].left, l, mid, pos));
    else
        return min(cur, query(pool[node].right, mid, r, pos));
}
 
// rollback: 변경 기록을 mods.size()가 sz가 될 때까지 롤백
void rollback(int sz) {
    while((int)mods.size() > sz) {
        Modification mod = mods.back();
        mods.pop_back();
        if(mod.type == 0) {
            pool[mod.node].m = mod.old_m;
            pool[mod.node].b = mod.old_b;
        } else if(mod.type == 1) {
            pool[mod.node].left = mod.old_child;
        } else { // type == 2
            pool[mod.node].right = mod.old_child;
        }
    }
}
 
// --- 트리(도시) 관련 전역 변수
int n;
vector<vector<pair<int,int>>> adj; // adj[u]: {v, weight}
 
// 각 도시 i (1-indexed)에 대해 S, V, dp, dist 값
vector<ll> Sval, Vval, dpVal, distVal;
 
// getIndex: x값을 comp에서의 인덱스로 반환 (좌표 압축)
int getIndex(ll x) {
    return int(lower_bound(comp.begin(), comp.end(), x) - comp.begin());
}
 
// DFS: 현재 도시 cur, 부모 parent. 현재 LiChao 트리에는 경로 상 조상들의 선들이 모두 반영됨.
void dfs(int cur, int parent, int msize, int liChaoRoot) {
    for(auto &edge : adj[cur]) {
        int nxt = edge.first, w = edge.second;
        if(nxt == parent) continue;
        distVal[nxt] = distVal[cur] + w;
        int snap = mods.size();
 
        // dp[nxt] = S[nxt] + V[nxt]*dist[nxt] + min_{j in ancestors} { dp[j] - V[nxt]*dist[j] }
        int pos = getIndex(Vval[nxt]);
        ll best = query(liChaoRoot, 0, msize, pos);
        dpVal[nxt] = Sval[nxt] + Vval[nxt] * distVal[nxt] + best;
 
        // 새 선: f(x) = dp[nxt] - dist[nxt]*x.
        Line newLine(-distVal[nxt], dpVal[nxt]);
        add_line(liChaoRoot, 0, msize, newLine);
 
        dfs(nxt, cur, msize, liChaoRoot);
        rollback(snap);
    }
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n;
    adj.resize(n+1);
    for(int i = 1; i <= n-1; i++){
        int u, v, d;
        cin >> u >> v >> d;
        adj[u].push_back({v, d});
        adj[v].push_back({u, d});
    }
    Sval.resize(n+1, 0);
    Vval.resize(n+1, 0);
    for(int i = 2; i <= n; i++){
        cin >> Sval[i] >> Vval[i];
    }
    dpVal.resize(n+1, 0);
    distVal.resize(n+1, 0);
 
    // 좌표 압축: towns 2..n의 V값들을 모아서 정렬 및 유일화
    vector<ll> Vall;
    for (int i = 2; i <= n; i++) {
        Vall.push_back(Vval[i]);
    }
    sort(Vall.begin(), Vall.end());
    Vall.erase(unique(Vall.begin(), Vall.end()), Vall.end());
    comp = Vall; // comp에 압축된 좌표 저장
    int msize = comp.size();  // 압축된 좌표 개수
 
    // 초기: 수도(1번)의 dp=0, dist=0.
    dpVal[1] = 0; 
    distVal[1] = 0;
    mods.clear();
    poolIdx = 0;
    int liChaoRoot = newNode(Line(0, 0));  // town1의 선: f(x)=0.
 
    dfs(1, 0, msize, liChaoRoot);
 
    // 도시 2부터 n까지 dp값 출력
    for (int i = 2; i <= n; i++){
        cout << dpVal[i] << (i == n ? "\n" : " ");
    }
    return 0;
}
