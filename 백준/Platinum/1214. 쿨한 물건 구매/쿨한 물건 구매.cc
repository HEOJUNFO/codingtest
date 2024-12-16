#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    long long D, P, Q;
    cin >> D >> P >> Q;

    // P를 더 큰 금액으로 조정
    if (P < Q) swap(P, Q);

    // P원 지폐만 사용했을 때 최소 비용
    long long min_cnt = D / P + 1;
    long long min_cost = min_cnt * P;

    // D가 P나 Q로 나누어 떨어지는 경우 즉시 D 출력 가능
    if (D % P == 0 || D % Q == 0) {
        cout << D << "\n";
        return 0;
    }

    // P원 지폐 수를 줄여가며 Q 지폐로 보충하는 경우 탐색
    for (long long t = min_cnt - 1; t >= 0; t--) {
        long long remain = D - (t * P);
        if (remain <= 0) {
            // 이 경우 D를 이미 초과했으므로 비용은 t*P
            min_cost = min(min_cost, t * P);
            continue;
        }
        long long div = remain / Q;
        long long mod = remain % Q;
        if (mod == 0) {
            // Q로 정확히 나눠떨어지므로 최소비용은 D
            min_cost = D;
            break;
        } else {
            // 나누어 떨어지지 않을 경우 한 장 더 필요
            long long temp = t * P + (div + 1) * Q;
            if (temp == min_cost) {
                // 더 이상의 개선 없음
                break;
            }
            min_cost = min(min_cost, temp);
        }
    }

    cout << min_cost << "\n";
    return 0;
}
