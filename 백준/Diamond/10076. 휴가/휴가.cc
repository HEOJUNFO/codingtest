#include <bits/stdc++.h>
using namespace std;
 
// ----- Persistent Segment Tree for “top-k” queries -----
//
// We will build a persistent segment tree over the array of city attraction values.
// (The idea is: given any contiguous subarray of cities we “pass by”, we want to answer
// “if you can only spend X days visiting (and must choose the X best cities in that segment),
// what is the maximum total attraction sum?”)
// 
// The tree is built over the (compressed) values. In each node we store the number
// of items (cnt) and the sum of the actual values (sum) in that node’s range.

struct Node {
    int l, r; // indices of left and right child in segtree vector
    int cnt;  // number of elements in this segment
    long long sum; // sum of values in this segment
};
 
// We'll store all nodes in a global vector.
vector<Node> segtree;
 
// Global vector of persistent tree roots for prefixes.
vector<int> rootP;
 
// Global vector of distinct sorted attraction values (for coordinate–compression).
vector<long long> compVals;
 
// Create a new node and return its index.
int newNode(int l, int r, int cnt, long long sum) {
    Node nd; nd.l = l; nd.r = r; nd.cnt = cnt; nd.sum = sum;
    segtree.push_back(nd);
    return (int)segtree.size()-1;
}
 
// Build an empty segment tree over the index range [L, R] (L=0, R=m–1).
int buildTree(int L, int R) {
    if(L == R) return newNode(-1, -1, 0, 0);
    int mid = (L + R) / 2;
    int leftChild = buildTree(L, mid);
    int rightChild = buildTree(mid+1, R);
    return newNode(leftChild, rightChild, 0, 0);
}
 
// Update the persistent tree “prev” by inserting an occurrence of value ‘val’
// at the (compressed) position pos. Returns the new root.
int updateTree(int prev, int L, int R, int pos, long long val) {
    if(L > pos || R < pos) return prev;
    if(L == R){
        int newcnt = segtree[prev].cnt + 1;
        long long newsum = segtree[prev].sum + val;
        return newNode(-1, -1, newcnt, newsum);
    }
    int mid = (L + R) / 2;
    int leftChild, rightChild;
    if(pos <= mid)
        leftChild = updateTree(segtree[prev].l, L, mid, pos, val);
    else
        leftChild = segtree[prev].l;
    if(pos > mid)
        rightChild = updateTree(segtree[prev].r, mid+1, R, pos, val);
    else
        rightChild = segtree[prev].r;
    int newcnt = segtree[leftChild].cnt + segtree[rightChild].cnt;
    long long newsum = segtree[leftChild].sum + segtree[rightChild].sum;
    return newNode(leftChild, rightChild, newcnt, newsum);
}
 
// Query function: given two persistent tree nodes (u and v) representing the difference
// between the tree for prefix up to index R and prefix up to index L–1,
// return the sum of the top k largest values (largest = highest actual number) in the range.
// (Since the tree is built over the sorted values in increasing order, “largest” means from the right.)
 
long long queryTopK(int u, int v, int L, int R, int k) {
    if(k <= 0) return 0;
    if(L == R){
        int cntDiff = segtree[v].cnt - segtree[u].cnt;
        int take = min(k, cntDiff);
        return (long long)take * compVals[L];
    }
    int mid = (L + R) / 2;
    int cntRight = segtree[ segtree[v].r ].cnt - segtree[ segtree[u].r ].cnt;
    if(cntRight >= k)
        return queryTopK(segtree[u].r, segtree[v].r, mid+1, R, k);
    else {
        long long sumRight = segtree[ segtree[v].r ].sum - segtree[ segtree[u].r ].sum;
        return sumRight + queryTopK(segtree[u].l, segtree[v].l, L, mid, k - cntRight);
    }
}
 
// Given query range [ql, qr] (0-indexed) in the original array,
// return the sum of the top k elements.
long long queryRangeTopK(int ql, int qr, int k) {
    return queryTopK(rootP[ql], rootP[qr+1], 0, (int)compVals.size()-1, k);
}
 
// ----- End persistent segtree functions -----
 
// Global variables for the problem.
int n, start;
long long d;
vector<long long> A;          // A[i]: number of attractions in city i.
vector<long long> prefixSum;  // For full–sum queries on contiguous segments.
 
// Precompute prefix sums of A.
void computePrefixSum() {
    prefixSum.resize(n);
    prefixSum[0] = A[0];
    for (int i = 1; i < n; i++)
        prefixSum[i] = prefixSum[i-1] + A[i];
}
 
// Return full sum of A[L..R] (inclusive).
long long rangeFullSum(int L, int R) {
    if(L > R) return 0;
    return prefixSum[R] - (L == 0 ? 0 : prefixSum[L-1]);
}
 
// --- “Route simulation” functions ---
//
// We imagine that during the vacation the traveler (starting at city “start”)
// may “extend” his route a steps to the left and b steps to the right (so that
// the segment covered is [start–a, start+b]). In that segment there are S = a+b+1 cities.
// Each “visit” day (when the traveler collects attractions) must be assigned
// to one city from this set. However, the traveler does not have enough days;
// he must “choose” only some of them. (Movement days are forced.)
// 
// If he “travels” so that the farthest left city is reached (distance a) and farthest right (distance b),
// then depending on the order he uses one of two movement–cost formulas:
//  • If a ≤ b (we use “left–first” order) the movement cost is: M = 2*a + b.
//  • If a > b  (use “right–first” order) the cost is: M = 2*b + a.
// 
// Then the number of days left for “visiting” is (d – M). (If negative, the route is not feasible.)
// He can visit at most min(d – M, S) cities from the available S.
// Since all attraction counts are nonnegative, if (d – M) ≥ S he simply “visits all”.
// Otherwise he should choose the (d – M) cities having the highest attraction values.
// (The persistent segtree query “queryRangeTopK” does exactly that.)
// 
// The following two functions return the maximum total attractions collected
// when the segment [start–a, start+b] is “traversed” under the two orders.
 
