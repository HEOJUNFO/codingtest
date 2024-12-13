#include <bits/stdc++.h>
using namespace std;

const int MAXN = 40000;
const int LOG = 16; // because 2^16 = 65536 > 40000

int N, M;
struct Edge {
    int to;
    int w;
};
vector<Edge> adj[MAXN+1];

int depth[MAXN+1];
int parent[LOG+1][MAXN+1];
long long dist[MAXN+1];

void dfs(int u, int p, int d, long long w) {
    parent[0][u] = p;
    depth[u] = d;
    dist[u] = w;
    for (auto &nx : adj[u]) {
        if (nx.to == p) continue;
        dfs(nx.to, u, d+1, w + nx.w);
    }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    // Lift u up to the same depth as v
    int diff = depth[u] - depth[v];
    for (int i=0; diff; i++) {
        if (diff & 1) u = parent[i][u];
        diff >>= 1;
    }
    if (u == v) return u;
    // Lift u and v up until their parents are same
    for (int i=LOG; i>=0; i--) {
        if (parent[i][u] != parent[i][v]) {
            u = parent[i][u];
            v = parent[i][v];
        }
    }
    return parent[0][u];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    for (int i=0; i<N-1; i++){
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }
    // root the tree at 1
    dfs(1,0,0,0);

    for (int i=1; i<=LOG; i++){
        for (int v=1; v<=N; v++){
            parent[i][v] = parent[i-1][ parent[i-1][v] ];
        }
    }

    cin >> M;
    while (M--){
        int u,v;
        cin >> u >> v;
        int c = lca(u,v);
        long long ans = dist[u]+dist[v]-2*dist[c];
        cout << ans << "\n";
    }

    return 0;
}
