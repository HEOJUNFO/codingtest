#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// Global constants for our 2D segment tree.
// We set BASE = 2048 (since n<=2000) so that indices 0..n-1 lie in [0,BASE).
const int BASE = 2048;
const int SIZE = 2 * BASE;  // our 1D tree size in each dimension
// We choose a very small number as our “minus infinity”.
static const ll MINF = -1000000000000000000LL;
 
// We'll store our 2D segtree in a single vector (of size SIZE*SIZE),
// so that element at “row i, column j” is at segtree[i*SIZE + j].
vector<ll> segtree;
 
// Iterative update of a single point (x,y) with new value (taking maximum).
// (x,y) are 0-indexed (they represent the subarray’s start and end indices, respectively).
// This is a standard iterative 2D segtree update.
 
// Update inner tree for fixed row.
void updatePoint(int x, int y, ll val) {
    int i = x + BASE;
    int j = y + BASE;
    segtree[i * SIZE + j] = max(segtree[i * SIZE + j], val);
    // Update column tree (for row i)
    int jj = j;
    while(jj > 1){
        int par = jj >> 1;
        int left = par << 1, right = left | 1;
        segtree[i * SIZE + par] = max(segtree[i * SIZE + left], segtree[i * SIZE + right]);
        jj = par;
    }
    // Update outer tree (rows)
    int ii = i;
    while(ii > 1){
        int par = ii >> 1;
        int sib = (ii & 1) ? (ii - 1) : (ii + 1);
        // update the inner tree at the leaf corresponding to column y:
        int pos = y + BASE;
        segtree[par * SIZE + pos] = max(segtree[ii * SIZE + pos], segtree[sib * SIZE + pos]);
        int temp = pos;
        while(temp > 1){
            int ptemp = temp >> 1;
            int left = ptemp << 1, right = left | 1;
            segtree[par * SIZE + ptemp] = max(segtree[par * SIZE + left], segtree[par * SIZE + right]);
            temp = ptemp;
        }
        ii = par;
    }
}
 
// Query the inner (column) segment tree for fixed row i
// for columns in [l, r] (l,r are 0-indexed in original coordinate, so we add BASE).
ll queryInner(int i, int l, int r) {
    ll res = MINF;
    int L = l + BASE, R = r + BASE + 1; // R is exclusive
    while(L < R){
        if(L & 1){
            res = max(res, segtree[i * SIZE + L]);
            L++;
        }
        if(R & 1){
            R--;
            res = max(res, segtree[i * SIZE + R]);
        }
        L >>= 1; R >>= 1;
    }
    return res;
}
 
// Query the 2D segment tree for rows in [x1, x2] and columns in [y1, y2] (all 0-indexed).
ll query2D(int x1, int x2, int y1, int y2) {
    ll res = MINF;
    int L = x1 + BASE, R = x2 + BASE + 1;
    while(L < R){
        if(L & 1){
            res = max(res, queryInner(L, y1, y2));
            L++;
        }
        if(R & 1){
            R--;
            res = max(res, queryInner(R, y1, y2));
        }
        L >>= 1; R >>= 1;
    }
    return res;
}
 
// Structure for a contiguous subarray “point”.
// For a subarray that (in the original 1-indexed input) starts at index i and ends at j,
// we use 0-indexing here: we store L = i-1 and R = j-1. (Because prefix‐sum differences use p[j]-p[i-1].)
struct Point {
    int L, R;
    ll sum;
};
 
// Structure for queries.
struct Query {
    int S, E; // query interval [S, E] in 0-indexed (original [S,E] with S-1 and E-1)
    ll U;
    int idx;
};
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m;
    cin >> n >> m;
    vector<ll> arr(n);
    for (int i = 0; i < n; i++){
        cin >> arr[i];
    }
    // Build prefix sums p[0..n] with p[0]=0.
    vector<ll> p(n+1, 0);
    for (int i = 1; i <= n; i++){
        p[i] = p[i-1] + arr[i-1];
    }
 
    // Precompute all contiguous subarray sums.
    // A subarray from index i+1 to j (1-indexed) has sum = p[j] - p[i].
    // In our 0-indexed system, the subarray “point” will be at (L, R) = (i, j-1).
    int tot = n*(n+1)/2;
    vector<Point> points;
    points.reserve(tot);
    for (int i = 0; i < n; i++){
        for (int j = i+1; j <= n; j++){
            Point pt;
            pt.L = i; 
            pt.R = j - 1;
            pt.sum = p[j] - p[i];
            points.push_back(pt);
        }
    }
    // Sort points by sum ascending.
    sort(points.begin(), points.end(), [](const Point &a, const Point &b){
        return a.sum < b.sum;
    });
 
    // Read queries.
    vector<Query> queries(m);
    for (int i = 0; i < m; i++){
        int S, E;
        ll U;
        cin >> S >> E >> U;
        // Convert S,E from 1-indexed to 0-indexed.
        Query q;
        q.S = S - 1;
        q.E = E - 1;
        q.U = U;
        q.idx = i;
        queries[i] = q;
    }
    // Sort queries by U ascending.
    sort(queries.begin(), queries.end(), [](const Query &a, const Query &b){
        return a.U < b.U;
    });
 
    // Initialize 2D segment tree with MINF.
    segtree.assign(SIZE * SIZE, MINF);
 
    // Process queries offline.
    int ptIndex = 0;
    int ptsz = points.size();
    vector<ll> retAns(m, MINF);
    // For each query (in order of increasing U), add all points with sum <= U.
    for (int qi = 0; qi < m; qi++){
        while(ptIndex < ptsz && points[ptIndex].sum <= queries[qi].U){
            // update the segtree at coordinate (points[ptIndex].L, points[ptIndex].R)
            updatePoint(points[ptIndex].L, points[ptIndex].R, points[ptIndex].sum);
            ptIndex++;
        }
        // Now answer the query: we want the maximum subarray sum (among those added) 
        // with starting index in [S,E] and ending index in [S,E].
        ll cur = query2D(queries[qi].S, queries[qi].E, queries[qi].S, queries[qi].E);
        retAns[queries[qi].idx] = cur;
    }
 
    // Output answers in original order. If no subarray was valid (retAns remains MINF), output "NONE".
    for (int i = 0; i < m; i++){
        if(retAns[i] == MINF)
            cout << "NONE\n";
        else
            cout << retAns[i] << "\n";
    }
    return 0;
}
