#include <bits/stdc++.h>
using namespace std;
 
// 물 높이의 최대값 (0~1000)
const int MAXH = 1000;
 
// globalArea[L] : 모든 구간에서 물 높이 L일 때 채워진 넓이의 합.
static double globalArea[MAXH+1];
 
// 구간 (두 점 a, b)의 기여를 물 높이 L에 대해 globalArea에 더하거나 빼는 함수
// mult = +1 (추가) 또는 -1 (빼기)
inline void updateSegmentContribution(int a, int b, int mult) {
    int low = a, high = b;
    if(b < a){
        low = b; high = a;
    }
    // L이 low 이하일 땐 기여는 0이므로, L = low+1 부터 처리.
    int start = low + 1;
    if(start > MAXH) return;
 
    if(a == b) {
        // 평평한 구간: 기여는 L - a (단, L > a)
        for (int L = start; L <= MAXH; L++){
            globalArea[L] += mult * ( (double)L - a );
        }
    } else {
        int d = high - low; // d > 0
        // L in (low, high) : 기여는 (L - low)^2 / (2*d)
        int midEnd = min(high - 1, MAXH);
        for (int L = start; L <= midEnd; L++){
            double diff = L - low;
            double val = (diff * diff) / (2.0 * d);
            globalArea[L] += mult * val;
        }
        // L >= high : 기여는 L - (a+b)/2.0
        int Lbegin = max(high, start);
        for (int L = Lbegin; L <= MAXH; L++){
            double val = L - ((a + b) / 2.0);
            globalArea[L] += mult * val;
        }
    }
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    vector<int> H(N);
    for (int i = 0; i < N; i++){
        cin >> H[i];
    }
 
    // globalArea 초기화
    for (int i = 0; i <= MAXH; i++){
        globalArea[i] = 0.0;
    }
 
    // 초기 구간 (0~N-2) 각각에 대해 기여함수를 globalArea에 누적
    for (int i = 0; i < N-1; i++){
        updateSegmentContribution(H[i], H[i+1], +1);
    }
 
    // M번의 명령 처리
    // Q h : 물 높이를 h로 설정 – globalArea[h]를 3자리 소수로 출력
    // U i h : H[i]를 h로 변경. 이때 인접한 구간(있다면)을 업데이트.
    for (int op = 0; op < M; op++){
        char type;
        cin >> type;
        if (type == 'Q'){
            int waterLevel;
            cin >> waterLevel;
            cout << fixed << setprecision(3) << globalArea[waterLevel] << "\n";
        } else if (type == 'U'){
            int idx, newVal;
            cin >> idx >> newVal;
            int oldVal = H[idx];
            // idx와 인접한 구간 업데이트
            if (idx > 0) {
                // 구간 (H[idx-1], H[idx])
                updateSegmentContribution(H[idx-1], oldVal, -1);
                updateSegmentContribution(H[idx-1], newVal, +1);
            }
            if (idx < N-1) {
                // 구간 (H[idx], H[idx+1])
                updateSegmentContribution(oldVal, H[idx+1], -1);
                updateSegmentContribution(newVal, H[idx+1], +1);
            }
            H[idx] = newVal;
        }
    }
 
    return 0;
}
