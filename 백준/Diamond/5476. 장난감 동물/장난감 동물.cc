#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// BIT 1차원 (Fenwick Tree)
struct BIT1D {
    int n;
    vector<ll> tree;
    BIT1D(int n): n(n) {
        tree.assign(n+1, 0);
    }
    void update(int i, ll delta) {
        for(; i<=n; i += i & -i)
            tree[i] += delta;
    }
    ll query(int i) {
        ll s = 0;
        for(; i>0; i -= i & -i)
            s += tree[i];
        return s;
    }
    ll queryRange(int l, int r) {
        if(l > r) return 0;
        return query(r) - query(l-1);
    }
};
 
// BIT 3차원
struct BIT3D {
    int n, m, l;
    vector<int> tree; // 전체 점의 수는 최대 10^5이므로 int 사용 가능
    BIT3D(int n, int m, int l): n(n), m(m), l(l) {
        tree.assign((n+1) * (m+1) * (l+1), 0);
    }
    inline int idx(int i, int j, int k) {
        return ( (i*(m+1) + j) * (l+1) + k );
    }
    void update(int i, int j, int k, int delta) {
        for (int a = i; a <= n; a += a & -a)
            for (int b = j; b <= m; b += b & -b)
                for (int c = k; c <= l; c += c & -c)
                    tree[idx(a,b,c)] += delta;
    }
    int query(int i, int j, int k) {
        int res = 0;
        for (int a = i; a > 0; a -= a & -a)
            for (int b = j; b > 0; b -= b & -b)
                for (int c = k; c > 0; c -= c & -c)
                    res += tree[idx(a,b,c)];
        return res;
    }
    int queryRange(int i1, int j1, int k1, int i2, int j2, int k2) {
        if(i1 > i2 || j1 > j2 || k1 > k2) return 0;
        int res = query(i2, j2, k2)
            - query(i1-1, j2, k2)
            - query(i2, j1-1, k2)
            - query(i2, j2, k1-1)
            + query(i1-1, j1-1, k2)
            + query(i1-1, j2, k1-1)
            + query(i2, j1-1, k1-1)
            - query(i1-1, j1-1, k1-1);
        return res;
    }
};
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int B, N, D, M;
    cin >> B >> N >> D >> M;
    ll totalPairs = (ll)N * (N - 1) / 2;
 
    // 각 경우마다, D가 충분히 크면 모든 쌍이 조건을 만족합니다.
    if(B == 1) {
        if(D >= M-1) {
            cout << totalPairs << "\n";
            return 0;
        }
        vector<ll> arr(N);
        for (int i=0; i<N; i++){
            cin >> arr[i];
        }
        sort(arr.begin(), arr.end());
        ll ans = 0;
        int j = 0;
        for (int i=0; i<N; i++){
            while(j < N && arr[j] - arr[i] <= D)
                j++;
            ans += (j - i - 1);
        }
        cout << ans << "\n";
    }
    else if(B == 2) {
        // 2차원: 각 점 (x,y)에 대해 A=x+y, B=x-y.
        // 조건: |A_i-A_j| <= D, |B_i-B_j| <= D.
        if(D >= 2*(M-1)) {
            cout << totalPairs << "\n";
            return 0;
        }
        struct Point {
            ll A, B;
        };
        vector<Point> pts(N);
        for (int i=0; i<N; i++){
            int x, y;
            cin >> x >> y;
            pts[i].A = (ll)x + y;
            pts[i].B = (ll)x - y;
        }
        sort(pts.begin(), pts.end(), [](const Point &p1, const Point &p2){
            return p1.A < p2.A;
        });
 
        // B의 범위: x-y ∈ [1-M, M-1]
        int minB = 1 - M, maxB = M - 1;
        int sizeB = maxB - minB + 1;  // 2*M - 1
        BIT1D bit(sizeB);
        auto getIndex = [&](ll bVal) -> int {
            return (int)(bVal - minB + 1);
        };
 
        ll ans = 0;
        int left = 0;
        for (int i=0; i<N; i++){
            while(left < i && pts[i].A - pts[left].A > D) {
                bit.update(getIndex(pts[left].B), -1);
                left++;
            }
            int lidx = getIndex(pts[i].B - D);
            int ridx = getIndex(pts[i].B + D);
            if(lidx < 1) lidx = 1;
            if(ridx > sizeB) ridx = sizeB;
            ll cnt = bit.queryRange(lidx, ridx);
            ans += cnt;
            bit.update(getIndex(pts[i].B), 1);
        }
        cout << ans << "\n";
    }
    else if(B == 3) {
        // 3차원: 각 점 (x,y,z)에 대해 변환:
        // U = x+y+z, V = x+y-z, W = x-y+z, T = -x+y+z.
        // 이때, |x-x'|+|y-y'|+|z-z'| = max(|U-U'|,|V-V'|,|W-W'|,|T-T'|).
        // 조건은 네 값 모두가 D 이하가 되어야 함.
        if(D >= 3*(M-1)) {
            cout << totalPairs << "\n";
            return 0;
        }
        // 변환 후 각 좌표의 범위를 1이상으로 맞추기 위해 shift합니다.
        // U: 원래 범위 [3, 3*M] → u = U - 2, 범위 [1, 3*M-2]
        // V, W, T: 원래 V = x+y-z, 최소값 = 1+1-M = 2-M, 최대값 = M+M-1 = 2*M-1.
        // offset = -(minV) + 1 = M - 1.
        int sizeDim = 3 * M - 2;
        int offset = M - 1;
 
        struct P3 {
            int u, v, w, t; // shifted 좌표 (모두 1 이상, 최대 sizeDim)
        };
        vector<P3> pts(N);
        for (int i=0; i<N; i++){
            int x, y, z;
            cin >> x >> y >> z;
            int U = x + y + z;
            int V = x + y - z;
            int W = x - y + z;
            int T = -x + y + z;
            pts[i].u = U - 2;         // U ∈ [3,3*M] → u ∈ [1, 3*M-2]
            pts[i].v = V + offset;
            pts[i].w = W + offset;
            pts[i].t = T + offset;
        }
        sort(pts.begin(), pts.end(), [](const P3 &a, const P3 &b){
            return a.u < b.u;
        });
 
        BIT3D bit3(sizeDim, sizeDim, sizeDim);
        ll ans = 0;
        int left = 0;
        // U에 대해 투포인터 기법을 사용합니다.
        for (int i=0; i<N; i++){
            while(left < i && pts[i].u - pts[left].u > D) {
                bit3.update(pts[left].v, pts[left].w, pts[left].t, -1);
                left++;
            }
            int lv = max(1, pts[i].v - D);
            int rv = min(sizeDim, pts[i].v + D);
            int lw = max(1, pts[i].w - D);
            int rw = min(sizeDim, pts[i].w + D);
            int lt = max(1, pts[i].t - D);
            int rt = min(sizeDim, pts[i].t + D);
 
            int cnt = bit3.queryRange(lv, lw, lt, rv, rw, rt);
            ans += cnt;
            bit3.update(pts[i].v, pts[i].w, pts[i].t, 1);
        }
        cout << ans << "\n";
    }
 
    return 0;
}
