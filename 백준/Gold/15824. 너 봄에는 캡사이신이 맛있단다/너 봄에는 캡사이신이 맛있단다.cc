#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N; cin >> N;
    vector<long long> arr(N);
    for (int i = 0; i < N; i++) cin >> arr[i];

    sort(arr.begin(), arr.end());

    // 미리 2의 거듭제곱을 계산해둔다.
    // pow2[i] = 2^i % MOD
    vector<long long> pow2(N, 1);
    for (int i = 1; i < N; i++) {
        pow2[i] = (pow2[i-1] * 2) % MOD;
    }

    long long answer = 0;
    for (int i = 0; i < N; i++) {
        // arr[i]가 최대값이 되는 경우: +arr[i]*2^i
        // arr[i]가 최소값이 되는 경우: -arr[i]*2^(N-1-i)
        long long add_val = (arr[i] * pow2[i]) % MOD;
        long long sub_val = (arr[i] * pow2[N-1 - i]) % MOD;
        answer = (answer + add_val - sub_val) % MOD;
    }

    // 음수가 될 수 있으므로 MOD 더해주기
    if (answer < 0) answer += MOD;
    cout << answer << "\n";

    return 0;
}
