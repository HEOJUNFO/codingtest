#include <bits/stdc++.h>
using namespace std;
 
// Lazy segment tree for range-add and range-max query.
struct SegTree {
    int n;
    vector<int> tree, lazy;
    SegTree(int n) : n(n) {
        tree.assign(4*n, 0);
        lazy.assign(4*n, 0);
    }
    void pushDown(int idx, int l, int r) {
        if(lazy[idx]!=0){
            int mid = (l+r)/2;
            int left = idx*2, right = idx*2+1;
            tree[left] += lazy[idx];
            tree[right] += lazy[idx];
            lazy[left] += lazy[idx];
            lazy[right] += lazy[idx];
            lazy[idx] = 0;
        }
    }
    void update(int idx, int l, int r, int ql, int qr, int val) {
        if(ql>r || qr<l) return;
        if(ql<=l && r<=qr) {
            tree[idx] += val;
            lazy[idx] += val;
            return;
        }
        pushDown(idx, l, r);
        int mid = (l+r)/2;
        update(idx*2, l, mid, ql, qr, val);
        update(idx*2+1, mid+1, r, ql, qr, val);
        tree[idx] = max(tree[idx*2], tree[idx*2+1]);
    }
    int query(int idx, int l, int r, int ql, int qr) {
        if(ql>r || qr<l) return 0;
        if(ql<=l && r<=qr) return tree[idx];
        pushDown(idx, l, r);
        int mid = (l+r)/2;
        return max(query(idx*2, l, mid, ql, qr), query(idx*2+1, mid+1, r, ql, qr));
    }
    void updateRange(int l, int r, int val) {
        update(1, 0, n-1, l, r, val);
    }
    int queryRange(int l, int r) {
        return query(1, 0, n-1, l, r);
    }
};
 
// Structures for candidate edges.
struct VertEdge {
    int u, v;
    int cost; // difference in y
    int x;
    int y1, y2; // y1 < y2
};
 
struct HorzEdge {
    int u, v;
    int cost; // difference in x
    int y;
    int x1, x2; // x1 < x2
};
 
struct Edge {
    int u, v;
    int cost;
};
 
// DSU for Kruskal MST.
struct DSU {
    vector<int> parent, rank;
    int comp;
    DSU(int n) : parent(n), rank(n,0), comp(n) {
        for(int i=0; i<n; i++){
            parent[i] = i;
        }
    }
    int find(int a) {
        return parent[a] == a ? a : parent[a] = find(parent[a]);
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if(a==b)return false;
        if(rank[a] < rank[b]) swap(a,b);
        parent[b] = a;
        if(rank[a]==rank[b]) rank[a]++;
        comp--;
        return true;
    }
};
 
// Obstacle definition.
struct Obstacle {
    int P, Q, R, S;
};
 
// Events for vertical sweep (for vertical road queries).
struct VertEvent {
    int x;
    int type; // +1 for add, -1 for remove
    int yLow, yHigh;
};
 
