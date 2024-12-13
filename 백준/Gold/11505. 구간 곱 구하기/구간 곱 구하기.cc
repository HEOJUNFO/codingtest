#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
typedef long long ll;

int N, M, K;
vector<ll> arr;
vector<ll> segtree;

ll init(int start, int end, int node) {
    if (start == end) {
        return segtree[node] = arr[start] % MOD;
    }
    int mid = (start + end) / 2;
    ll leftVal = init(start, mid, node * 2);
    ll rightVal = init(mid + 1, end, node * 2 + 1);
    return segtree[node] = (leftVal * rightVal) % MOD;
}

ll update(int start, int end, int node, int idx, ll val) {
    if (idx < start || idx > end) return segtree[node];
    if (start == end) {
        return segtree[node] = val % MOD;
    }
    int mid = (start + end) / 2;
    ll leftVal = update(start, mid, node * 2, idx, val);
    ll rightVal = update(mid + 1, end, node * 2 + 1, idx, val);
    return segtree[node] = (leftVal * rightVal) % MOD;
}

ll query(int start, int end, int node, int left, int right) {
    if (right < start || end < left) return 1;
    if (left <= start && end <= right) return segtree[node];
    int mid = (start + end) / 2;
    ll leftVal = query(start, mid, node * 2, left, right);
    ll rightVal = query(mid + 1, end, node * 2 + 1, left, right);
    return (leftVal * rightVal) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> K;
    arr.resize(N+1);
    for (int i = 1; i <= N; i++) cin >> arr[i];

    segtree.resize(4 * N);
    init(1, N, 1);

    int cnt = M + K;
    for (int i = 0; i < cnt; i++) {
        ll a, b, c;
        cin >> a >> b >> c;
        if (a == 1) { // update
            update(1, N, 1, (int)b, c);
        } else { // query
            cout << query(1, N, 1, (int)b, (int)c) % MOD << "\n";
        }
    }
    return 0;
}
