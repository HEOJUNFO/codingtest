#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
const int MOD = 880803841; 
// primitive root for MOD (assumed to be 26)
 
// Fast modular exponentiation.
int modexp (int base, int exp, int mod = MOD) {
    ll result = 1, b = base;
    while(exp > 0) {
        if(exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)result;
}
 
// Number Theoretic Transform (NTT)
void ntt(vector<int>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j -= bit;
        j += bit;
        if(i < j)
            swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = modexp(26, (MOD - 1) / len, MOD);
        if(invert)
            wlen = modexp(wlen, MOD - 2, MOD);
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len/2; j++) {
                int u = a[i+j];
                int v = (int)((ll)a[i+j+len/2] * w % MOD);
                a[i+j] = u + v < MOD ? u+v : u+v - MOD;
                a[i+j+len/2] = u - v >= 0 ? u-v : u-v+MOD;
                w = (int)((ll)w * wlen % MOD);
            }
        }
    }
    if(invert) {
        int n_inv = modexp(n, MOD-2, MOD);
        for (int & x : a)
            x = (int)((ll)x * n_inv % MOD);
    }
}
 
// Multiply two polynomials using NTT.
vector<int> multiplyPoly(const vector<int>& a, const vector<int>& b) {
    int n = a.size(), m = b.size();
    int sz = 1;
    while(sz < n + m - 1) sz <<= 1;
    vector<int> fa(sz,0), fb(sz,0);
    for (int i = 0; i < n; i++) fa[i] = a[i];
    for (int i = 0; i < m; i++) fb[i] = b[i];
    ntt(fa, false); ntt(fb, false);
    for (int i = 0; i < sz; i++) {
        fa[i] = (int)((ll)fa[i] * fb[i] % MOD);
    }
    ntt(fa, true);
    fa.resize(n + m - 1);
    return fa;
}
 
// Segment Tree for range flip and range sum.
struct SegTree {
    int n;
    vector<int> tree;
    vector<bool> lazy;
 
    SegTree(int n) : n(n) {
        tree.resize(4*n, 0);
        lazy.assign(4*n, false);
    }
 
    void build(vector<int>& arr, int idx, int l, int r) {
        if(l == r) {
            tree[idx] = arr[l-1];
            return;
        }
        int mid = (l+r)/2;
        build(arr, idx*2, l, mid);
        build(arr, idx*2+1, mid+1, r);
        tree[idx] = tree[idx*2] + tree[idx*2+1];
    }
 
    void pushDown(int idx, int l, int r) {
        if(lazy[idx]) {
            int mid = (l+r)/2;
            lazy[idx*2] = !lazy[idx*2];
            lazy[idx*2+1] = !lazy[idx*2+1];
            tree[idx*2] = (mid - l + 1) - tree[idx*2];
            tree[idx*2+1] = (r - mid) - tree[idx*2+1];
            lazy[idx] = false;
        }
    }
 
    void update(int idx, int l, int r, int ql, int qr) {
        if(ql <= l && r <= qr) {
            tree[idx] = (r - l + 1) - tree[idx];
            lazy[idx] = !lazy[idx];
            return;
        }
        pushDown(idx, l, r);
        int mid = (l+r)/2;
        if(ql <= mid) update(idx*2, l, mid, ql, qr);
        if(qr > mid) update(idx*2+1, mid+1, r, ql, qr);
        tree[idx] = tree[idx*2] + tree[idx*2+1];
    }
 
    int query(int idx, int l, int r, int ql, int qr) {
        if(ql <= l && r <= qr) return tree[idx];
        pushDown(idx, l, r);
        int mid = (l+r)/2;
        int res = 0;
        if(ql <= mid) res += query(idx*2, l, mid, ql, qr);
        if(qr > mid) res += query(idx*2+1, mid+1, r, ql, qr);
        return res;
    }
};
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
    while(T--){
        int N, M, D;
        cin >> N >> M >> D;
 
        // 전처리: 0≤ i ≤ M에 대해 factorial과 inverse factorial 계산
        vector<int> fact(M+1), invfact(M+1);
        fact[0] = 1;
        for (int i = 1; i <= M; i++){
            fact[i] = (int)((ll)fact[i-1] * i % MOD);
        }
        invfact[M] = modexp(fact[M], MOD-2, MOD);
        for (int i = M; i >= 0; i--){
            if(i-1 >= 0)
                invfact[i-1] = (int)((ll)invfact[i] * i % MOD);
        }
 
        // A[i] = i^N / i! (단, i=0일 때는 N>=1이면 0), B[i] = (-1)^i / i!
        vector<int> A(M+1,0), B(M+1,0);
        for (int i = 0; i <= M; i++){
            if(i == 0)
                A[i] = (N==0 ? 1 : 0);
            else
                A[i] = (int)((ll) modexp(i, N, MOD) * invfact[i] % MOD);
            B[i] = ((i & 1) ? MOD - 1 : 1);
            B[i] = (int)((ll)B[i] * invfact[i] % MOD);
        }
 
        // 다항식 곱셈 (NTT 이용)로 convolution 계산: C[k] = S(N,k)
        vector<int> conv = multiplyPoly(A, B);
        vector<int> stir(M+1, 0);
        for (int k = 0; k <= M; k++){
            stir[k] = (k < (int)conv.size() ? conv[k] % MOD : 0);
        }
 
        // 세그먼트 트리: 초기 방 상태는 모두 available (1)
        vector<int> initArr(M, 1);
        SegTree seg(M);
        seg.build(initArr, 1, 1, M);
        int available = M;
 
        // 각 일마다 구간 [L,R] flip 후 available room 개수에 대응하는 S(N, available) 출력
        for (int i = 0; i < D; i++){
            int L, R;
            cin >> L >> R;
            int cnt = seg.query(1, 1, M, L, R);
            int len = R - L + 1;
            available = available + (len - 2 * cnt);
            seg.update(1, 1, M, L, R);
            int ans = (available >= 0 && available <= M) ? stir[available] : 0;
            cout << ans << "\n";
        }
    }
    return 0;
}
