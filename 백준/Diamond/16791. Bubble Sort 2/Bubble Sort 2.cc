#include <bits/stdc++.h>
using namespace std;
 
// ---------- Treap structure with lazy propagation ------------
 
struct Node {
    int a;      // compressed value
    int idx;    // original index (1-indexed)
    int b;      // computed value: b = idx - (# of nodes with key.a ≤ A[i]) (see explanation)
    int sz;     // subtree size
    int mx;     // maximum b in subtree
    int lazy;   // lazy-add value to be propagated
    int priority;  // random priority
    int left, right; // child indices
};
 
const int MAXNODES = 1100000; // Maximum nodes (N+Q <= about 1e6)
Node treap[MAXNODES];
int tot = 0;  // total allocated nodes
int root = 0; // index of treap root (0 means null)
 
// Return subtree size (0 if null)
inline int getSize(int t) { return t ? treap[t].sz : 0; }
 
// Return maximum value in subtree (or very small if null)
inline int getMax(int t) { return t ? treap[t].mx : -1000000000; }
 
// Apply lazy value to node t
inline void applyLazy(int t, int addVal) {
    if(t == 0) return;
    treap[t].b += addVal;
    treap[t].mx += addVal;
    treap[t].lazy += addVal;
}
 
// Update node t (its size and maximum) from its children
inline void updateNode(int t) {
    if(t == 0) return;
    treap[t].sz = getSize(treap[t].left) + getSize(treap[t].right) + 1;
    treap[t].mx = max(treap[t].b, max(getMax(treap[t].left), getMax(treap[t].right)));
}
 
// Push down lazy value from node t to its children
inline void pushDown(int t) {
    if(t == 0 || treap[t].lazy == 0) return;
    int lazyVal = treap[t].lazy;
    if(treap[t].left)  applyLazy(treap[t].left, lazyVal);
    if(treap[t].right) applyLazy(treap[t].right, lazyVal);
    treap[t].lazy = 0;
}
 
// In our treap the key is the pair (a, idx); compare lexicographically.
 
// Split treap 't' by key 'k'. All nodes with key < k go to L and the rest go to R.
void split(int t, pair<int,int> k, int &L, int &R) {
    if(t == 0) { L = R = 0; return; }
    pushDown(t);
    // if (treap[t].a, treap[t].idx) < k, then t belongs to L.
    if ((treap[t].a < k.first) || (treap[t].a == k.first && treap[t].idx < k.second)) {
        L = t;
        split(treap[t].right, k, treap[t].right, R);
        updateNode(t);
    } else {
        R = t;
        split(treap[t].left, k, L, treap[t].left);
        updateNode(t);
    }
}
 
// Merge two treaps L and R (assume all keys in L are < all keys in R)
int mergeTreap(int L, int R) {
    if(L == 0 || R == 0) return L ? L : R;
    pushDown(L); pushDown(R);
    if(treap[L].priority < treap[R].priority) {
        treap[L].right = mergeTreap(treap[L].right, R);
        updateNode(L);
        return L;
    } else {
        treap[R].left = mergeTreap(L, treap[R].left);
        updateNode(R);
        return R;
    }
}
 
// Create a new node with key (a, idx) and value b; return its index.
int newNode(int a, int idx, int b) {
    tot++;
    treap[tot].a = a;
    treap[tot].idx = idx;
    treap[tot].b = b;
    treap[tot].mx = b;
    treap[tot].sz = 1;
    treap[tot].lazy = 0;
    treap[tot].priority = rand();
    treap[tot].left = treap[tot].right = 0;
    return tot;
}
 
// Insert a node with key (a, idx) and value b into the treap.
void insertNode(int a, int idx, int b) {
    int L, R;
    pair<int,int> key = {a, idx};
    split(root, key, L, R);
    int cur = newNode(a, idx, b);
    root = mergeTreap(mergeTreap(L, cur), R);
}
 
// Erase the node with key (a, idx) from the treap.
void eraseNode(pair<int,int> key) {
    int L, mid, R;
    split(root, key, L, R);
    split(R, {key.first, key.second+1}, mid, R);
    // (mid should contain the node to erase; we simply discard it)
    root = mergeTreap(L, R);
}
 
// Range add: add addVal to every node with key.a >= X.
// We split by (X, 0) (since our idx are ≥1).
void rangeAdd(int X, int addVal) {
    int L, R;
    split(root, {X, 0}, L, R);
    if(R != 0) applyLazy(R, addVal);
    root = mergeTreap(L, R);
}
 
// Query: count the number of nodes with key.a ≤ X.
int querySize(int X) {
    int L, R;
    split(root, {X+1, 0}, L, R);
    int res = getSize(L);
    root = mergeTreap(L, R);
    return res;
}
 
