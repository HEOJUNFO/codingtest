#include <bits/stdc++.h>
using namespace std;

/*
  문제:
    - L칸의 감옥 (1..L),
    - i번 칸의 탈옥력 = C[i].
    - 최대 G명의 간수.
    - 간수 1명이 [s..e] 구간(연속)을 맡으면,
        각 방 k (s <= k <= e)에 대한 위험도 = C[k] * (e - s + 1).
      여기서는 누적합을 이용해 cost(s,e)를
        ( prefix[e] - prefix[s-1] ) * ( (e)-(s)+1 )
      형태로 계산 가능 (인덱스 주의).
    - dp[i][g] = 1..i 번 방을 g 명으로 커버할 때의 최소 위험도.
    - 목표: dp[L][G] 의 값.

  여기서는 cost(j+1, i) = (prefix[i] - prefix[j]) * (i-j) 형태(인덱스만 맞추면 동일).
*/

/* 
   전역 크기 주의:
     - L 최대 8000
     - G 최대 800
     - dp[g][i] => 801 x 8001 정도는 가능
*/
static const long long INF = LLONG_MAX/4;

// dp[g][i] : i개의 방을 g명으로 커버했을 때의 최소 위험도
static long long dp[801][80001]; // 만약 L=8000인데, 인덱스 좀 넉넉히 잡았음(주의)

// C와 prefix는 1-based로 쓸 예정
static long long C[80001];
static long long prefix[80001];

// ----- cost 함수 -----
// 구간 [start..end]를 한 명이 담당했을 때의 위험도
// 여기서는 start <= end 라고 가정
inline long long cost(int start, int end) {
    // (prefix[end] - prefix[start-1]) * (end - (start) + 1)
    // = (prefix[end] - prefix[start-1]) * (end - start + 1)
    return (prefix[end] - prefix[start-1]) * (long long)(end - start + 1);
}

// ----- Divide & Conquer를 이용해 dp[g][L..R]을 채운다 -----
// solve(g, L, R, optL, optR) : i가 [L..R] 구간일 때, 
//   dp[g][i]의 최적 j(분할점)가 [optL..optR] 범위 내에 존재한다는 의미
void solve(int g, int left, int right, int optL, int optR) {
    if (left > right) return;
    
    int mid = (left + right) >> 1;
    
    // dp[g][mid]를 최소화시키는 j를 찾기
    // j in [optL..min(mid-1, optR)]
    long long bestVal = INF;
    int bestPos = -1;
    
    // 완전 탐색(여기서는 optL..optR로 범위 축소)
    // 이걸로 O(L^2) 되는 거 아닌가? -> 하지만 DnC는 각 레벨에서 탐색 범위가 좁아진다는 점,
    // 그리고 "분할"이 일어나면서 전체적으로 O(L log L)~O(L) 수준으로 끝난다는 것이 핵심.
    int endJ = min(mid-1, optR);
    for(int j = optL; j <= endJ; j++){
        long long val = dp[g-1][j] + cost(j+1, mid);
        if(val < bestVal){
            bestVal = val;
            bestPos = j;
        }
    }
    
    dp[g][mid] = bestVal;
    
    // 분할 정복 재귀
    // 왼쪽 구간
    solve(g, left, mid-1, optL, bestPos);
    // 오른쪽 구간
    solve(g, mid+1, right, bestPos, optR);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int L, G;
    cin >> L >> G;
    for(int i = 1; i <= L; i++){
        cin >> C[i];
    }
    
    // prefix 계산 (1-based)
    // prefix[i] = C[1] + C[2] + ... + C[i]
    for(int i = 1; i <= L; i++){
        prefix[i] = prefix[i-1] + C[i];
    }
    
    // dp 배열 초기화
    // dp[g][i] : i개의 방을 g명으로 커버할 때 최소 위험도
    // g=0 일 때, dp[0][i>0] = INF (커버 불가능), dp[0][0] = 0
    for(int i = 1; i <= L; i++){
        dp[0][i] = INF;
    }
    dp[0][0] = 0LL;
    
    // g = 1..G 에 대해 DP를 채움
    for(int g = 1; g <= G; g++){
        // Divide & Conquer로 dp[g][1..L] 채우기
        solve(g, 1, L, 0, L-1);
    }
    
    cout << dp[G][L] << "\n";
    
    return 0;
}
