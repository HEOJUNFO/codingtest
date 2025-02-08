#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    
    // dp 배열은 0~N+1까지 (더미 위치 포함)
    // lv[i], rv[i]는 dp 전이 시 사용할 범위를 나타냅니다.
    vector<int> lv(N + 2, 0), rv(N + 2, 0);
    // 초기값: rv[i] = i-1 (i번 위치에서 이전에 사용할 수 있는 최대 인덱스는 i-1)
    for (int i = 0; i <= N+1; i++) {
        rv[i] = i - 1;
    }
    
    // 각 사진의 정보: 사진 i에는 [a, b] 범위의 소들이 포함되어 있으며, 
    // 사진마다 "정확히 한 마리의 점박이 소"가 있어야 하므로
    // 전이 상태에 제약이 발생합니다.
    // 각 사진 (a, b)를 읽으면서, 해당 제약이 dp 전이에 반영되도록 lv와 rv를 갱신합니다.
    for (int i = 0; i < M; i++){
        int a, b;
        cin >> a >> b;
        // 사진의 조건은 “a번부터 b번 소 중 한 마리만 점찍어야 한다”입니다.
        // 이를 dp 전이에서 처리하기 위해, b+1번 위치에서는 이전 상태로부터 적어도 a번 이상의 인덱스만 올 수 있고,
        // b번 위치에서는 이전 상태의 인덱스가 최대 a-1까지여야 함을 의미합니다.
        lv[b + 1] = max(lv[b + 1], a);
        rv[b] = min(rv[b], a - 1);
    }
    // 누적 최대값 갱신: 각 위치 i에서 lv[i]는 이전 lv 값과 비교해 최대값으로 유지
    for (int i = 1; i <= N + 1; i++){
        lv[i] = max(lv[i], lv[i - 1]);
    }
    // rv도 오른쪽부터 누적 최소값으로 갱신
    for (int i = N; i >= 1; i--){
        rv[i] = min(rv[i], rv[i + 1]);
    }
    
    // dp[i]: 0~i까지 소들에 대해 valid하게 점찍었을 때 (dp 전이로 이어진) 최대 점찍은 소의 개수.
    // dp[0]를 시작 상태로 둡니다.
    vector<int> dp(N + 2, -1);
    dp[0] = 0;
    
    // 슬라이딩 윈도우 최대값을 위한 deque, 인덱스를 저장합니다.
    deque<int> dQ;
    dQ.push_back(0);
    int p1 = 0, p2 = 0;
    
    // i = 1부터 N+1까지 처리 (N+1은 더미 위치)
    for (int i = 1; i <= N + 1; i++){
        int L = lv[i], R = rv[i];
        
        // [p2, R] 범위에 해당하는 dp 인덱스들을 deque에 삽입 (단, 단조 감소 형태로 유지)
        while (p2 <= R) {
            while (!dQ.empty() && dp[p2] > dp[dQ.back()]) {
                dQ.pop_back();
            }
            dQ.push_back(p2);
            p2++;
        }
        // [p1, L-1]에 해당하는 오래된 값은 deque에서 제거
        while (!dQ.empty() && p1 < L) {
            if (dQ.front() == p1) {
                dQ.pop_front();
            }
            p1++;
        }
        // 만약 범위가 무효하다면 dp[i] = -1 (전이 불가능)
        if (L > R) {
            dp[i] = -1;
            continue;
        } else {
            if (dp[dQ.front()] == -1)
                dp[i] = -1;
            else
                dp[i] = dp[dQ.front()] + 1;
        }
    }
    
    // 더미 위치(N+1)에서 dp값이 -1이면 불가능한 할당이므로 -1 출력
    if (dp[N+1] == -1) {
        cout << -1 << "\n";
    } else {
        // dp[N+1]에는 더미로 한 번 추가된 상태이므로 실제 답은 dp[N+1]-1
        cout << dp[N+1] - 1 << "\n";
    }
    
    return 0;
}
