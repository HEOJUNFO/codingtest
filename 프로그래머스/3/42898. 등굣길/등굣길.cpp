#include <string>
#include <vector>
using namespace std;

int solution(int m, int n, vector<vector<int>> puddles) {
    // 문제에서 요구하는 나머지
    const int MOD = 1000000007;

    // dp[y][x] : (1,1)에서 (x,y)에 도달하는 최단 경로의 수
    // n: 세로(행), m: 가로(열)
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, 0));

    // 물에 잠긴 지역 표시용
    // puddles에 들어있는 좌표는 (x, y) 형태로 주어짐
    for (auto &p : puddles) {
        int px = p[0];
        int py = p[1];
        // 물에 잠긴 지점은 -1 등으로 표시 (dp 계산 시 0으로 처리)
        dp[py][px] = -1;
    }

    // 시작점 초기화
    dp[1][1] = 1;

    // DP를 이용해 경로의 수를 계산
    for (int y = 1; y <= n; y++) {
        for (int x = 1; x <= m; x++) {
            // 시작점은 이미 1로 설정했으므로 건너뜀
            if (y == 1 && x == 1) continue;
            // 물에 잠긴 지역이면 0으로
            if (dp[y][x] == -1) {
                dp[y][x] = 0;
                continue;
            }

            long long left = 0, up = 0;
            // 왼쪽에서 오는 경우
            if (x - 1 >= 1 && dp[y][x - 1] != -1) {
                left = dp[y][x - 1];
            }
            // 위에서 오는 경우
            if (y - 1 >= 1 && dp[y - 1][x] != -1) {
                up = dp[y - 1][x];
            }
            dp[y][x] = (left + up) % MOD;
        }
    }

    // 학교에 도착하는 최단경로의 개수를 반환
    return dp[n][m] % MOD;
}
