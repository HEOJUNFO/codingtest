#include <vector>
using namespace std;

static const int MOD = 20170805;

int solution(int m, int n, vector<vector<int>> city_map) 
{
    // dpR[i][j] = (i,j)에 '오른쪽에서' 들어온 경우의 수
    // dpD[i][j] = (i,j)에 '위에서' 들어온 경우의 수
    vector<vector<int>> dpR(m, vector<int>(n, 0));
    vector<vector<int>> dpD(m, vector<int>(n, 0));

    // 시작점 (0,0)
    dpR[0][0] = 1; // "출발"을 '오른쪽 이동' 1건으로 치든, dpD로 치든 상관없음(둘 중 하나만 1)
    dpD[0][0] = 0;

    // 첫 행 초기화(i=0, j=1..n-1)
    for(int j=1; j<n; j++){
        // (0,j)가 통행 불가면 dp=0
        if(city_map[0][j] == 1) {
            dpR[0][j] = 0;
            dpD[0][j] = 0;
            continue;
        }

        // 오른쪽에서 (0,j)에 올 때 => 왼쪽 칸(0,j-1) 확인
        if(city_map[0][j-1] == 1) {
            dpR[0][j] = 0;
        } else if(city_map[0][j-1] == 2) {
            // 이전 칸이 2 -> 꺾지 않고 그대로 가야 하므로 dpR만 이어받음
            dpR[0][j] = dpR[0][j-1] % MOD;
        } else { 
            // 이전 칸이 0
            dpR[0][j] = (dpR[0][j-1] + dpD[0][j-1]) % MOD; 
        }

        // 첫 행에서는 위(i-1)가 없음 => dpD[0][j] = 0
        dpD[0][j] = 0;
    }

    // 첫 열 초기화(j=0, i=1..m-1)
    for(int i=1; i<m; i++){
        if(city_map[i][0] == 1) {
            dpR[i][0] = 0;
            dpD[i][0] = 0;
            continue;
        }

        // 아래쪽에서 (i,0)에 올 때 => 위 칸(i-1,0) 확인
        if(city_map[i-1][0] == 1) {
            dpD[i][0] = 0;
        } else if(city_map[i-1][0] == 2) {
            dpD[i][0] = dpD[i-1][0] % MOD;
        } else {
            dpD[i][0] = (dpR[i-1][0] + dpD[i-1][0]) % MOD;
        }

        // 첫 열에서는 왼쪽(j-1)이 없음 => dpR[i][0] = 0
        dpR[i][0] = 0;
    }

    // 나머지 내부 칸
    for(int i=1; i<m; i++){
        for(int j=1; j<n; j++){
            // 통행 불가(1)
            if(city_map[i][j] == 1){
                dpR[i][j] = 0;
                dpD[i][j] = 0;
                continue;
            }

            // 오른쪽 이동 (i, j-1) -> (i, j)
            if(city_map[i][j-1] == 1) {
                dpR[i][j] = 0;
            }
            else if(city_map[i][j-1] == 2){
                // 직진만(이전 칸이 2이면 방향 전환 불가)
                dpR[i][j] = dpR[i][j-1] % MOD;
            }
            else { // 0
                dpR[i][j] = ((long long)dpR[i][j-1] + dpD[i][j-1]) % MOD;
            }

            // 아래쪽 이동 (i-1, j) -> (i, j)
            if(city_map[i-1][j] == 1) {
                dpD[i][j] = 0;
            }
            else if(city_map[i-1][j] == 2){
                dpD[i][j] = dpD[i-1][j] % MOD;
            }
            else { // 0
                dpD[i][j] = ((long long)dpR[i-1][j] + dpD[i-1][j]) % MOD;
            }
        }
    }

    // 최종 결과
    int answer = ( (long long)dpR[m-1][n-1] + dpD[m-1][n-1] ) % MOD;
    return answer;
}

