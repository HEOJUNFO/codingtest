#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct p{
    ll x, y, w;
    bool operator < (const p &t) const {
        if(x != t.x) return x < t.x;
        return y < t.y;
    }
};

int n;
int pv;
int id[262626], arr[262626];
vector<p> v;

void dfs(int nd, int d){
    if(nd*2 <= n) dfs(nd*2, d+1);
    id[nd] = pv;
    v.push_back({d, pv++, 0});
    if(nd*2+1 <= n) dfs(nd*2+1, d+1);
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    cin >> n; dfs(1, 0);
    ll ans = -1e9;
    for(int i=1; i<=n; i++){
        ll t; cin >> t;
        v[id[i]].w = t;
        ans = max(ans, t);
    }
    if(ans <= 0){
        cout << ans; return 0;
    }
    ans = 0;
    for(int i=0; i<20; i++){
        for(int j=i; j<20; j++){
            ll now = 0, mx = 0;
            for(int k=0; k<n; k++){
                if(v[k].x < i || v[k].x > j) continue;
                now = max(0LL, now + v[k].w);
                mx = max(mx, now);
            }
            ans = max(ans, mx);
        }
    }
    cout << ans;
}