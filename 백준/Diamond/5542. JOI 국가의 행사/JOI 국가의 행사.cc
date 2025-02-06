#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
const ll INF = 1LL << 60;
 
// 간선 구조체 : MST 구성 시 사용할 간선 (안전도 = min(d[u], d[v]))
struct Edge {
    int u, v;
    ll weight; // 안전도
};
 
// DSU (Disjoint Set Union) : 크루스칼 알고리즘에 사용
struct DSU {
    vector<int> parent, rank;
    DSU(int n) {
        parent.resize(n+1);
        rank.resize(n+1, 0);
        for (int i = 0; i <= n; i++)
            parent[i] = i;
    }
    int find(int a) {
        if(parent[a] == a) return a;
        return parent[a] = find(parent[a]);
    }
    void merge(int a, int b) {
        a = find(a), b = find(b);
        if(a == b) return;
        if(rank[a] < rank[b]) swap(a, b);
        parent[b] = a;
        if(rank[a] == rank[b]) rank[a]++;
    }
};
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K, Q;
    cin >> N >> M >> K >> Q;
    
    // (1) 입력 및 그래프 구성 – 도로정보 (양방향)
    // Dijkstra용 인접 리스트 (도시번호 1~N)
    vector<vector<pair<int,int>>> graph(N+1);
    // 입력된 도로 정보를 별도 저장 (나중에 MST 구성 시 간선 리스트로 사용)
    vector<tuple<int,int,int>> roads;
    roads.reserve(M);
    for (int i = 0; i < M; i++){
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
        roads.push_back({u, v, w});
    }
    
    // (2) Multi‑source Dijkstra: 축제 도시들을 시작점으로 하여 각 도시의 d[v] (축제까지 최단거리)를 구함
    vector<ll> d(N+1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    vector<bool> isFestival(N+1, false);
    for (int i = 0; i < K; i++){
        int city;
        cin >> city;
        d[city] = 0;
        isFestival[city] = true;
        pq.push({0, city});
    }
    while(!pq.empty()){
        auto [curDist, cur] = pq.top();
        pq.pop();
        if(curDist != d[cur]) continue;
        for(auto &edge : graph[cur]){
            int nxt = edge.first;
            int w = edge.second;
            if(d[nxt] > d[cur] + w){
                d[nxt] = d[cur] + w;
                pq.push({d[nxt], nxt});
            }
        }
    }
    
    // (3) 각 도로를 “간선”으로 변환 – 간선의 안전도는 min(d[u], d[v])
    vector<Edge> edges;
    edges.reserve(M);
    for (int i = 0; i < M; i++){
        int u, v, w;
        tie(u, v, w) = roads[i];
        ll cand = min(d[u], d[v]);
        Edge e;
        e.u = u; e.v = v; e.weight = cand;
        edges.push_back(e);
    }
    // 간선을 안전도 내림차순으로 정렬
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){
        return a.weight > b.weight;
    });
    
    // (4) 크루스칼 알고리즘으로 최대 스패닝 트리(MST) 구성 
    DSU dsu(N);
    // MST를 인접리스트 형태로 저장 (각 간선의 safety 값도 함께)
    vector<vector<pair<int,ll>>> mst(N+1);
    for(auto &e : edges){
        if(dsu.find(e.u) != dsu.find(e.v)){
            dsu.merge(e.u, e.v);
            mst[e.u].push_back({e.v, e.weight});
            mst[e.v].push_back({e.u, e.weight});
        }
    }
    
    // (5) MST에 대해 LCA (Binary Lifting) 전처리 – 임의의 두 정점 s, t 사이 경로의 최소 간선 안전도(=경로의 병목)를 구할 수 있음.
    int LOG = 0;
    while((1 << LOG) <= N) LOG++;
    vector<vector<int>> parent(LOG, vector<int>(N+1, -1));
    // minEdge[i][v] : 정점 v에서 2^i번째 조상까지 가는 경로 상의 최소 간선 안전도
    vector<vector<ll>> minEdge(LOG, vector<ll>(N+1, INF));
    vector<int> depth(N+1, 0);
    
    // DFS로 parent[0] 와 minEdge[0] 채우기. (그래프가 연결되어 있으므로 임의의 루트, 예를 들어 1번 정점에서 시작)
    function<void(int,int)> dfs = [&](int cur, int par) {
        parent[0][cur] = par;
        for(auto &nx : mst[cur]){
            int nxt = nx.first;
            ll w = nx.second;
            if(nxt == par) continue;
            depth[nxt] = depth[cur] + 1;
            minEdge[0][nxt] = w; // cur->nxt 간선 안전도
            dfs(nxt, cur);
        }
    };
    dfs(1, -1);
    
    // Binary lifting 테이블 구성
    for (int i = 1; i < LOG; i++){
        for (int v = 1; v <= N; v++){
            if(parent[i-1][v] != -1){
                parent[i][v] = parent[i-1][parent[i-1][v]];
                minEdge[i][v] = min(minEdge[i-1][v], minEdge[i-1][parent[i-1][v]]);
            }
        }
    }
    
    // 두 정점 u, v 사이의 MST 경로 상 최소 간선 안전도를 구하는 함수 (즉, 축제까지의 최대 최소 거리)
    auto queryPath = [&](int u, int v) -> ll {
        if(u == v) return INF; // (문제에서 s와 t는 서로 다름)
        ll ans = INF;
        if(depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; i++){
            if(diff & (1 << i)){
                ans = min(ans, minEdge[i][u]);
                u = parent[i][u];
            }
        }
        if(u == v) return ans;
        for (int i = LOG-1; i >= 0; i--){
            if(parent[i][u] != parent[i][v]){
                ans = min(ans, minEdge[i][u]);
                ans = min(ans, minEdge[i][v]);
                u = parent[i][u];
                v = parent[i][v];
            }
        }
        ans = min(ans, minEdge[0][u]);
        ans = min(ans, minEdge[0][v]);
        return ans;
    };
    
    // (6) 각 질의에 대해 MST 상의 경로의 최소 간선 안전도가 곧 원하는 값임.
    for (int i = 0; i < Q; i++){
        int s, t;
        cin >> s >> t;
        ll res = queryPath(s, t);
        cout << res << "\n";
    }
    
    return 0;
}
