#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N, M;
    cin >> N >> M;
    vector<vector<int>> g(N+1);
    vector<int> ind(N+1, 0);
    for(int i=0; i<M; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        ind[v]++;
    }
    queue<int> q;
    vector<int> topo;
    for(int i=1; i<=N; i++) {
        if(ind[i] == 0) q.push(i);
    }
    while(!q.empty()) {
        int u = q.front(); q.pop();
        topo.push_back(u);
        for(int v : g[u]) {
            if(--ind[v] == 0) q.push(v);
        }
    }
    if(topo.size() != (size_t)N) {
        // cycle, but problem says no
        cout << 0 << endl;
        return 0;
    }
    vector<int> pos(N+1);
    for(int i=0; i<N; i++) {
        pos[topo[i]] = i+1;
    }
    const int INF = N+1;
    vector<int> max_par(N+1, -1);
    for(int u=1; u<=N; u++) {
        for(int v : g[u]) {
            max_par[v] = max(max_par[v], pos[u]);
        }
    }
    vector<int> mp(N+1);
    for(int k=1; k<=N; k++) {
        mp[k] = max_par[topo[k-1]];
    }
    vector<int> suffix_min(N+2, INF);
    for(int k=N; k>=1; k--) {
        suffix_min[k] = min(mp[k], suffix_min[k+1]);
    }
    vector<bool> reaches_all(N+1, false);
    for(int i=1; i<=N; i++) {
        int k = pos[i];
        if(suffix_min[k+1] >= k) reaches_all[i] = true;
    }
    vector<int> min_chi(N+1, INF);
    for(int u=1; u<=N; u++) {
        for(int v : g[u]) {
            min_chi[u] = min(min_chi[u], pos[v]);
        }
    }
    vector<int> mc(N+1);
    for(int k=1; k<=N; k++) {
        mc[k] = min_chi[topo[k-1]];
    }
    vector<int> prefix_max(N+2, -1);
    for(int k=1; k<=N; k++) {
        prefix_max[k] = max(mc[k], prefix_max[k-1]);
    }
    vector<bool> reached_all(N+1, false);
    for(int i=1; i<=N; i++) {
        int k = pos[i];
        if(prefix_max[k-1] <= k) reached_all[i] = true;
    }
    vector<int> crit;
    for(int i=1; i<=N; i++) {
        if(reaches_all[i] && reached_all[i]) crit.push_back(i);
    }
    sort(crit.begin(), crit.end());
    cout << crit.size() << endl;
    if(!crit.empty()) {
        for(int x : crit) cout << x << " ";
        cout << endl;
    }
    return 0;
}