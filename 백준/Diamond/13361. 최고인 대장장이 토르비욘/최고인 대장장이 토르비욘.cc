#include <bits/stdc++.h>
using namespace std;
 
// This solution is for BOJ 13361 "최고인 대장장이 토르비욘"
// 
// Each plate is a rectangle with sides s and t (with 1 ≤ s ≤ t).
// When welding the plates (without cutting) to form a sword,
// the “width” (one of the two sides) must strictly decrease as we go down.
// A non‐trivial transformation shows that if you “charge” each plate
// with (s+t) and then build a graph on all numbers (the distinct s’s and t’s)
// with an edge for each plate (joining its s and t),
// then the optimal sword length is
//
//     answer = (sum of all (s+t)) – (for each connected component: 
//                   if cyclic then (sum of vertices) 
//                   else (sum of vertices – max(vertex)) ).
 
// We use union–find (disjoint set) on the compressed vertices.
 
struct UF {
    vector<int> parent;
    vector<long long> compSum; // sum of vertex values in component
    vector<long long> compMax; // maximum vertex value in component
    vector<bool> cyclic;      // true if the component contains a cycle
    UF(int n, const vector<long long>& vals) {
        parent.resize(n);
        compSum.resize(n);
        compMax.resize(n);
        cyclic.resize(n, false);
        for (int i = 0; i < n; i++){
            parent[i] = i;
            compSum[i] = vals[i];
            compMax[i] = vals[i];
        }
    }
    int find(int a) {
        return (parent[a] == a ? a : parent[a] = find(parent[a]));
    }
    // merge returns false if a and b are already in the same component
    bool merge(int a, int b) {
        a = find(a); b = find(b);
        if(a == b) return false;
        parent[a] = b;
        compSum[b] += compSum[a];
        compMax[b] = max(compMax[b], compMax[a]);
        cyclic[b] = cyclic[b] || cyclic[a];
        return true;
    }
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n;
    cin >> n;
    vector<pair<long long,long long>> plates(n);
    long long total = 0; // total = sum_{i=1}^{n}(s_i+t_i)
    vector<long long> compVals; // for coordinate compression
    compVals.reserve(2*n);
    for (int i = 0; i < n; i++){
        long long s, t;
        cin >> s >> t;
        plates[i] = {s, t};
        total += s + t;
        compVals.push_back(s);
        compVals.push_back(t);
    }
    sort(compVals.begin(), compVals.end());
    compVals.erase(unique(compVals.begin(), compVals.end()), compVals.end());
    int m = compVals.size();
 
    // Create union-find on the m distinct numbers.
    UF uf(m, compVals);
 
    // For each plate, add an edge connecting s and t.
    // (If an edge connects two vertices already in the same set, mark the component as cyclic.)
    auto idx = [&](long long x) -> int {
        return int(lower_bound(compVals.begin(), compVals.end(), x) - compVals.begin());
    };
    for (int i = 0; i < n; i++){
        int u = idx(plates[i].first);
        int v = idx(plates[i].second);
        if(!uf.merge(u, v)) {
            int rep = uf.find(u);
            uf.cyclic[rep] = true;
        }
    }
 
    // Now for each connected component, subtract:
    //   if cyclic: subtract (sum of vertices)
    //   else (tree): subtract (sum of vertices - max(vertex))
    long long loss = 0;
    for (int i = 0; i < m; i++){
        if(uf.find(i) != i) continue;
        if(uf.cyclic[i])
            loss += uf.compSum[i];
        else
            loss += (uf.compSum[i] - uf.compMax[i]);
    }
 
    long long ans = total - loss;
    cout << ans << "\n";
    return 0;
}
