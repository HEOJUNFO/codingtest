#include <bits/stdc++.h>
using namespace std;
 
// 최대 N = 1500이므로 여유 있게 1502×1502 크기의 전역 배열을 사용합니다.
static int Agrid[1502][1502]; // 각 격자 칸의 clam 최대 개수
static int dp[1502][1502];     // dp[i][j] = (0,0)에서 (i,j)까지 최대 clam 수 (이동은 오른쪽과 아래쪽이 반대임)
// lastEnq[i][j]는 해당 셀이 마지막으로 큐에 들어간 업데이트의 타임스탬프입니다.
static int lastEnq[1502][1502];
 
// 전파를 위한 셀 구조체 (행, 열)
struct Cell {
    int i, j;
};
 
// main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            cin >> Agrid[i][j];
        }
    }
 
    // 초기 dp 값 계산 (0-indexed)
    dp[0][0] = Agrid[0][0];
    long long totalSum = dp[0][0];
    // 첫 행: dp[0][j] = A[0][j] + dp[0][j-1]
    for (int j = 1; j < N; j++){
        dp[0][j] = Agrid[0][j] + dp[0][j-1];
        totalSum += dp[0][j];
    }
    // 첫 열: dp[i][0] = A[i][0] + dp[i-1][0]
    for (int i = 1; i < N; i++){
        dp[i][0] = Agrid[i][0] + dp[i-1][0];
        totalSum += dp[i][0];
    }
    // 나머지 셀들 : dp[i][j] = A[i][j] + max(dp[i-1][j], dp[i][j-1])
    for (int i = 1; i < N; i++){
        for (int j = 1; j < N; j++){
            dp[i][j] = Agrid[i][j] + max(dp[i-1][j], dp[i][j-1]);
            totalSum += dp[i][j];
        }
    }
 
    cout << totalSum << "\n";
 
    // 업데이트 명령은 총 N개가 주어집니다.
    int Q = N;
    // 전파를 위해 최대 N*N 개의 셀을 담을 큐(배열)를 사용합니다.
    int maxQueue = N * N;
    vector<Cell> que;
    que.resize(maxQueue);
    int head = 0, tail = 0;
 
    int curUpdate = 0; // 업데이트마다 증가시키는 타임스탬프
 
    for (int q = 0; q < Q; q++){
        char type;
        int r, c;
        cin >> type >> r >> c;
        // 입력은 1-indexed이므로 0-indexed로 변환합니다.
        r--; c--;
 
        curUpdate++; // 이번 업데이트의 타임스탬프
        int delta = (type == 'U' ? 1 : -1);
        Agrid[r][c] += delta;
 
        // 해당 셀 (r, c)의 dp 값을 다시 계산합니다.
        int newVal;
        if(r == 0 && c == 0){
            newVal = Agrid[0][0];
        } else if(r == 0){
            newVal = Agrid[r][c] + dp[r][c-1];
        } else if(c == 0){
            newVal = Agrid[r][c] + dp[r-1][c];
        } else {
            newVal = Agrid[r][c] + max(dp[r-1][c], dp[r][c-1]);
        }
 
        int oldVal = dp[r][c];
        if(newVal == oldVal){
            // 변화가 없으면 바로 전체합을 출력
            cout << totalSum << "\n";
            continue;
        }
 
        int diffVal = newVal - oldVal;
        dp[r][c] = newVal;
        totalSum += diffVal;
 
        // 전파를 위한 큐 초기화: (r, c)를 넣습니다.
        head = 0; tail = 0;
        que[tail++] = {r, c};
        lastEnq[r][c] = curUpdate;
 
        // (r,c)에서 시작해 오른쪽, 아래쪽으로 전파합니다.
        while(head < tail){
            Cell cur = que[head++];
            int i = cur.i, j = cur.j;
 
            // 오른쪽 이웃: (i, j+1)
            if(j + 1 < N){
                int ni = i, nj = j + 1;
                int candidate, computed;
                if(ni == 0){
                    // 첫 행이면 왼쪽 셀만 부모.
                    candidate = dp[ni][nj-1];
                    computed = Agrid[ni][nj] + candidate;
                } else {
                    candidate = max(dp[ni-1][nj], dp[ni][nj-1]);
                    computed = Agrid[ni][nj] + candidate;
                }
                if(computed != dp[ni][nj]){
                    int d = computed - dp[ni][nj];
                    dp[ni][nj] = computed;
                    totalSum += d;
                    if(lastEnq[ni][nj] != curUpdate){
                        lastEnq[ni][nj] = curUpdate;
                        que[tail++] = {ni, nj};
                    }
                }
            }
 
            // 아래쪽 이웃: (i+1, j)
            if(i + 1 < N){
                int ni = i + 1, nj = j;
                int candidate, computed;
                if(nj == 0){
                    candidate = dp[ni-1][nj];
                    computed = Agrid[ni][nj] + candidate;
                } else {
                    candidate = max(dp[ni-1][nj], dp[ni][nj-1]);
                    computed = Agrid[ni][nj] + candidate;
                }
                if(computed != dp[ni][nj]){
                    int d = computed - dp[ni][nj];
                    dp[ni][nj] = computed;
                    totalSum += d;
                    if(lastEnq[ni][nj] != curUpdate){
                        lastEnq[ni][nj] = curUpdate;
                        que[tail++] = {ni, nj};
                    }
                }
            }
        } // end while propagation
 
        cout << totalSum << "\n";
    }
 
    return 0;
}
