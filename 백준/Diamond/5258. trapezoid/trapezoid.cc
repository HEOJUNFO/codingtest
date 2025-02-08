#include <bits/stdc++.h>
using namespace std;
 
// modulo constant
const int MOD = 30013;
 
// DP state stored in a node
struct Node {
    int dp;
    int ways;
};
 
// Combine two DP states: choose the one with larger dp,
// and if equal, add their counts modulo MOD.
Node combine(const Node &a, const Node &b) {
    Node res;
    if(a.dp > b.dp) res = a;
    else if(a.dp < b.dp) res = b;
    else { // equal dp values
        res.dp = a.dp;
        res.ways = ( (long long)a.ways + b.ways ) % MOD;
    }
    return res;
}
 
// 1D Fenwick tree (BIT) supporting “max‐with‐tie–sum” queries.
struct Fenw1D {
    int n;
    vector<Node> fenw;
    Fenw1D(int n): n(n), fenw(n+1, {0,0}) {}
 
    // update at position pos with value 'val'
    void update(int pos, const Node &val) {
        for(; pos <= n; pos += pos & -pos)
            fenw[pos] = combine(fenw[pos], val);
    }
    // query maximum over [1, pos]
    Node query(int pos) {
        Node res = {0,0};
        for(; pos > 0; pos -= pos & -pos)
            res = combine(res, fenw[pos]);
        return res;
    }
};
 
// 2D Fenwick tree: first dimension indexed by B (top–right coordinate)
// and second dimension by D (bottom–right coordinate).
struct Fenw2D {
    int n;
    vector<vector<int>> coords; // for each BIT node in first dimension, sorted d-values
    vector<Fenw1D*> fenw; // corresponding 1D BITs
    Fenw2D(int n): n(n) {
        coords.resize(n+1);
        fenw.resize(n+1, nullptr);
    }
    // For an update at first dimension index i, add the second coordinate d.
    void addPoint(int i, int d) {
        for(; i <= n; i += i & -i)
            coords[i].push_back(d);
    }
    // After all addPoint calls, build the 1D BITs.
    void init() {
        for (int i = 1; i <= n; i++){
            sort(coords[i].begin(), coords[i].end());
            coords[i].erase(unique(coords[i].begin(), coords[i].end()), coords[i].end());
            fenw[i] = new Fenw1D((int)coords[i].size());
        }
    }
    // get the 1-indexed position of value d in coords[i]
    int getIndex(int i, int d) {
        int pos = upper_bound(coords[i].begin(), coords[i].end(), d) - coords[i].begin();
        return pos; // pos is the count of numbers <= d (Fenw1D is 1-indexed)
    }
    // update (i,d) with new value val.
    void update(int i, int d, const Node &val) {
        for(; i <= n; i += i & -i) {
            int pos = getIndex(i, d);
            if(pos > 0)
                fenw[i]->update(pos, val);
        }
    }
    // query rectangle: first dimension [1, i] and second dimension all d' <= d.
    Node query(int i, int d) {
        Node res = {0,0};
        for(; i > 0; i -= i & -i) {
            int pos = getIndex(i, d);
            if(pos > 0)
                res = combine(res, fenw[i]->query(pos));
        }
        return res;
    }
};
 
// Structure to store a trapezoid
struct Trap {
    int A, B, C, D; // original coordinates
    int A_comp, B_comp, C_comp, D_comp; // compressed coordinates
};
 
// Main solution
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<Trap> traps(N);
    vector<int> tops;    // will hold A and B values (top–line)
    vector<int> bottoms; // will hold C and D values (bottom–line)
    for (int i = 0; i < N; i++){
        cin >> traps[i].A >> traps[i].B >> traps[i].C >> traps[i].D;
        tops.push_back(traps[i].A);
        tops.push_back(traps[i].B);
        bottoms.push_back(traps[i].C);
        bottoms.push_back(traps[i].D);
    }
    // Coordinate-compress the top (A and B) and bottom (C and D) coordinates.
    sort(tops.begin(), tops.end());
    tops.erase(unique(tops.begin(), tops.end()), tops.end());
    sort(bottoms.begin(), bottoms.end());
    bottoms.erase(unique(bottoms.begin(), bottoms.end()), bottoms.end());
 
    for (int i = 0; i < N; i++){
        traps[i].A_comp = lower_bound(tops.begin(), tops.end(), traps[i].A) - tops.begin() + 1;
        traps[i].B_comp = lower_bound(tops.begin(), tops.end(), traps[i].B) - tops.begin() + 1;
        traps[i].C_comp = lower_bound(bottoms.begin(), bottoms.end(), traps[i].C) - bottoms.begin() + 1;
        traps[i].D_comp = lower_bound(bottoms.begin(), bottoms.end(), traps[i].D) - bottoms.begin() + 1;
    }
 
    // Sort trapezoids by A (top–left) in increasing order.
    sort(traps.begin(), traps.end(), [](const Trap &a, const Trap &b) {
        if(a.A == b.A) return a.C < b.C;
        return a.A < b.A;
    });
 
    // We will do DP: for trapezoid j, we want to get the best dp among trapezoids i
    // with B[i] < A[j] (top condition) and D[i] < C[j] (bottom condition).
    // We build a 2D BIT keyed by (B,D). Note that B is from the top line.
    int sizeB = tops.size(); // possible B coordinates
    Fenw2D fenw2d(sizeB);
 
    // For every trapezoid (each update will be at (B_comp, D_comp)), add that point.
    for (int i = 0; i < N; i++){
        int bidx = traps[i].B_comp;
        for (int j = bidx; j <= sizeB; j += j & -j)
            fenw2d.coords[j].push_back(traps[i].D_comp);
    }
    fenw2d.init();
 
    // dp and ways arrays for each trapezoid.
    vector<int> dp(N,0);
    vector<int> ways(N,0);
 
    int bestGlobal = 0;
    long long waysGlobal = 0;
 
    // Process each trapezoid in increasing order of A.
    for (int i = 0; i < N; i++){
        // We want to consider previous trapezoids with B < A[i] and D < C[i].
        // Thus, query over first-dim indices [1, A_comp - 1] and second-dim up to C_comp - 1.
        int queryB = traps[i].A_comp - 1; 
        int queryD = traps[i].C_comp - 1;
 
        Node queryRes = {0,0};
        if(queryB >= 1 && queryD >= 1)
            queryRes = fenw2d.query(queryB, queryD);
 
        dp[i] = queryRes.dp + 1;
        ways[i] = (queryRes.dp == 0 ? 1 : queryRes.ways) % MOD;
 
        // update the global best answer
        if(dp[i] > bestGlobal) {
            bestGlobal = dp[i];
            waysGlobal = ways[i];
        } else if(dp[i] == bestGlobal) {
            waysGlobal = (waysGlobal + ways[i]) % MOD;
        }
 
        // Now update the BIT at (B_comp, D_comp) with the new state.
        Node newVal = {dp[i], ways[i]};
        fenw2d.update(traps[i].B_comp, traps[i].D_comp, newVal);
    }
 
    cout << bestGlobal << " " << waysGlobal % MOD << "\n";
 
    return 0;
}
