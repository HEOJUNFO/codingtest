#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int dr[4] = {-1,0,1,0}; // 상, 우, 하, 좌 방향을 나타내는 행 변화
int dc[4] = {0,1,0,-1}; // 상, 우, 하, 좌 방향을 나타내는 열 변화
// 방향 정의: 0:위, 1:오른쪽, 2:아래, 3:왼쪽

vector<int> solution(vector<string> grid) {
    int n = (int)grid.size();
    int m = (int)grid[0].size();

    // visited[r][c][dir]:
    // 0: 방문 안함
    // 1: 현재 탐색중(스택 안)
    // 2: 탐색 완료(사이클 여부 확정)
    vector<vector<vector<int>>> visited(n, vector<vector<int>> (m, vector<int>(4, 0)));

    vector<int> result;

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            for (int d = 0; d < 4; d++) {
                if (visited[r][c][d] == 0) {
                    // 새로운 경로 탐색 시작
                    int cur_r = r;
                    int cur_c = c;
                    int cur_d = d;

                    // 경로 추적을 위한 스택 or 벡터
                    // 경로를 저장했다가 사이클 길이 계산 필요 시 사용
                    vector<array<int,3>> path;

                    while (true) {
                        if (visited[cur_r][cur_c][cur_d] == 2) {
                            // 이미 완전히 탐색된 상태 -> 사이클 없음
                            // 현재 시작점에서 나온 경로는 모두 사이클 없음으로 마무리
                            for (auto &st: path) {
                                visited[st[0]][st[1]][st[2]] = 2;
                            }
                            break;
                        }
                        if (visited[cur_r][cur_c][cur_d] == 1) {
                            // 현재 탐색중이던 경로에서 재방문 발생 => 사이클 형성
                            // 사이클 길이 계산
                            int cycle_len = 0;
                            bool start_counting = false;
                            for (auto &st: path) {
                                if (!start_counting && st[0]==cur_r && st[1]==cur_c && st[2]==cur_d) {
                                    start_counting = true;
                                }
                                if (start_counting) cycle_len++;
                            }
                            result.push_back(cycle_len);

                            // 경로의 모든 상태 방문 완료 처리
                            for (auto &st: path) {
                                visited[st[0]][st[1]][st[2]] = 2;
                            }
                            break;
                        }

                        // 현재 상태를 방문중으로 표시
                        visited[cur_r][cur_c][cur_d] = 1;
                        path.push_back({cur_r, cur_c, cur_d});

                        // 현재 칸에 따른 방향 변화
                        char cell = grid[cur_r][cur_c];
                        int nd = cur_d;
                        if (cell == 'L') {
                            nd = (cur_d + 3) % 4; // 좌회전
                        } else if (cell == 'R') {
                            nd = (cur_d + 1) % 4; // 우회전
                        } else {
                            // 'S'인 경우 방향 변화 없음
                            nd = cur_d;
                        }

                        // 다음 위치 계산 (토러스 형태로 wrap-around)
                        int nr = cur_r + dr[nd];
                        int nc = cur_c + dc[nd];
                        if (nr < 0) nr = n - 1;
                        else if (nr >= n) nr = 0;
                        if (nc < 0) nc = m - 1;
                        else if (nc >= m) nc = 0;

                        cur_r = nr;
                        cur_c = nc;
                        cur_d = nd;
                    }
                }
            }
        }
    }

    sort(result.begin(), result.end());
    return result;
}