// Query: return the maximum b in the whole treap.
int queryMax() {
    return root ? treap[root].mx : -1000000000;
}
 
// ---------- End Treap structure ------------
 
// Now the main idea:
//
// We will compress the input values (both in the initial sequence and in the queries).
// For each 1-indexed position i we define an initial value b[i] as
//     b[i] = i - (number of elements with value ≤ A[i])
// (with a tie–breaking computed as in many published solutions).
// Then the answer (number of bubble sort passes) equals max{b[i]} – 1 (which is 0 when negative).
//
// When an update at position x (1-indexed) changes A[x] from old to new, we
//  1. remove the old element (and for every other element with value > old, its
//     “count of smaller” decreases by 1 so its b increases by 1),
//  2. update A[x] = new,
//  3. then for every element with value > new, its “count of smaller” increases by 1
//     (so its b decreases by 1),
//  4. recalc b[x] = x - ( (# of nodes with a ≤ new) + 1 ),
//  5. reinsert the new element,
//  6. and answer = queryMax() (if negative, answer 0).
//
// (This exactly follows the outline in the official editorial.)
 
// Main function
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, Q;
    cin >> N >> Q;
    vector<int> A(N+1);
    for (int i = 1; i <= N; i++){
        cin >> A[i];
    }
    vector<int> qX(Q+1), qV(Q+1);
    vector<int> allVals;
    for (int i = 1; i <= N; i++){
        allVals.push_back(A[i]);
    }
    for (int i = 1; i <= Q; i++){
        int x, v;
        cin >> x >> v;
        qX[i] = x + 1; // convert to 1-indexed
        qV[i] = v;
        allVals.push_back(v);
    }
    // Coordinate compression
    sort(allVals.begin(), allVals.end());
    allVals.erase(unique(allVals.begin(), allVals.end()), allVals.end());
    auto compress = [&](int x) -> int {
        return int(lower_bound(allVals.begin(), allVals.end(), x) - allVals.begin()) + 1;
    };
    for (int i = 1; i <= N; i++){
        A[i] = compress(A[i]);
    }
    for (int i = 1; i <= Q; i++){
        qV[i] = compress(qV[i]);
    }
 
    // Compute initial b values.
    // We form an array id[1..N] of indices and sort by A[i] (and by i to break ties).
    vector<int> b(N+1, 0);
    vector<int> id(N+1);
    for (int i = 1; i <= N; i++){
        id[i] = i;
    }
    sort(id.begin() + 1, id.end(), [&](int i, int j){
        if (A[i] != A[j]) return A[i] < A[j];
        return i < j;
    });
    // For each block of equal A, for each index in the block set:
    //     b[i] = i - (last index of block)  (this turns out to work correctly)
    for (int i = 1; i <= N; ) {
        int j = i;
        while(j <= N && A[id[i]] == A[id[j]]) j++;
        for (int k = i; k < j; k++){
            int pos = id[k];
            b[pos] = pos - (j - 1);
        }
        i = j;
    }
    // Build initial treap.
    tot = 0; root = 0;
    for (int i = 1; i <= N; i++){
        insertNode(A[i], i, b[i]);
    }
 
    // Process queries.
    // For each query we:
    //   (1) remove the node corresponding to position x (with key (old A[x], x));
    //   (2) update: for every node with key.a ≥ old A[x], add +1;
    //   (3) update A[x] to new value;
    //   (4) update: for every node with key.a ≥ new A[x], add -1;
    //   (5) compute b[x] = x - ( (number of nodes with key.a ≤ new A[x]) + 1 );
    //   (6) reinsert the new node; and answer = queryMax().
    for (int i = 1; i <= Q; i++){
        int x = qX[i];
        int oldVal = A[x];
        // Erase node with key (oldVal, x)
        eraseNode({oldVal, x});
        // For every remaining node with a ≥ oldVal, its count of smaller decreases by 1 so b increases by 1.
        rangeAdd(oldVal, +1);
 
        // Now update A[x] to new value.
        A[x] = qV[i];
        // For every node with a ≥ new A[x], its count of smaller increases by 1 so b decreases by 1.
        rangeAdd(A[x], -1);
 
        // Compute new b[x] = x - ( (# of nodes with key.a ≤ A[x]) + 1 )
        int cnt = querySize(A[x]); // count nodes with a ≤ A[x]
        b[x] = x - (cnt + 1);
 
        // Reinsert node with key (A[x], x) and value b[x]
        insertNode(A[x], x, b[x]);
 
        int ans = queryMax();
        if(ans < 0) ans = 0;
        cout << ans << "\n";
    }
 
    return 0;
}
