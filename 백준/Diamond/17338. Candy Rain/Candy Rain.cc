#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, LEN; 
    cin >> n >> LEN;
    int M = LEN * 2;
    vector<int> fTable(M * M);
    for (int X = 0; X < M; X++){
        for (int a = 0; a < M; a++){
            int r = a + X; 
            if(r >= M) r -= M;
            fTable[X * M + a] = (r <= LEN ? r : M - r);
        }
    }
 
    int fenwSize = LEN + 1;
    vector<int> BIT(M * (fenwSize+1), 0);
    vector<int> bucketTotal(M, 0);
    auto bit_update = [&](int bucket, int idx, int delta){
        int off = bucket * (fenwSize+1);
        int pos = idx + 1;
        for(; pos <= fenwSize; pos += pos & -pos)
            BIT[off + pos] += delta;
    };
    auto bit_query = [&](int bucket, int idx) -> int {
        int off = bucket * (fenwSize+1);
        int pos = idx + 1, s = 0;
        for(; pos; pos -= pos & -pos)
            s += BIT[off + pos];
        return s;
    };
    auto bit_range = [&](int bucket, int l, int r) -> int {
        if(l > r) return 0;
        return bit_query(bucket, r) - (l ? bit_query(bucket, l-1) : 0);
    };
 
    unordered_map<int, pair<int,int>> mp;
    mp.reserve(n*2);
 
    for (int i = 0; i < n; i++){
        int type; 
        cin >> type;
        if(type == 1){
            int T, color, L0, R0, d;
            cin >> T >> color >> L0 >> R0 >> d;
            int P = R0 - L0, A;
            if(d == 1){
                int tmp = (L0 - T) % M; 
                if(tmp < 0) tmp += M;
                A = tmp;
            } else {
                int tmp = (-T - L0 + M) % M; 
                if(tmp < 0) tmp += M;
                A = tmp;
            }
            mp[color] = {A, P};
            bucketTotal[A]++;
            bit_update(A, P, 1);
        } else if(type == 3){
            int T, color;
            cin >> T >> color;
            auto it = mp.find(color);
            if(it != mp.end()){
                int A = it->second.first, P = it->second.second;
                bucketTotal[A]--;
                bit_update(A, P, -1);
                mp.erase(it);
            }
        } else {
            int T, qL, qR;
            cin >> T >> qL >> qR;
            int X = T % M;
            ll ans = 0;
            for (int a = 0; a < M; a++){
                int f = fTable[X * M + a];
                if(f > qR) continue;
                if(f >= qL)
                    ans += bucketTotal[a];
                else{
                    int lower = qL - f; 
                    if(lower < 0) lower = 0;
                    ans += bit_range(a, lower, LEN);
                }
            }
            cout << ans << "\n";
        }
    }
    return 0;
}
