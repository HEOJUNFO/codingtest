#include <bits/stdc++.h>
using namespace std;
 
// Maximum number of nodes
const int NMAX = 200001;
 
// Global arrays for heavy–light decomposition
int n, C, Q;
vector<int> adj[NMAX];
int parent[NMAX], depth[NMAX], heavy[NMAX], head[NMAX], pos[NMAX], subSize[NMAX];
 
// DFS to compute subtree sizes and the heavy child for each node.
int dfs(int u, int p) {
    parent[u] = p;
    depth[u] = (p == -1 ? 0 : depth[p] + 1);
    subSize[u] = 1;
    heavy[u] = -1;
    int maxSize = 0;
    for (int v : adj[u]) {
        if(v == p) continue;
        int sz = dfs(v, u);
        if(sz > maxSize) {
            maxSize = sz;
            heavy[u] = v;
        }
        subSize[u] += sz;
    }
    return subSize[u];
}
 
int curPos = 1;
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
 
// Segment tree for roads
// We build the segment tree on the heavy–light base array indices [1, n].
// For each node u (except the capital 1) the road from u to parent[u] is stored at position pos[u].
// (For u==1 there is no road; we simply set its initial value to 0.)
 
struct Node {
    int l, r;
    int color; // if the segment is uniformly colored, store that color; otherwise -1.
    int lazy;  // lazy assignment value; -1 means "no lazy update pending".
};
 
vector<Node> segTree;
 
// Build the segment tree for indices L..R. Initially, every road is uncolored (represented as 0).
void buildTree(int idx, int L, int R) {
    segTree[idx].l = L;
    segTree[idx].r = R;
    segTree[idx].lazy = -1; // no pending update
    if(L == R) {
        segTree[idx].color = 0;
        return;
    }
    int mid = (L + R) / 2;
    buildTree(idx*2, L, mid);
    buildTree(idx*2+1, mid+1, R);
    if(segTree[idx*2].color == segTree[idx*2+1].color)
        segTree[idx].color = segTree[idx*2].color;
    else
        segTree[idx].color = -1;
}
 
// Push down lazy assignment from a node to its children.
void pushDown(int idx) {
    if(segTree[idx].lazy != -1) {
        int lazyVal = segTree[idx].lazy;
        int leftIdx = idx*2, rightIdx = idx*2+1;
        segTree[leftIdx].color = lazyVal;
        segTree[leftIdx].lazy = lazyVal;
        segTree[rightIdx].color = lazyVal;
        segTree[rightIdx].lazy = lazyVal;
        segTree[idx].lazy = -1;
    }
}
 
// This function updates the segment tree for the range [ql, qr] (within the segment tree node range)
// with newColor. It accumulates in “changes” every uniform segment that is overwritten,
// as a pair (oldColor, count). (Only segments where the old color is different from the new one matter.)
void update_range(int idx, int ql, int qr, int newColor, vector<pair<int,int>> &changes) {
    int L = segTree[idx].l, R = segTree[idx].r;
    if(qr < L || R < ql) return;
    if(ql <= L && R <= qr && segTree[idx].color != -1) {
        int oldColor = segTree[idx].color;
        int cnt = R - L + 1;
        segTree[idx].color = newColor;
        segTree[idx].lazy = newColor;
        changes.push_back({oldColor, cnt});
        return;
    }
    if(L == R) { // leaf case
        int oldColor = segTree[idx].color;
        segTree[idx].color = newColor;
        segTree[idx].lazy = newColor;
        changes.push_back({oldColor, 1});
        return;
    }
    pushDown(idx);
    update_range(idx*2, ql, qr, newColor, changes);
    update_range(idx*2+1, ql, qr, newColor, changes);
    if(segTree[idx*2].color == segTree[idx*2+1].color)
        segTree[idx].color = segTree[idx*2].color;
    else
        segTree[idx].color = -1;
}
 
// Heavy–light update: update the (unique) path from city u to the capital (city 1)
// with the new color newColor. The update is performed on the segment tree (which is built on the base array).
// All changes (old color and how many roads are updated) are collected in the vector “changes”.
void updatePath(int u, int newColor, vector<pair<int,int>> &changes) {
    // note: if u == 1 then the path is empty (the capital has no parent road)
    if(u == 1) return;
    // while u is not in the same heavy chain as the capital (head[1] is 1)
    while(head[u] != 1) {
        int h = head[u];
        // The nodes from h to u form a contiguous segment in the base array.
        update_range(1, pos[h], pos[u], newColor, changes);
        u = parent[h];
    }
    // Now u is in the heavy chain whose head is 1.
    if(u != 1)
        update_range(1, pos[1] + 1, pos[u], newColor, changes);
}
 
// Global arrays for color counts and frequency:
// colorCount[c] = number of roads (edges) currently painted with color c (for c from 1 to C)
// freq[x] = number of colors that appear on exactly x roads.
vector<int> colorCount; // size C+1 (1-indexed for colors)
vector<int> freq;       // maximum road count is n-1, so allocate size n+1
 
// After an update, process the collected changes (from update_range calls).
// For every road that changed color (if its old color differs from newColor),
// update the global colorCount and the frequency array.
void processChanges(const vector<pair<int,int>> &changes, int newColor) {
    int totalChange = 0;
    unordered_map<int,int> diff;
    for(auto &p : changes) {
        int oldColor = p.first;
        int cnt = p.second;
        if(oldColor == newColor) continue;
        totalChange += cnt;
        // We update only for valid colors (oldColor==0 means “uncolored” so not among the C colors)
        if(oldColor != 0)
            diff[oldColor] += cnt;
    }
    for(auto &entry : diff) {
        int col = entry.first, cnt = entry.second;
        freq[colorCount[col]]--;  // remove one color from the old frequency
        colorCount[col] -= cnt;
        freq[colorCount[col]]++;  // add it to the new frequency
    }
    if(totalChange > 0) {
        freq[colorCount[newColor]]--;
        colorCount[newColor] += totalChange;
        freq[colorCount[newColor]]++;
    }
}
 
// Main function
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n >> C >> Q;
    // Read tree edges (n-1 edges)
    for (int i = 1; i < n; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    // For n==1 there are no roads; we still process queries appropriately.
    if(n == 0) return 0;
 
    // Build heavy–light decomposition (rooting the tree at city 1, the capital)
    dfs(1, -1);
    decompose(1, 1);
 
    // Build the segment tree on indices 1..n.
    segTree.resize(4 * (n + 1));
    buildTree(1, 1, n);
 
    // Initialize global color counts.
    colorCount.assign(C+1, 0);  // colors 1..C; initially every color is used 0 times.
    freq.assign(n+1, 0);
    freq[0] = C;  // All C colors appear 0 times initially.
 
    // Process Q queries.
    // Each query is: u c m
    // (color the path from u to 1 with color c, then output how many colors appear exactly m times)
    for(int i = 0; i < Q; i++){
        int u, c, m;
        cin >> u >> c >> m;
        vector<pair<int,int>> changes;  // will collect (oldColor, count) for all roads updated in this query
        if(u != 1) {
            updatePath(u, c, changes);
            processChanges(changes, c);
        }
        cout << freq[m] << "\n";
    }
 
    return 0;
}
