#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long val;
    int idx;
};

int N, M;
long long arr[1000001]; 
Node segtree[4000004]; // Segment tree

// Segment tree build function
// node: current node index in segment tree
// start, end: segment of the original array this node is responsible for
void build(int node, int start, int end) {
    if(start == end) {
        segtree[node].val = arr[start];
        segtree[node].idx = start;
        return;
    }
    int mid = (start + end) / 2;
    build(node*2, start, mid);
    build(node*2+1, mid+1, end);
    // Merge
    if(segtree[node*2].val < segtree[node*2+1].val) {
        segtree[node].val = segtree[node*2].val;
        segtree[node].idx = segtree[node*2].idx;
    } else if(segtree[node*2].val > segtree[node*2+1].val) {
        segtree[node].val = segtree[node*2+1].val;
        segtree[node].idx = segtree[node*2+1].idx;
    } else {
        // If equal value, choose smaller index
        if(segtree[node*2].idx < segtree[node*2+1].idx) {
            segtree[node].val = segtree[node*2].val;
            segtree[node].idx = segtree[node*2].idx;
        } else {
            segtree[node].val = segtree[node*2+1].val;
            segtree[node].idx = segtree[node*2+1].idx;
        }
    }
}

// Update: set arr[pos] = val
void update(int node, int start, int end, int pos, long long val) {
    if(pos < start || pos > end) return;
    if(start == end) {
        segtree[node].val = val;
        segtree[node].idx = start;
        return;
    }
    int mid = (start + end) / 2;
    update(node*2, start, mid, pos, val);
    update(node*2+1, mid+1, end, pos, val);
    // Merge
    if(segtree[node*2].val < segtree[node*2+1].val) {
        segtree[node].val = segtree[node*2].val;
        segtree[node].idx = segtree[node*2].idx;
    } else if(segtree[node*2].val > segtree[node*2+1].val) {
        segtree[node].val = segtree[node*2+1].val;
        segtree[node].idx = segtree[node*2+1].idx;
    } else {
        if(segtree[node*2].idx < segtree[node*2+1].idx) {
            segtree[node].val = segtree[node*2].val;
            segtree[node].idx = segtree[node*2].idx;
        } else {
            segtree[node].val = segtree[node*2+1].val;
            segtree[node].idx = segtree[node*2+1].idx;
        }
    }
}

// Query: get min value index in range [l, r]
Node query(int node, int start, int end, int l, int r) {
    if(r < start || l > end) {
        return {(long long)1e15, (int)1e9}; // return large value & large index
    }
    if(l <= start && end <= r) {
        return segtree[node];
    }
    int mid = (start + end)/2;
    Node leftNode = query(node*2, start, mid, l, r);
    Node rightNode = query(node*2+1, mid+1, end, l, r);
    Node ret;
    if(leftNode.val < rightNode.val) {
        ret = leftNode;
    } else if(leftNode.val > rightNode.val) {
        ret = rightNode;
    } else {
        if(leftNode.idx < rightNode.idx) ret = leftNode;
        else ret = rightNode;
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> N;
    for (int i = 1; i <= N; i++) cin >> arr[i];
    build(1, 1, N);
    cin >> M;
    while(M--) {
        int t; cin >> t;
        if(t == 1) {
            int i; long long v;
            cin >> i >> v;
            update(1, 1, N, i, v);
        } else {
            int i, j;
            cin >> i >> j;
            Node ans = query(1, 1, N, i, j);
            cout << ans.idx << "\n";
        }
    }
    return 0;
}
