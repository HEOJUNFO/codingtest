#include <vector>
#include <algorithm>
using namespace std;

long long play_game(long long N, int Q, long long K, vector<long long> L, vector<long long> R) {
    // A[i]가 바뀔 가능성이 있는 좌표(경계)를 모은다.
    // 0, N와 각 업데이트의 시작점 L[i], 그리고 (R[i]+1)가 후보이다.
    vector<long long> coords;
    coords.push_back(0);
    coords.push_back(N);
    for (int i = 0; i < Q; i++) {
        coords.push_back(L[i]);
        if (R[i] + 1 < N)
            coords.push_back(R[i] + 1);
    }
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    int M = coords.size();

    // 압축된 좌표에 대해 차분 배열을 이용하여 각 구간의 A값(누적 업데이트 횟수)를 계산.
    vector<long long> diff(M, 0);
    for (int i = 0; i < Q; i++) {
        int li = int(lower_bound(coords.begin(), coords.end(), L[i]) - coords.begin());
        diff[li] += 1;
        if (R[i] + 1 < N) {
            int ri = int(lower_bound(coords.begin(), coords.end(), R[i] + 1) - coords.begin());
            diff[ri] -= 1;
        }
    }
    for (int i = 1; i < M; i++) {
        diff[i] += diff[i - 1];
    }
    
    // 각 구간 [coords[i], coords[i+1])에서의 상수 A값을 segWeight[i]에 저장.
    // M-1번째 인덱스는 단일 점(N)이므로 사용하지 않음.
    vector<long long> segWeight(M, 0);
    for (int i = 0; i < M - 1; i++) {
        segWeight[i] = diff[i];
    }

    // dp[i] : 좌표 coords[i]부터 시작할 때 얻을 수 있는 최대 점수.
    // dp(x) = max( dp(x+1), A[x] + dp(x+K) ) 의 점화식을 압축된 좌표에서 계산한다.
    vector<long long> dp(M, 0);
    dp[M - 1] = 0; // dp(N) = 0.
    
    // 압축 좌표를 내림차순으로 처리.
    for (int i = M - 2; i >= 0; i--) {
        // "걷기" 옵션: 현재 구간을 건너뛰고 바로 다음 구간으로 이동.
        long long walkOption = dp[i + 1];

        // 현재 구간 [Lpos, Rpos)에서의 A값 (상수)
        long long Lpos = coords[i], Rpos = coords[i + 1];
        long long c = segWeight[i];
        
        // 현재 구간 내에서 연속 점프할 수 있는 최대 횟수 m = ceil((Rpos - Lpos) / K)
        long long m = (Rpos - Lpos + K - 1) / K;
        // m번 점프 후 착지 위치
        long long landing = Lpos + m * K;
        
        // landing 이상의 첫 압축 좌표 인덱스 찾기
        int idx = int(lower_bound(coords.begin(), coords.end(), landing) - coords.begin());
        long long jumpOption = (idx < M ? dp[idx] : 0) + c * m;

        dp[i] = (walkOption > jumpOption ? walkOption : jumpOption);
    }
    
    return dp[0];
}
