#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

int solution(int n, int s, int a, int b, vector<vector<int>> fares) {
    // 무한대값 정의
    const int INF = numeric_limits<int>::max()/2;
    
    // 거리 배열 초기화
    vector<vector<int>> dist(n+1, vector<int>(n+1, INF));
    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0;
    }
    
    // fares 정보 입력
    for (auto &fare : fares) {
        int c = fare[0];
        int d = fare[1];
        int f = fare[2];
        dist[c][d] = f;
        dist[d][c] = f;
    }
    
    // 플로이드 워셜
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    
    // 모든 k에 대해 s->k + k->a + k->b의 최소값 찾기
    int answer = INF;
    for (int k = 1; k <= n; k++) {
        if (dist[s][k] == INF || dist[k][a] == INF || dist[k][b] == INF) continue;
        answer = min(answer, dist[s][k] + dist[k][a] + dist[k][b]);
    }

    // 합승 없이 (s->a + s->b)도 확인 (필요한 경우)
    int noShare = dist[s][a] + dist[s][b];
    answer = min(answer, noShare);
    
    return answer;
}
