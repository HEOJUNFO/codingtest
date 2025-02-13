#include <bits/stdc++.h>
using namespace std;
 
// LCA structure for system1 (max query)
struct LCA_Max {
    int n, LOG;
    vector<int> depth, parent;
    vector<int> val; // node values: villages (0) and hubs (their y)
    vector<vector<int>> up, dp; // up[v][i]: 2^i-th ancestor; dp[v][i]: max value on the path from v to up[v][i]
 
    LCA_Max(int n) : n(n) {
        LOG = 32 - __builtin_clz(n);
        depth.assign(n, 0);
        parent.assign(n, -1);
        up.assign(n, vector<int>(LOG, -1));
        dp.assign(n, vector<int>(LOG, -1));
    }
    void dfs(const vector<vector<int>> &adj, int v, int p) {
        parent[v] = p;
        for (int nxt : adj[v]) {
            if(nxt==p) continue;
            depth[nxt] = depth[v] + 1;
            dfs(adj, nxt, v);
        }
    }
    void build(const vector<vector<int>> &adj) {
        dfs(adj, 0, -1);
        for (int i = 0; i < n; i++) {
            if(parent[i]==-1) {
                up[i][0] = i;
                dp[i][0] = val[i];
            } else {
                up[i][0] = parent[i];
                dp[i][0] = max(val[i], val[parent[i]]);
            }
        }
        for (int j = 1; j < LOG; j++){
            for (int i = 0; i < n; i++){
                up[i][j] = up[ up[i][j-1] ][j-1];
                dp[i][j] = max(dp[i][j-1], dp[ up[i][j-1] ][j-1]);
            }
        }
    }
    // query returns max value on path from u to v (including both endpoints)
    int query(int u, int v) {
        int res = -1;
        if(depth[u] < depth[v]) swap(u,v);
        res = max(res, val[u]);
        int d = depth[u]-depth[v];
        for (int i = 0; i < LOG; i++){
            if(d & (1<<i)){
                res = max(res, dp[u][i]);
                u = up[u][i];
            }
        }
        res = max(res, val[v]);
        if(u==v) return res;
        for (int i = LOG-1; i>=0; i--){
            if(up[u][i] != up[v][i]){
                res = max(res, dp[u][i]);
                res = max(res, dp[v][i]);
                u = up[u][i];
                v = up[v][i];
            }
        }
        res = max(res, val[u]);
        res = max(res, val[v]);
        if(parent[u]!=-1)
            res = max(res, val[parent[u]]);
        return res;
    }
};
 
// LCA structure for system2 (min query)
struct LCA_Min {
    int n, LOG;
    vector<int> depth, parent;
    vector<int> val; // node values: villages (INF) and hubs (their y, negative)
    vector<vector<int>> up, dp; // up[v][i]: 2^i-th ancestor; dp[v][i]: min value on path from v to up[v][i]
    int INF;
    LCA_Min(int n, int INF): n(n), INF(INF) {
        LOG = 32 - __builtin_clz(n);
        depth.assign(n, 0);
        parent.assign(n, -1);
        up.assign(n, vector<int>(LOG, -1));
        dp.assign(n, vector<int>(LOG, INF));
    }
    void dfs(const vector<vector<int>> &adj, int v, int p) {
        parent[v] = p;
        for (int nxt : adj[v]) {
            if(nxt==p) continue;
            depth[nxt] = depth[v] + 1;
            dfs(adj, nxt, v);
        }
    }
    void build(const vector<vector<int>> &adj) {
        dfs(adj, 0, -1);
        for (int i = 0; i < n; i++){
            if(parent[i]==-1){
                up[i][0] = i;
                dp[i][0] = val[i];
            } else {
                up[i][0] = parent[i];
                dp[i][0] = min(val[i], val[parent[i]]);
            }
        }
        for (int j = 1; j < LOG; j++){
            for (int i = 0; i < n; i++){
                up[i][j] = up[ up[i][j-1] ][j-1];
                dp[i][j] = min(dp[i][j-1], dp[ up[i][j-1] ][j-1]);
            }
        }
    }
    // query returns min value on path from u to v (including endpoints)
    int query(int u, int v) {
        int res = INF;
        if(depth[u] < depth[v]) swap(u,v);
        res = min(res, val[u]);
        int d = depth[u]-depth[v];
        for (int i = 0; i < LOG; i++){
            if(d & (1<<i)){
                res = min(res, dp[u][i]);
                u = up[u][i];
            }
        }
        res = min(res, val[v]);
        if(u==v) return res;
        for (int i = LOG-1; i>=0; i--){
            if(up[u][i]!=up[v][i]){
                res = min(res, dp[u][i]);
                res = min(res, dp[v][i]);
                u = up[u][i];
                v = up[v][i];
            }
        }
        res = min(res, val[u]);
        res = min(res, val[v]);
        if(parent[u]!=-1)
            res = min(res, val[parent[u]]);
        return res;
    }
};
 
