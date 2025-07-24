#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 50010;
ll S[MAXN];
ll DP[MAXN];
int prevv[MAXN];
ll v[MAXN];
int N, K;
ll INF = 1LL << 60;
vector<int> dq;

ll F(int j, int x) {
    if (x == 0) return 0;
    return -S[j] * (ll)x - (ll)j * S[x] + DP[j] + (ll)j * S[j];
}

ll get_cross(int a, int b) {
    int low = b + 1, high = N + 1;
    while (low < high) {
        int mid = (low + high) / 2;
        ll fa = F(a, mid);
        ll fb = F(b, mid);
        if (fa > fb) high = mid;
        else low = mid + 1;
    }
    return low;
}

void compute_DP(ll C) {
    fill(DP, DP + N + 1, INF);
    DP[0] = 0;
    dq.clear();
    dq.push_back(0);
    for (int x = 1; x <= N; x++) {
        while (dq.size() >= 2) {
            int a = dq[0], b = dq[1];
            ll cros = get_cross(a, b);
            if (cros <= x) {
                dq.erase(dq.begin());
            } else break;
        }
        int best = dq[0];
        ll minf = F(best, x) + C;
        DP[x] = minf + (ll)x * S[x];
        prevv[x] = best;
        while (dq.size() >= 2) {
            int bb = dq[dq.size() - 2];
            int aa = dq.back();
            ll cross1 = get_cross(bb, aa);
            ll cross2 = get_cross(aa, x);
            if (cross1 >= cross2) {
                dq.pop_back();
            } else break;
        }
        dq.push_back(x);
    }
}

int get_groups() {
    int cur = N;
    int cnt = 0;
    while (cur > 0) {
        cnt++;
        cur = prevv[cur];
    }
    return cnt;
}

int main() {
    cin >> N >> K;
    if (K > N) K = N;
    for (int i = 1; i <= N; i++) {
        cin >> v[i];
        S[i] = S[i - 1] + v[i];
    }
    ll low = 0, high = 50000LL * 50000 * 50000 + 1;
    while (low < high) {
        ll mid = (low + high) / 2;
        compute_DP(mid);
        int groups = get_groups();
        if (groups >= K) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    ll opt_C = low - 1;
    compute_DP(opt_C);
    ll ans = DP[N] - opt_C * (ll)K;
    cout << ans << endl;
    return 0;
}