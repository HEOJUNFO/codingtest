#include <bits/stdc++.h>
using namespace std;
 
// Node structure for our segment tree.
struct Node {
    int first, last;  // values at beginning and end of the segment
    int mn, mx;       // minimum and maximum values in the segment
    int cnt;          // number of adjacent sign changes in this segment
    int lazy;         // lazy propagation value
};
 
// Segment tree structure that supports range addition and queries 
// for the number of sign changes (i.e. axis crossings in one coordinate).
struct SegTree {
    int n;
    vector<Node> tree;
 
    SegTree(int n) : n(n) {
        tree.resize(4 * n);
    }
 
    // Merge two nodes L and R.
    Node mergeNode(const Node &L, const Node &R) {
        Node res;
        res.first = L.first;
        res.last = R.last;
        res.mn = min(L.mn, R.mn);
        res.mx = max(L.mx, R.mx);
        // The crossing at the junction: add 1 if L.last and R.first have different signs.
        res.cnt = L.cnt + R.cnt + (((L.last > 0) != (R.first > 0)) ? 1 : 0);
        res.lazy = 0;
        return res;
    }
 
    // Build the tree over array "arr" in interval [l, r] at node idx.
    void build(vector<int> &arr, int idx, int l, int r) {
        if(l == r) {
            tree[idx].first = arr[l];
            tree[idx].last = arr[l];
            tree[idx].mn = arr[l];
            tree[idx].mx = arr[l];
            tree[idx].cnt = 0; // single element: no adjacent pair
            tree[idx].lazy = 0;
            return;
        }
        int mid = (l + r) / 2;
        build(arr, idx * 2, l, mid);
        build(arr, idx * 2 + 1, mid + 1, r);
        tree[idx] = mergeNode(tree[idx * 2], tree[idx * 2 + 1]);
    }
 
    // Helper: apply a lazy update (add delta) to node idx covering [l, r].
    void applyNode(int idx, int l, int r, int delta) {
        tree[idx].first += delta;
        tree[idx].last += delta;
        tree[idx].mn += delta;
        tree[idx].mx += delta;
        tree[idx].lazy += delta;
        // If after update the entire segment is uniformly >0 or <0, then no internal crossing.
        if(tree[idx].mn > 0 || tree[idx].mx < 0)
            tree[idx].cnt = 0;
    }
 
    // Push lazy update down to children.
    void pushDown(int idx, int l, int r) {
        if(tree[idx].lazy != 0 && l < r) {
            int mid = (l + r) / 2;
            int lazyVal = tree[idx].lazy;
            applyNode(idx * 2, l, mid, lazyVal);
            applyNode(idx * 2 + 1, mid + 1, r, lazyVal);
            tree[idx].lazy = 0;
        }
    }
 
    // Range update: add delta to every index in [ql, qr].
    void update(int idx, int l, int r, int ql, int qr, int delta) {
        if(ql > r || qr < l) return;
        if(ql <= l && r <= qr) {
            int newMin = tree[idx].mn + delta;
            int newMax = tree[idx].mx + delta;
            // If after addition the whole segment is uniformly positive or uniformly negative,
            // we can update at once.
            if(newMin > 0 || newMax < 0) {
                applyNode(idx, l, r, delta);
                return;
            }
        }
        if(l == r) {
            // leaf node update
            tree[idx].first += delta;
            tree[idx].last += delta;
            tree[idx].mn += delta;
            tree[idx].mx += delta;
            tree[idx].cnt = 0;
            return;
        }
        pushDown(idx, l, r);
        int mid = (l + r) / 2;
        update(idx * 2, l, mid, ql, qr, delta);
        update(idx * 2 + 1, mid + 1, r, ql, qr, delta);
        tree[idx] = mergeNode(tree[idx * 2], tree[idx * 2 + 1]);
    }
 
    // Query the node representing the interval [ql, qr].
    Node query(int idx, int l, int r, int ql, int qr) {
        if(ql <= l && r <= qr)
            return tree[idx];
        pushDown(idx, l, r);
        int mid = (l + r) / 2;
        if(qr <= mid)
            return query(idx * 2, l, mid, ql, qr);
        else if(ql > mid)
            return query(idx * 2 + 1, mid + 1, r, ql, qr);
        else {
            Node leftNode = query(idx * 2, l, mid, ql, qr);
            Node rightNode = query(idx * 2 + 1, mid + 1, r, ql, qr);
            return mergeNode(leftNode, rightNode);
        }
    }
 
    // Public update: add delta to indices [ql, qr].
    void updateRange(int ql, int qr, int delta) {
        update(1, 0, n - 1, ql, qr, delta);
    }
 
    // Public query: returns the node covering [ql, qr].
    Node queryRange(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }
};
 
// Main – simulating the commands.
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    // The plan is given as N vectors (indexed 1..N).
    vector<int> vx(N + 1), vy(N + 1);
    for (int i = 1; i <= N; i++){
        cin >> vx[i] >> vy[i];
    }
    // Build prefix arrays for positions.
    // X[0] = 1 and Y[0] = 1 (starting point).
    // For i>=1, X[i] = X[i-1] + vx[i] and Y[i] = Y[i-1] + vy[i].
    vector<int> X(N + 1), Y(N + 1);
    X[0] = 1;
    Y[0] = 1;
    for (int i = 1; i <= N; i++){
        X[i] = X[i - 1] + vx[i];
        Y[i] = Y[i - 1] + vy[i];
    }
 
    // Build segment trees over indices 0..N.
    SegTree segX(N + 1), segY(N + 1);
    segX.build(X, 1, 0, N);
    segY.build(Y, 1, 0, N);
 
    int M;
    cin >> M;
    int cursor = 1; // initial cursor position is at line 1.
    while(M--){
        char op;
        cin >> op;
        if(op == 'B'){
            if(cursor > 1) cursor--;
        } else if(op == 'F'){
            if(cursor < N) cursor++;
        } else if(op == 'C'){
            int nx, ny;
            cin >> nx >> ny;
            // Compute delta for the vector at the current cursor position.
            int dx = nx - vx[cursor];
            int dy = ny - vy[cursor];
            vx[cursor] = nx; vy[cursor] = ny;
            // This change affects all prefix sums from index "cursor" to N.
            if(dx) segX.updateRange(cursor, N, dx);
            if(dy) segY.updateRange(cursor, N, dy);
        } else if(op == 'Q'){
            // Query: the answer is the sum over segments (from 0 to N) of
            // the x–axis crossings (i.e. transitions in segX) plus y–axis crossings.
            // (For a segment from S[i-1] to S[i], if S[i-1] and S[i] have different signs
            // in the x coordinate, that contributes 1 – and similarly for y.)
            Node resX = segX.queryRange(0, N);
            Node resY = segY.queryRange(0, N);
            cout << (resX.cnt + resY.cnt) << "\n";
        }
    }
    return 0;
}
