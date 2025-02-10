#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
const ll INF = 1e18;
 
// DSU for Kruskal MST
struct DSU {
    vector<int> par;
    DSU(int n) : par(n) {
        for (int i = 0; i < n; i++){
            par[i] = i;
        }
    }
    int find(int a){
        return par[a] = (par[a] == a ? a : find(par[a]));
    }
    bool unite(int a, int b){
        a = find(a); b = find(b);
        if(a == b) return false;
        par[b] = a;
        return true;
    }
};
 
// Structure for edges.
struct Edge {
    int u, v;
    ll w;
    int idx;
    bool used; // true if the edge is in the MST
};
 
// Global variables for heavy–light decomposition
vector<int> parentArr, depthArr, heavy, head, pos;
vector<ll> val; // val[u] = weight of the edge connecting parent[u] to u (for u != root)
int curPos = 0;
 
// MST tree as an adjacency list; each element is (neighbor, weight, originalEdgeIndex)
vector<vector<tuple<int, ll, int>>> tree;
 
// DFS to compute subtree sizes, parent, depth, and determine heavy child.
int dfs(int u, int p) {
    parentArr[u] = p;
    depthArr[u] = (p == -1 ? 0 : depthArr[p] + 1);
    int size = 1, maxSize = 0;
    heavy[u] = -1;
    for(auto &edge : tree[u]){
        int v; ll w; int eidx;
        tie(v, w, eidx) = edge;
        if(v == p) continue;
        val[v] = w; // record the MST edge weight from u to v
        int subSize = dfs(v, u);
        if(subSize > maxSize){
            maxSize = subSize;
            heavy[u] = v;
        }
        size += subSize;
    }
    return size;
}
 
// Decompose the tree: assign head and position (pos) values.
void decompose(int u, int h) {
    head[u] = h;
    pos[u] = curPos++;
    if(heavy[u] != -1)
        decompose(heavy[u], h);
    for(auto &edge : tree[u]){
        int v; ll w; int eidx;
        tie(v, w, eidx) = edge;
        if(v == parentArr[u] || v == heavy[u])
            continue;
        decompose(v, v);
    }
}
 
