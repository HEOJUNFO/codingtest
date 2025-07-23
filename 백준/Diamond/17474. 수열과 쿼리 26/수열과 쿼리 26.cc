#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1LL << 60; // Large value for INF

struct NODE {
    ll max1, max2, cnt_max;
    ll sum;
};

struct SEG_TREE {
    int n;
    vector<NODE> tree;
    SEG_TREE() {}
    SEG_TREE(vector<ll> v) {
        n = v.size();
        tree.resize(4 * n);
        init(v, 0, n - 1, 1);
    }
    NODE add(NODE a, NODE b) {
        if (a.max1 == b.max1) return {a.max1, max(a.max2, b.max2), a.cnt_max + b.cnt_max, a.sum + b.sum};
        else if (a.max1 > b.max1) return {a.max1, max(a.max2, b.max1), a.cnt_max, a.sum + b.sum};
        else return {b.max1, max(a.max1, b.max2), b.cnt_max, a.sum + b.sum};
    }
    NODE init(vector<ll>& v, int left, int right, int node) {
        if (left == right) return tree[node] = {v[left], -INF, 1, v[left]};
        int mid = (left + right) / 2;
        return tree[node] = add(init(v, left, mid, node * 2), init(v, mid + 1, right, node * 2 + 1));
    }
    void lazy_propagation(int left, int right, int node) {
        if (left == right) return;
        for (int idx = node * 2; idx <= node * 2 + 1; idx++) {
            if (tree[node].max1 < tree[idx].max1) {
                tree[idx].sum -= (tree[idx].max1 - tree[node].max1) * tree[idx].cnt_max;
                tree[idx].max1 = tree[node].max1;
            }
        }
    }
    void update(int left, int right, ll k) { update(left, right, k, 0, n - 1, 1); }
    void update(int left, int right, ll k, int nodeLeft, int nodeRight, int node) {
        if (nodeRight < left || right < nodeLeft || tree[node].max1 <= k) return;
        lazy_propagation(nodeLeft, nodeRight, node);
        if (left <= nodeLeft && nodeRight <= right && tree[node].max2 < k) {
            tree[node].sum -= (tree[node].max1 - k) * tree[node].cnt_max;
            tree[node].max1 = k;
            lazy_propagation(nodeLeft, nodeRight, node);
            return;
        }
        int mid = (nodeLeft + nodeRight) / 2;
        update(left, right, k, nodeLeft, mid, node * 2);
        update(left, right, k, mid + 1, nodeRight, node * 2 + 1);
        tree[node] = add(tree[node * 2], tree[node * 2 + 1]);
    }
    ll query_max(int left, int right) { return query_max(left, right, 0, n - 1, 1); }
    ll query_max(int left, int right, int nodeLeft, int nodeRight, int node) {
        lazy_propagation(nodeLeft, nodeRight, node);
        if (nodeRight < left || right < nodeLeft) return -INF;
        if (left <= nodeLeft && nodeRight <= right) return tree[node].max1;
        int mid = (nodeLeft + nodeRight) / 2;
        return max(query_max(left, right, nodeLeft, mid, node * 2), query_max(left, right, mid + 1, nodeRight, node * 2 + 1));
    }
    ll query_sum(int left, int right) { return query_sum(left, right, 0, n - 1, 1); }
    ll query_sum(int left, int right, int nodeLeft, int nodeRight, int node) {
        lazy_propagation(nodeLeft, nodeRight, node);
        if (nodeRight < left || right < nodeLeft) return 0;
        if (left <= nodeLeft && nodeRight <= right) return tree[node].sum;
        int mid = (nodeLeft + nodeRight) / 2;
        return query_sum(left, right, nodeLeft, mid, node * 2) + query_sum(left, right, mid + 1, nodeRight, node * 2 + 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    cin >> N;
    vector<ll> v(N);
    for (auto &i : v) cin >> i;
    SEG_TREE seg(v);
    int M;
    cin >> M;
    for (int qr = 0; qr < M; qr++) {
        int Q;
        cin >> Q;
        if (Q == 1) {
            int L, R;
            ll X;
            cin >> L >> R >> X;
            seg.update(L - 1, R - 1, X);
        } else if (Q == 2) {
            int L, R;
            cin >> L >> R;
            cout << seg.query_max(L - 1, R - 1) << '\n';
        } else {
            int L, R;
            cin >> L >> R;
            cout << seg.query_sum(L - 1, R - 1) << '\n';
        }
    }
    return 0;
}