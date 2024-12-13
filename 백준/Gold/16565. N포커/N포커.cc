#include <bits/stdc++.h>
using namespace std;

const int MOD = 10007;

// 조합 계산을 위한 파스칼 삼각형
int comb[53][53];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N; cin >> N;
    
    // 조합값 계산 (C(n,k))
    for (int i = 0; i <= 52; i++) {
        comb[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            comb[i][j] = (comb[i-1][j-1] + comb[i-1][j]) % MOD;
        }
    }
    
    // dp[i][j]: i개의 랭크(숫자)에서 j장 뽑는 포카드 없는 경우의 수
    static int dp[14][53];
    dp[0][0] = 1;
    
    for (int i = 1; i <= 13; i++) {
        for (int j = 0; j <= N; j++) {
            // k = 0 ~ 3 장 뽑기
            // C(4,0)=1, C(4,1)=4, C(4,2)=6, C(4,3)=4
            if (j >= 0) dp[i][j] = (dp[i][j] + (dp[i-1][j] * comb[4][0]) % MOD) % MOD;
            if (j >= 1) dp[i][j] = (dp[i][j] + (dp[i-1][j-1] * comb[4][1]) % MOD) % MOD;
            if (j >= 2) dp[i][j] = (dp[i][j] + (dp[i-1][j-2] * comb[4][2]) % MOD) % MOD;
            if (j >= 3) dp[i][j] = (dp[i][j] + (dp[i-1][j-3] * comb[4][3]) % MOD) % MOD;
        }
    }
    
    int total = comb[52][N];       // 전체 경우의 수
    int no_four = dp[13][N];       // 포카드 없는 경우의 수
    int ans = (total - no_four) % MOD;
    if (ans < 0) ans += MOD;
    
    cout << ans << "\n";
    return 0;
}