// Function to get intervals (in terms of positions) corresponding to MST edges on the path from u to v.
// (Recall: in our HLD, every vertex u (except the root) “carries” the MST edge from parent[u] to u at position pos[u].)
vector<pair<int,int>> getPathIntervals(int u, int v) {
    vector<pair<int,int>> intervals;
    while(head[u] != head[v]){
        if(depthArr[ head[u] ] > depthArr[ head[v] ]){
            int L = pos[ head[u] ];
            int R = pos[u];
            if(L == 0) L = 1; // skip the root position (which does not represent an edge)
            if(L <= R)
                intervals.push_back({L, R});
            u = parentArr[ head[u] ];
        } else {
            int L = pos[ head[v] ];
            int R = pos[v];
            if(L == 0) L = 1;
            if(L <= R)
                intervals.push_back({L, R});
            v = parentArr[ head[v] ];
        }
    }
    if(u == v){
        // No interval if u and v are the same.
    } else {
        if(depthArr[u] > depthArr[v])
            swap(u, v);
        int L = pos[u] + 1;
        int R = pos[v];
        if(L <= R)
            intervals.push_back({L, R});
    }
    return intervals;
}
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++){
        int u, v;
        ll w;
        cin >> u >> v >> w;
        u--; v--; // convert to 0–based indexing
        edges[i] = {u, v, w, i, false};
    }
 
    // Compute the MST using Kruskal.
    DSU dsu(n);
    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), [](const Edge &a, const Edge &b){
        return a.w < b.w;
    });
    ll MSTcost = 0;
    int cnt = 0;
    for(auto &e : sortedEdges){
        if(dsu.unite(e.u, e.v)){
            MSTcost += e.w;
            cnt++;
            edges[e.idx].used = true;
        }
    }
    // If the MST does not cover all vertices then no spanning tree exists.
    if(cnt != n - 1){
        for (int i = 0; i < m; i++){
            cout << -1 << "\n";
        }
        return 0;
    }
 
    // Build the MST tree.
    tree.assign(n, vector<tuple<int, ll, int>>());
    for(auto &e : edges){
        if(e.used){
            tree[e.u].push_back({e.v, e.w, e.idx});
            tree[e.v].push_back({e.u, e.w, e.idx});
        }
    }
 
    // Set up arrays for heavy–light decomposition.
    parentArr.assign(n, -1);
    depthArr.assign(n, 0);
    heavy.assign(n, -1);
    head.assign(n, 0);
    pos.assign(n, 0);
    val.assign(n, INF);
    // Choose 0 as the root.
    dfs(0, -1);
    curPos = 0;
    decompose(0, 0);
 
    // For each non–MST edge, use HLD to get the list of intervals (in the pos array)
    // covering the MST–edges along the unique path between its endpoints.
    struct Interval {
        int L, R;
        ll w;
    };
    vector<Interval> intervals;
    // (Each non–MST edge may yield one or more intervals.)
    for(auto &e : edges){
        if(!e.used){
            int u = e.u, v = e.v;
            vector<pair<int,int>> segs = getPathIntervals(u, v);
            for(auto &pr : segs){
                int L = pr.first, R = pr.second;
                if(L <= R)
                    intervals.push_back({L, R, e.w});
            }
        }
    }
    // We now want to “cover” the positions (which represent MST edges) with candidate values.
    // For each position i, we want cand[i] = min { w of non–MST edge with an interval covering i }.
    vector<ll> cand(curPos, INF);
    // Sweep–line method: sort intervals by starting index.
    sort(intervals.begin(), intervals.end(), [](const Interval &a, const Interval &b){
        return a.L < b.L;
    });
    int ptr = 0;
    // We'll use a min–heap keyed by candidate weight; each element is (w, R)
    typedef pair<ll, int> pli;
    priority_queue< pli, vector<pli>, greater<pli> > pq;
    for (int i = 0; i < curPos; i++){
        while(ptr < (int)intervals.size() && intervals[ptr].L == i){
            pq.push({intervals[ptr].w, intervals[ptr].R});
            ptr++;
        }
        while(!pq.empty() && pq.top().second < i)
            pq.pop();
        if(!pq.empty()){
            cand[i] = min(cand[i], pq.top().first);
        }
    }
 
    // For MST edges: each MST edge is “represented” by a vertex u (u ≠ root) at pos[u].
    // We need to know, for that MST edge, what is the replacement candidate.
    // To later output in the original order we build an answer array.
    vector<ll> ans(m, -1);
    // For non–MST edges, removal does not affect the MST; answer is MSTcost.
    for(auto &e : edges){
        if(!e.used)
            ans[e.idx] = MSTcost;
    }
    // For MST edges, we need to know which input edge corresponds to the MST edge connecting parent[u] to u.
    // We can “associate” each vertex (except the root) with the input edge used to connect it.
    vector<int> mstEdgeIdx(curPos, -1);
    for (int u = 0; u < n; u++){
        if(u == 0) continue;
        for(auto &edge : tree[u]){
            int v; ll w; int eidx;
            tie(v, w, eidx) = edge;
            if(v == parentArr[u]){
                mstEdgeIdx[pos[u]] = eidx;
                break;
            }
        }
    }
    // Now, for every vertex u (u ≠ root), if cand[pos[u]] is INF then there was no replacement (the removed MST edge disconnects the graph)
    // otherwise new MST cost = MSTcost - (weight of MST edge from parent[u] to u) + cand[pos[u]]
    for (int u = 1; u < n; u++){
        int eidx = mstEdgeIdx[pos[u]];
        if(eidx < 0) continue;
        if(cand[pos[u]] == INF)
            ans[eidx] = -1;
        else
            ans[eidx] = MSTcost - val[u] + cand[pos[u]];
    }
 
    // Output answers in the original input order.
    for (int i = 0; i < m; i++){
        cout << ans[i] << "\n";
    }
 
    return 0;
}