// Case A: a ≤ b. (Left-first order; movement cost M = 2*a + b.)
long long getValueA(int a, int b, int globalStart, long long globalD) {
    int Lidx = globalStart - a;
    int Ridx = globalStart + b;
    int S = a + b + 1;
    int M = 2 * a + b; // movement cost
    long long allowed = globalD - M; // days available for visits
    if(allowed < 0) return -1; // not feasible
    if(allowed >= S)
        return rangeFullSum(Lidx, Ridx);
    else
        return queryRangeTopK(Lidx, Ridx, (int)allowed);
}
 
// Case B: a > b. (Right-first order; movement cost M = 2*b + a.)
long long getValueB(int a, int b, int globalStart, long long globalD) {
    int Lidx = globalStart - a;
    int Ridx = globalStart + b;
    int S = a + b + 1;
    int M = 2 * b + a;
    long long allowed = globalD - M;
    if(allowed < 0) return -1;
    if(allowed >= S)
        return rangeFullSum(Lidx, Ridx);
    else
        return queryRangeTopK(Lidx, Ridx, (int)allowed);
}
 
// ----- Ternary search on integer domain -----
//
// Given an integer domain [lo, hi] and a function f(int),
// we perform a ternary search (assuming f is unimodal)
// and return the maximum value of f on [lo, hi].
template <typename Func>
long long ternarySearchInt(int lo, int hi, Func f) {
    if(lo > hi) return -1;
    while(hi - lo > 5) {
        int m1 = lo + (hi - lo) / 3;
        int m2 = hi - (hi - lo) / 3;
        long long f1 = f(m1), f2 = f(m2);
        if(f1 < f2)
            lo = m1 + 1;
        else
            hi = m2 - 1;
    }
    long long best = -1;
    for (int i = lo; i <= hi; i++){
        best = max(best, f(i));
    }
    return best;
}
 
// ----- Main -----
//
// We “simulate” the possible segments that include the starting city.
// Let a = how many cities to the left (distance) and b = how many to the right.
// (Then the segment is [start–a, start+b].)
// We then split the cases by the order:
//   • When a ≤ b, we use getValueA (movement cost = 2*a+b).
//   • When a > b, we use getValueB (movement cost = 2*b+a).
// For each possible left extension a (0 ≤ a ≤ start) we determine the feasible range of b:
//   For case A: we require a ≤ b ≤ min(Rmax, d – 2*a)   [since d – (2*a+b) must be ≥ 0]
//   For case B: if a > 0, then 0 ≤ b ≤ min(a–1, Rmax, d–a)
// Then for each we “optimize” (by ternary search) over b.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n >> start >> d;
    A.resize(n);
    for (int i = 0; i < n; i++){
        cin >> A[i];
    }
    computePrefixSum();
 
    // Build persistent segment tree.
    // First, compress A.
    compVals = A;
    sort(compVals.begin(), compVals.end());
    compVals.erase(unique(compVals.begin(), compVals.end()), compVals.end());
    int mComp = compVals.size();
 
    // Reserve space for segtree nodes.
    segtree.reserve((n+5) * (int)(ceil(log2(mComp)) + 5));
    int emptyRoot = buildTree(0, mComp-1);
    rootP.resize(n+1);
    rootP[0] = emptyRoot;
    for (int i = 0; i < n; i++){
        int pos = (int)(std::lower_bound(compVals.begin(), compVals.end(), A[i]) - compVals.begin());
        int newRoot = updateTree(rootP[i], 0, mComp-1, pos, A[i]);
        rootP[i+1] = newRoot;
    }
 
    long long ans = 0;
    // Let a = (start - L) [extension to the left] and
    //     b = (R - start) [extension to the right].
    int Lmax = start;              // maximum possible left extension
    int Rmax = n - 1 - start;        // maximum possible right extension
 
    // Iterate over possible left extensions a.
    for (int a = 0; a <= Lmax; a++){
        // --- Case A: a <= b, so require a <= b <= min(Rmax, d - 2*a) ---
        if(a <= Rmax) {
            int lo = a;
            int hi = min(Rmax, (int)d - 2*a); // because we need d - (2*a + b) >= 0
            if(lo <= hi){
                auto fA = [a, start](int b) -> long long {
                    return getValueA(a, b, start, d);
                };
                long long bestA = ternarySearchInt(lo, hi, fA);
                ans = max(ans, bestA);
            }
        }
        // --- Case B: a > b (so a must be at least 1) ---
        if(a > 0) {
            int lo = 0;
            int hi = min({a - 1, Rmax, (int)d - a});
            if(lo <= hi){
                auto fB = [a, start](int b) -> long long {
                    return getValueB(a, b, start, d);
                };
                long long bestB = ternarySearchInt(lo, hi, fB);
                ans = max(ans, bestB);
            }
        }
    }
    // Also, ensure the trivial segment [start,start] is considered.
    ans = max(ans, A[start]);
 
    cout << ans << "\n";
    return 0;
}