// Events for horizontal sweep (for horizontal road queries).
struct HorzEvent {
    int y;
    int type; // +1 add, -1 remove
    int xLow, xHigh;
};
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M, C;
    cin >> N >> M >> C;
    vector<pair<int,int>> towns(N);
    for(int i=0;i<N;i++){
        cin >> towns[i].first >> towns[i].second;
    }
    vector<Obstacle> obstacles(M);
    for (int j = 0; j < M; j++){
        cin >> obstacles[j].P >> obstacles[j].Q >> obstacles[j].R >> obstacles[j].S;
    }
    vector<pair<int,int>> companies(C); // (B, H)
    for (int k = 0; k < C; k++){
        cin >> companies[k].first >> companies[k].second;
    }
 
    // Build candidate vertical edges:
    unordered_map<int, vector<pair<int,int>>> townsByX;
    townsByX.reserve(N*2);
    for (int i = 0; i < N; i++){
        int x = towns[i].first, y = towns[i].second;
        townsByX[x].push_back({y, i});
    }
    vector<VertEdge> vertEdges;
    for (auto &kv : townsByX) {
        auto &vec = kv.second;
        if(vec.size() < 2) continue;
        sort(vec.begin(), vec.end());
        for (int i = 0; i < (int)vec.size()-1; i++){
            int y1 = vec[i].first, y2 = vec[i+1].first;
            VertEdge e;
            e.u = vec[i].second; e.v = vec[i+1].second;
            e.cost = y2 - y1;
            e.x = kv.first;
            e.y1 = y1; e.y2 = y2;
            vertEdges.push_back(e);
        }
    }
 
    // Build candidate horizontal edges:
    unordered_map<int, vector<pair<int,int>>> townsByY;
    townsByY.reserve(N*2);
    for (int i = 0; i < N; i++){
        int x = towns[i].first, y = towns[i].second;
        townsByY[y].push_back({x, i});
    }
    vector<HorzEdge> horzEdges;
    for (auto &kv : townsByY) {
        auto &vec = kv.second;
        if(vec.size() < 2) continue;
        sort(vec.begin(), vec.end());
        for (int i = 0; i < (int)vec.size()-1; i++){
            HorzEdge e;
            e.u = vec[i].second; e.v = vec[i+1].second;
            int x1 = vec[i].first, x2 = vec[i+1].first;
            e.cost = x2 - x1;
            e.y = kv.first;
            e.x1 = x1; e.x2 = x2;
            horzEdges.push_back(e);
        }
    }
 
    // Determine which candidate vertical edges are allowed.
    // We use a sweep-line over x; for each vertical edge (at x) we query a segment tree built on y.
    vector<int> compY;
    for (int j = 0; j < M; j++){
        compY.push_back(obstacles[j].Q);
        compY.push_back(obstacles[j].S);
    }
    for (auto &e : vertEdges) {
        compY.push_back(e.y1);
        compY.push_back(e.y2);
    }
    sort(compY.begin(), compY.end());
    compY.erase(unique(compY.begin(), compY.end()), compY.end());
    auto getYIndex = [&](int y) -> int {
        return int(lower_bound(compY.begin(), compY.end(), y) - compY.begin());
    };
    vector<VertEvent> vEvents;
    vEvents.reserve(M*2);
    for (int j = 0; j < M; j++){
        VertEvent ev;
        ev.x = obstacles[j].P; ev.type = +1;
        ev.yLow = getYIndex(obstacles[j].Q);
        ev.yHigh = getYIndex(obstacles[j].S);
        vEvents.push_back(ev);
 
        ev.x = obstacles[j].R + 1; ev.type = -1;
        vEvents.push_back(ev);
    }
    sort(vEvents.begin(), vEvents.end(), [](const VertEvent &a, const VertEvent &b){
        return a.x < b.x;
    });
    sort(vertEdges.begin(), vertEdges.end(), [](const VertEdge &a, const VertEdge &b){
        return a.x < b.x;
    });
    SegTree segV((int)compY.size());
    vector<Edge> allowedEdges;
    int evPtr = 0;
    for (auto &e : vertEdges) {
        while(evPtr < (int)vEvents.size() && vEvents[evPtr].x <= e.x) {
            segV.updateRange(vEvents[evPtr].yLow, vEvents[evPtr].yHigh, vEvents[evPtr].type);
            evPtr++;
        }
        int ql = getYIndex(e.y1), qr = getYIndex(e.y2);
        if(segV.queryRange(ql, qr) == 0) {
            Edge edge;
            edge.u = e.u; edge.v = e.v; edge.cost = e.cost;
            allowedEdges.push_back(edge);
        }
    }
 
    // Determine allowed horizontal edges.
    vector<int> compX;
    for (int j = 0; j < M; j++){
        compX.push_back(obstacles[j].P);
        compX.push_back(obstacles[j].R);
    }
    for (auto &e : horzEdges) {
        compX.push_back(e.x1);
        compX.push_back(e.x2);
    }
    sort(compX.begin(), compX.end());
    compX.erase(unique(compX.begin(), compX.end()), compX.end());
    auto getXIndex = [&](int x) -> int {
        return int(lower_bound(compX.begin(), compX.end(), x) - compX.begin());
    };
    vector<HorzEvent> hEvents;
    hEvents.reserve(M*2);
    for (int j = 0; j < M; j++){
        HorzEvent ev;
        ev.y = obstacles[j].Q; ev.type = +1;
        ev.xLow = getXIndex(obstacles[j].P);
        ev.xHigh = getXIndex(obstacles[j].R);
        hEvents.push_back(ev);
 
        ev.y = obstacles[j].S + 1; ev.type = -1;
        hEvents.push_back(ev);
    }
    sort(hEvents.begin(), hEvents.end(), [](const HorzEvent &a, const HorzEvent &b){
        return a.y < b.y;
    });
    sort(horzEdges.begin(), horzEdges.end(), [](const HorzEdge &a, const HorzEdge &b){
        return a.y < b.y;
    });
    SegTree segH((int)compX.size());
    evPtr = 0;
    for (auto &e : horzEdges) {
        while(evPtr < (int)hEvents.size() && hEvents[evPtr].y <= e.y) {
            segH.updateRange(hEvents[evPtr].xLow, hEvents[evPtr].xHigh, hEvents[evPtr].type);
            evPtr++;
        }
        int ql = getXIndex(e.x1), qr = getXIndex(e.x2);
        if(segH.queryRange(ql, qr) == 0) {
            Edge edge;
            edge.u = e.u; edge.v = e.v; edge.cost = e.cost;
            allowedEdges.push_back(edge);
        }
    }
 
    // Build the MST (spanning forest) using the allowed edges.
    sort(allowedEdges.begin(), allowedEdges.end(), [](const Edge &a, const Edge &b){
        return a.cost < b.cost;
    });
    DSU dsu(N);
    long long totalMST = 0;
    vector<int> mstEdges; // store costs of MST edges used.
    for (auto &edge : allowedEdges) {
        if(dsu.unite(edge.u, edge.v)) {
            totalMST += edge.cost;
            mstEdges.push_back(edge.cost);
        }
    }
    int r0 = dsu.comp; // base components count
    sort(mstEdges.begin(), mstEdges.end(), greater<int>());
    int mEdges = mstEdges.size();
    vector<long long> prefix(mEdges+1, 0);
    for (int i = 0; i < mEdges; i++){
        prefix[i+1] = prefix[i] + mstEdges[i];
    }
 
    // For each company, decide the minimum total cost.
    // For a company with airport cost A and maximum H airports, if H < r0 output -1.
    // Otherwise, let k0 be the number of MST edges with cost > A.
    // We can remove k = min(k0, H - r0) edges (thus “cutting” the MST) to create extra components.
    // Then answer = totalMST - (sum of removed edges) + A*(r0 + k).
    for (int i = 0; i < C; i++){
        int A = companies[i].first;
        int H = companies[i].second;
        if(H < r0){
            cout << -1 << "\n";
            continue;
        }
        int lo = 0, hi = mEdges;
        while(lo < hi){
            int mid = (lo + hi) / 2;
            if(mstEdges[mid] > A)
                lo = mid+1;
            else
                hi = mid;
        }
        int countGreater = lo;
        int k_remove = min(countGreater, H - r0);
        long long ans = totalMST - prefix[k_remove] + (long long)A * (r0 + k_remove);
        cout << ans << "\n";
    }
    return 0;
}
