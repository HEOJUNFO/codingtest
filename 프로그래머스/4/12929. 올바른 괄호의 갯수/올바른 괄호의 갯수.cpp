#include <string>
#include <vector>
using namespace std;

int solution(int n) {
    vector<long long> dp(n+1, 0);
    dp[0] = 1; // 빈 문자열(0쌍)에 대한 올바른 괄호 문자열의 수는 1

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }

    return (int)dp[n];
}
