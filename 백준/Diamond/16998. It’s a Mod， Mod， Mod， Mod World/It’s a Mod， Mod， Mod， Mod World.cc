#include <iostream>
#include <algorithm>
using namespace std;
using ll = long long;

// Computes sum_{0<=i<n} floor((a*i+b)/m)
// Assumes m > 0, n >= 0, a >= 0, b >= 0.
ll floor_sum(ll n, ll m, ll a, ll b) {
    ll ans = 0;
    while(n) {
        if(a >= m) {
            ans += (n - 1) * n * (a / m) / 2;
            a %= m;
        }
        if(b >= m) {
            ans += n * (b / m);
            b %= m;
        }
        ll y = a * n + b;
        if(y < m) break;
        n = y / m;
        b = y % m;
        swap(a, m);
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int W;
    cin >> W;
    while(W--){
        ll p, q, n;
        cin >> p >> q >> n;
        
        // Total sum of p * i for i=1..n.
        ll sum_i = n * (n + 1) / 2;
        // Sum of floor((p*i)/q) for i=1..n can be computed as:
        //   floor_sum(n, q, p, p)
        ll floorPart = floor_sum(n, q, p, p);
        
        // Final answer: sum_{i=1}^{n} (p*i mod q)
        ll result = p * sum_i - q * floorPart;
        cout << result << "\n";
    }
    return 0;
}
