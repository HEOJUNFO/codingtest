#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

static ll dp[1<<15][101]; 
string arr[15];
int N, K;
int modArr[15];
int lenArr[15];
int p10[51]; // 10^i % K를 저장할 배열
int nextRemainder[101][15];

// 최대공약수 계산
ll gcd(ll a, ll b) {
    while (b != 0) {
        ll tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    for (int i = 0; i < N; i++){
        cin >> arr[i];
    }
    cin >> K;

    for (int i = 0; i < N; i++) {
        int m = 0;
        for (auto c : arr[i]) {
            m = (m * 10 + (c - '0')) % K;
        }
        modArr[i] = m;
        lenArr[i] = (int)arr[i].size();
    }

    p10[0] = 1 % K;
    for (int i = 1; i <= 50; i++){
        p10[i] = (p10[i-1] * 10) % K;
    }

    // nextRemainder[r][i]
    for (int r = 0; r < K; r++){
        for (int i = 0; i < N; i++){
            int mul = p10[lenArr[i]];
            int nr = (int)(((ll)r * mul + modArr[i]) % K);
            nextRemainder[r][i] = nr;
        }
    }

    for (int i = 0; i < (1<<N); i++){
        for (int j = 0; j < K; j++){
            dp[i][j] = 0;
        }
    }

    for (int i = 0; i < N; i++){
        dp[1<<i][modArr[i]] = 1;
    }

    for (int mask = 0; mask < (1<<N); mask++){
        for (int r = 0; r < K; r++){
            ll cnt = dp[mask][r];
            if (cnt == 0) continue;
            for (int i = 0; i < N; i++){
                if (mask & (1<<i)) continue;
                int nextMask = mask | (1<<i);
                int nr = nextRemainder[r][i];
                dp[nextMask][nr] += cnt;
            }
        }
    }

    ll fact = 1;
    for (int i = 2; i <= N; i++) fact *= i;

    ll numerator = dp[(1<<N)-1][0];
    if (numerator == 0) {
        cout << "0/1\n";
        return 0;
    }
    if (numerator == fact) {
        cout << "1/1\n";
        return 0;
    }

    ll g = gcd(numerator, fact);
    cout << (numerator/g) << "/" << (fact/g) << "\n";

    return 0;
}
