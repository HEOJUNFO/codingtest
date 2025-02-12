#include <bits/stdc++.h>
using namespace std;
 
// Node used in the segment tree: holds a candidate fruit type and a “vote count”
struct Node {
    int candidate, count;
};
 
int n, q;
vector<int> fruit;
vector<vector<int>> adj;
 
// Data for heavy–light decomposition
vector<int> parent, depth, heavy, head, pos, siz;
int curPos = 0;
 
// DFS: compute subtree sizes and record heavy child for each vertex.
int dfs(int u, int p) {
    parent[u] = p;
    depth[u] = (p == -1 ? 0 : depth[p] + 1);
    int maxSize = 0;
    siz[u] = 1;
    heavy[u] = -1;
    for (int v : adj[u]) {
        if(v == p) continue;
        int subSize = dfs(v, u);
        if(subSize > maxSize){
            maxSize = subSize;
            heavy[u] = v;
        }
        siz[u] += subSize;
    }
    return siz[u];
}
 
// Decompose the tree into heavy chains. 
void decompose(int u, int h) {
    head[u] = h;
    pos[u] = curPos++;
    if(heavy[u] != -1)
        decompose(heavy[u], h);
    for (int v : adj[u]) {
        if(v == parent[u] || v == heavy[u]) continue;
        decompose(v, v);
    }
}
 
// Segment tree (global vector) to answer queries on the base array.
// Each leaf corresponds to one vertex (in HLD order) with its fruit type.
vector<Node> segTree;
 
// Merge two nodes using the idea of the Boyer–Moore majority vote.
Node mergeNode(const Node &L, const Node &R) {
    if(L.candidate == -1) return R;
    if(R.candidate == -1) return L;
    Node res;
    if(L.candidate == R.candidate) {
        res.candidate = L.candidate;
        res.count = L.count + R.count;
    } else if(L.count > R.count) {
        res.candidate = L.candidate;
        res.count = L.count - R.count;
    } else {
        res.candidate = R.candidate;
        res.count = R.count - L.count;
    }
    return res;
}
 
// Build the segment tree over the base array.
void buildSegTree(vector<int> &base, int idx, int l, int r) {
    if(l == r) {
        segTree[idx] = {base[l], 1};
        return;
    }
    int mid = (l + r) / 2;
    buildSegTree(base, idx*2, l, mid);
    buildSegTree(base, idx*2+1, mid+1, r);
    segTree[idx] = mergeNode(segTree[idx*2], segTree[idx*2+1]);
}
 
// Query the segment tree on interval [ql,qr] in the base array.
Node querySegTree(int idx, int l, int r, int ql, int qr) {
    if(ql > r || qr < l) return {-1, 0};
    if(ql <= l && r <= qr) return segTree[idx];
    int mid = (l + r) / 2;
    Node leftNode = querySegTree(idx*2, l, mid, ql, qr);
    Node rightNode = querySegTree(idx*2+1, mid+1, r, ql, qr);
    return mergeNode(leftNode, rightNode);
}
 
// Query the path from u to v using heavy–light decomposition.
// This function returns a candidate (with a “vote” value) for a majority fruit type.
Node queryPath(int u, int v, int n) {
    Node res = {-1, 0};
    while(head[u] != head[v]) {
        if(depth[head[u]] > depth[head[v]]) {
            Node cur = querySegTree(1, 0, n-1, pos[head[u]], pos[u]);
            res = mergeNode(res, cur);
            u = parent[head[u]];
        } else {
            Node cur = querySegTree(1, 0, n-1, pos[head[v]], pos[v]);
            res = mergeNode(res, cur);
            v = parent[head[v]];
        }
    }
    if(depth[u] > depth[v]) swap(u, v);
    Node cur = querySegTree(1, 0, n-1, pos[u], pos[v]);
    res = mergeNode(res, cur);
    return res;
}
 
// Compute Lowest Common Ancestor (LCA) using heavy–light decomposition.
int lca(int u, int v) {
    while(head[u] != head[v]) {
        if(depth[head[u]] > depth[head[v]])
            u = parent[head[u]];
        else
            v = parent[head[v]];
    }
    return (depth[u] < depth[v] ? u : v);
}
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n >> q;
    fruit.resize(n);
    for (int i = 0; i < n; i++){
        cin >> fruit[i];
    }
    adj.assign(n, vector<int>());
    for (int i = 0; i < n - 1; i++){
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    parent.resize(n); depth.resize(n); heavy.resize(n);
    head.resize(n); pos.resize(n); siz.resize(n);
    dfs(0, -1);
    decompose(0, 0);
 
    // Build the base array: for each vertex u, base[pos[u]] = fruit[u]
    vector<int> base(n);
    for (int u = 0; u < n; u++){
        base[pos[u]] = fruit[u];
    }
 
    segTree.resize(4 * n);
    buildSegTree(base, 1, 0, n - 1);
 
    // For fast frequency queries: for each fruit type (from 1 to n) store all positions (in base order)
    vector<vector<int>> occ(n+1);
    for (int i = 0; i < n; i++){
        occ[ base[i] ].push_back(i);
    }
    // (They are already in increasing order because of how positions are assigned.)
 
    // Process each query.
    for (int i = 0; i < q; i++){
        int u, v;
        cin >> u >> v;
        u--; v--;
        Node candNode = queryPath(u, v, n);
        int candidate = candNode.candidate;
 
        // Compute the path length: use LCA
        int anc = lca(u, v);
        int pathLength = depth[u] + depth[v] - 2 * depth[anc] + 1;
 
        // Count frequency of candidate along the path by decomposing it into heavy–light segments.
        int freq = 0;
        int uu = u, vv = v;
        while(head[uu] != head[vv]){
            if(depth[head[uu]] > depth[head[vv]]) {
                int L = pos[head[uu]], R = pos[uu];
                freq += (int)(upper_bound(occ[candidate].begin(), occ[candidate].end(), R)
                               - lower_bound(occ[candidate].begin(), occ[candidate].end(), L));
                uu = parent[head[uu]];
            } else {
                int L = pos[head[vv]], R = pos[vv];
                freq += (int)(upper_bound(occ[candidate].begin(), occ[candidate].end(), R)
                               - lower_bound(occ[candidate].begin(), occ[candidate].end(), L));
                vv = parent[head[vv]];
            }
        }
        if(depth[uu] > depth[vv]) swap(uu, vv);
        freq += (int)(upper_bound(occ[candidate].begin(), occ[candidate].end(), pos[vv])
                      - lower_bound(occ[candidate].begin(), occ[candidate].end(), pos[uu]));
 
        // Verify if candidate is a strict majority.
        if(freq > pathLength/2)
            cout << candidate << "\n";
        else
            cout << -1 << "\n";
    }
    return 0;
}
