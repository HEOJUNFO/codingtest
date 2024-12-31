#include <bits/stdc++.h>
using namespace std;

/*
 * 파일 합치기 (소설 합치기) 문제
 * Knuth Optimization을 사용하여 O(K^2)로 해결하는 코드 예시
 */

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T; // 테스트 케이스 개수

    while (T--) {
        int K;
        cin >> K;
        vector<long long> files(K+1), prefixSum(K+1, 0LL);
        for(int i = 1; i <= K; i++){
            cin >> files[i];
            prefixSum[i] = prefixSum[i-1] + files[i];
        }

        // dp[i][j] : 파일 i부터 j까지 하나로 합치는 최소 비용
        static long long dp[5001][5001];
        // opt[i][j] : dp[i][j]를 만족하는 분할점(최적 k)
        static int opt[5001][5001];

        // 길이가 1인 구간(자기 자신)은 비용이 0
        for(int i = 1; i <= K; i++){
            for(int j = 1; j <= K; j++){
                dp[i][j] = 0LL;
                opt[i][j] = i;  // 일단 초기화
            }
        }

        // 구간의 길이를 2부터 K까지 순차적으로 늘려가며 dp 계산
        for(int length = 2; length <= K; length++){
            for(int i = 1; i + length - 1 <= K; i++){
                int j = i + length - 1;
                
                // 구간 [i..j]를 합치는 비용은
                // dp[i][k] + dp[k+1][j] + (구간 [i..j] 파일 크기 합)
                // 여기서 sum[i..j] = prefixSum[j] - prefixSum[i-1]
                // - Knuth Optimization: 이전 단계에서 구한 최적 분할점 범위를 활용
                //   opt[i][j-1] <= opt[i][j] <= opt[i+1][j]
                
                // 먼저 탐색 범위를 opt[i][j-1] ~ opt[i+1][j] 로 제한
                int left = opt[i][j-1];
                int right = opt[i+1][j];
                if (right == 0) right = j-1; // 혹시나 j-1을 벗어나지 않도록
                
                long long cost = LLONG_MAX;
                int bestK = -1;
                for(int k = left; k <= right; k++){
                    long long temp = dp[i][k] + dp[k+1][j];
                    if(temp < cost){
                        cost = temp;
                        bestK = k;
                    }
                }
                
                dp[i][j] = cost + (prefixSum[j] - prefixSum[i-1]);
                opt[i][j] = bestK;
            }
        }

        cout << dp[1][K] << "\n";
    }

    return 0;
}
