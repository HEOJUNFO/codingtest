#include <bits/stdc++.h>
using namespace std;
 
// 2D Fenwick Tree for maximum queries.
struct Fenw2D {
    int n;
    vector<vector<int>> coords; // For each BIT node, sorted list of b-coordinates.
    vector<vector<int>> tree;   // BIT structure (1-indexed arrays for each node).
    
    Fenw2D(int n) : n(n) {
        coords.resize(n+1);
        tree.resize(n+1);
    }
    
    // Add coordinate b to all BIT nodes that cover index x.
    void add_coord(int x, int b) {
        for(; x <= n; x += x & -x)
            coords[x].push_back(b);
    }
    
    // After adding all coordinates, sort and unique each BIT node list and initialize the tree.
    void init() {
        for (int i = 1; i <= n; i++){
            sort(coords[i].begin(), coords[i].end());
            coords[i].erase(unique(coords[i].begin(), coords[i].end()), coords[i].end());
            tree[i].assign(coords[i].size() + 1, 0);
        }
    }
    
    // Update: for all BIT nodes covering x, update the entry for coordinate b with newVal (if greater).
    void update(int x, int b, int newVal) {
        for(; x <= n; x += x & -x){
            int pos = int(lower_bound(coords[x].begin(), coords[x].end(), b) - coords[x].begin()) + 1;
            for(; pos < (int)tree[x].size(); pos += pos & -pos)
                tree[x][pos] = max(tree[x][pos], newVal);
        }
    }
    
    // Query: get the maximum value for all BIT nodes up to x for coordinates <= b.
    int query(int x, int b) {
        int res = 0;
        for(; x > 0; x -= x & -x){
            int pos = int(upper_bound(coords[x].begin(), coords[x].end(), b) - coords[x].begin());
            int cur = 0;
            for(; pos > 0; pos -= pos & -pos)
                cur = max(cur, tree[x][pos]);
            res = max(res, cur);
        }
        return res;
    }
};
 
// Main solution: given two arrays A and B (prices for the two companies), we wish to choose a subsequence of days 
// (preserving order) such that both A and B are strictly increasing. We compute the maximum length of such subsequence.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        vector<int> A(n), B(n);
        for (int i = 0; i < n; i++){
            cin >> A[i];
        }
        for (int i = 0; i < n; i++){
            cin >> B[i];
        }
        
        // Coordinate compression for A and B.
        vector<int> compA = A, compB = B;
        sort(compA.begin(), compA.end());
        compA.erase(unique(compA.begin(), compA.end()), compA.end());
        sort(compB.begin(), compB.end());
        compB.erase(unique(compB.begin(), compB.end()), compB.end());
        for (int i = 0; i < n; i++){
            A[i] = int(lower_bound(compA.begin(), compA.end(), A[i]) - compA.begin()) + 1;
            B[i] = int(lower_bound(compB.begin(), compB.end(), B[i]) - compB.begin()) + 1;
        }
        int sizeA = compA.size();
        
        // Build the 2D BIT. We add each day’s B coordinate at its A coordinate.
        Fenw2D fenw(sizeA);
        for (int i = 0; i < n; i++){
            fenw.add_coord(A[i], B[i]);
        }
        fenw.init();
        
        int ans = 0;
        // Process days in the original order.
        for (int i = 0; i < n; i++){
            // We query for all days j with A[j] < A[i] and B[j] < B[i].
            int best = fenw.query(A[i] - 1, B[i] - 1);
            int cur = best + 1;
            ans = max(ans, cur);
            // Update BIT at (A[i], B[i]) with the new dp value.
            fenw.update(A[i], B[i], cur);
        }
        
        cout << ans << "\n";
    }
    
    return 0;
}