// A structure for storing gap info between adjacent villages.
struct Gap {
    int g; // from T1: maximum hub y on path between adjacent villages
    int f; // from T2: minimum hub y on path (always negative)
};
 
// Segment tree for range-minimum on the f-values.
struct SegTree {
    int n;
    vector<int> tree;
    int INF;
    SegTree(int n, int INF): n(n), INF(INF) {
        tree.assign(4*n, INF);
    }
    void build(vector<int> &arr, int idx, int l, int r) {
        if(l==r) {
            tree[idx] = arr[l];
            return;
        }
        int mid=(l+r)/2;
        build(arr, idx*2, l, mid);
        build(arr, idx*2+1, mid+1, r);
        tree[idx] = min(tree[idx*2], tree[idx*2+1]);
    }
    int query(int idx, int l, int r, int ql, int qr) {
        if(ql>r || qr<l) return INF;
        if(ql<=l && r<=qr) return tree[idx];
        int mid = (l+r)/2;
        return min(query(idx*2, l, mid, ql, qr), query(idx*2+1, mid+1, r, ql, qr));
    }
};
 
// Main
// We “re-number” nodes in each system as follows:
// For system1: villages are nodes 0..N-1, hubs are nodes N..N+M1-1.
// For system2: villages are nodes 0..N-1, hubs are nodes N..N+M2-1.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M1, M2, Q;
    cin >> N >> M1 >> M2 >> Q;
 
    // Read system1 hubs.
    int tot1 = N + M1;
    vector<pair<int,int>> hubs1(M1);
    for(int i=0; i<M1; i++){
        int X, Y; cin >> X >> Y;
        hubs1[i] = {X, Y};
    }
    // Build values for system1: villages have value 0; hubs get their y.
    vector<int> T1_val(tot1,0);
    for (int i=0; i<N; i++) T1_val[i] = 0;
    for (int i=0; i<M1; i++){
        T1_val[N+i] = hubs1[i].second;
    }
    // Build system1 tree: there are (N+M1-1) edges.
    vector<vector<int>> T1_adj(tot1);
    int edges1 = N + M1 - 1;
    for (int i=0; i<edges1; i++){
        int T, C, D; cin >> T >> C >> D;
        if(T==1){
            int v = C-1;
            int hub = N + (D-1);
            T1_adj[v].push_back(hub);
            T1_adj[hub].push_back(v);
        } else { // T==2: hub-to-hub edge.
            int hub1 = N + (C-1), hub2 = N + (D-1);
            T1_adj[hub1].push_back(hub2);
            T1_adj[hub2].push_back(hub1);
        }
    }
 
    // Read system2 hubs.
    int tot2 = N + M2;
    // For system2, villages get INF (so that they are never “chosen” in a min query) and hubs get their y (negative).
    int INF_val = 1000000001;
    vector<int> T2_val(tot2, INF_val);
    for (int i=0; i<N; i++) T2_val[i] = INF_val;
    vector<pair<int,int>> hubs2(M2);
    for (int i=0; i<M2; i++){
        int X, Y; cin >> X >> Y;
        hubs2[i] = {X, Y};
        T2_val[N+i] = Y;
    }
    vector<vector<int>> T2_adj(tot2);
    int edges2 = N + M2 - 1;
    for (int i=0; i<edges2; i++){
        int T, C, D; cin >> T >> C >> D;
        if(T==1){
            int v = C-1;
            int hub = N + (D-1);
            T2_adj[v].push_back(hub);
            T2_adj[hub].push_back(v);
        } else {
            int hub1 = N + (C-1), hub2 = N + (D-1);
            T2_adj[hub1].push_back(hub2);
            T2_adj[hub2].push_back(hub1);
        }
    }
 
    // Build LCA structures.
    LCA_Max lca1(tot1);
    lca1.val = T1_val;
    lca1.dfs(T1_adj, 0, -1);
    for (int i=0; i<tot1; i++){
        if(lca1.parent[i]==-1){
            lca1.up[i][0] = i;
            lca1.dp[i][0] = lca1.val[i];
        } else {
            lca1.up[i][0] = lca1.parent[i];
            lca1.dp[i][0] = max(lca1.val[i], lca1.val[lca1.parent[i]]);
        }
    }
    for (int j=1; j<lca1.LOG; j++){
        for (int i=0; i<tot1; i++){
            lca1.up[i][j] = lca1.up[ lca1.up[i][j-1] ][j-1];
            lca1.dp[i][j] = max(lca1.dp[i][j-1], lca1.dp[ lca1.up[i][j-1] ][j-1]);
        }
    }
 
    LCA_Min lca2(tot2, INF_val);
    lca2.val = T2_val;
    lca2.dfs(T2_adj, 0, -1);
    for (int i=0; i<tot2; i++){
        if(lca2.parent[i]==-1){
            lca2.up[i][0] = i;
            lca2.dp[i][0] = lca2.val[i];
        } else {
            lca2.up[i][0] = lca2.parent[i];
            lca2.dp[i][0] = min(lca2.val[i], lca2.val[lca2.parent[i]]);
        }
    }
    for (int j=1; j<lca2.LOG; j++){
        for (int i=0; i<tot2; i++){
            lca2.up[i][j] = lca2.up[ lca2.up[i][j-1] ][j-1];
            lca2.dp[i][j] = min(lca2.dp[i][j-1], lca2.dp[ lca2.up[i][j-1] ][j-1]);
        }
    }
 
    // For each adjacent pair of villages (i and i+1, 0-indexed) compute:
    // g(i) = max value on path in T1; f(i) = min value on path in T2.
    int gapsCount = max(0, N-1);
    vector<Gap> gaps(gapsCount);
    auto queryT1 = [&](int u, int v) -> int {
        int res = -1;
        if(lca1.depth[u] < lca1.depth[v]) swap(u,v);
        res = max(res, lca1.val[u]);
        int d = lca1.depth[u]-lca1.depth[v];
        for (int i=0; i<lca1.LOG; i++){
            if(d & (1<<i)){
                res = max(res, lca1.dp[u][i]);
                u = lca1.up[u][i];
            }
        }
        res = max(res, lca1.val[v]);
        if(u==v) return res;
        for (int i=lca1.LOG-1; i>=0; i--){
            if(lca1.up[u][i]!=lca1.up[v][i]){
                res = max(res, lca1.dp[u][i]);
                res = max(res, lca1.dp[v][i]);
                u = lca1.up[u][i];
                v = lca1.up[v][i];
            }
        }
        res = max(res, lca1.val[u]);
        res = max(res, lca1.val[v]);
        if(lca1.parent[u]!=-1)
            res = max(res, lca1.val[lca1.parent[u]]);
        return res;
    };
    auto queryT2 = [&](int u, int v) -> int {
        int res = INF_val;
        if(lca2.depth[u] < lca2.depth[v]) swap(u,v);
        res = min(res, lca2.val[u]);
        int d = lca2.depth[u]-lca2.depth[v];
        for (int i=0; i<lca2.LOG; i++){
            if(d & (1<<i)){
                res = min(res, lca2.dp[u][i]);
                u = lca2.up[u][i];
            }
        }
        res = min(res, lca2.val[v]);
        if(u==v) return res;
        for (int i=lca2.LOG-1; i>=0; i--){
            if(lca2.up[u][i]!=lca2.up[v][i]){
                res = min(res, lca2.dp[u][i]);
                res = min(res, lca2.dp[v][i]);
                u = lca2.up[u][i];
                v = lca2.up[v][i];
            }
        }
        res = min(res, lca2.val[u]);
        res = min(res, lca2.val[v]);
        if(lca2.parent[u]!=-1)
            res = min(res, lca2.val[lca2.parent[u]]);
        return res;
    };
 
    vector<Gap> sortedGaps;
    for (int i=0; i<gapsCount; i++){
        int g_val = queryT1(i, i+1);
        int f_val = queryT2(i, i+1);
        sortedGaps.push_back({g_val, f_val});
    }
    // sort gaps by g (the T1 threshold)
    sort(sortedGaps.begin(), sortedGaps.end(), [](const Gap &a, const Gap &b){
        return a.g < b.g;
    });
    vector<int> fArr(sortedGaps.size());
    for (size_t i=0; i<sortedGaps.size(); i++){
        fArr[i] = sortedGaps[i].f;
    }
    int segSize = fArr.size();
    SegTree seg(segSize, INF_val);
    if(segSize>0) seg.build(fArr, 1, 0, segSize-1);
 
    // Answer queries:
    // For each query with given A, we need the minimum f among gaps with g > A.
    // (If no such gap exists, system1 already connects all villages so answer = 0.)
    for (int qi=0; qi<Q; qi++){
        int A; cin >> A;
        int lo = 0, hi = segSize;
        while(lo<hi){
            int mid = (lo+hi)/2;
            if(sortedGaps[mid].g > A) hi = mid;
            else lo = mid+1;
        }
        if(lo>=segSize) cout << 0 << "\n";
        else {
            int ans = seg.query(1, 0, segSize-1, lo, segSize-1);
            cout << ans << "\n";
        }
    }
 
    return 0;
}
