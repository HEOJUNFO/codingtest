#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, k;
    cin >> l >> k;

    static ll D[101]; // l 최대 100이므로 넉넉히 잡음
    // 초기값 설정
    if (1 <= l) D[1] = 1;   // 어두운 얇은 디스크 1개
    if (k <= l) D[k] = 1;   // 어두운 두꺼운 디스크 1개

    for (int i = 1; i <= l; i++) {
        // i에서 2 추가
        if (i + 2 <= l) D[i+2] += D[i];
        // i에서 k+1 추가
        if (i + k + 1 <= l) D[i+k+1] += D[i];
    }

    ll result = 0;
    for (int i = 1; i <= l; i++) {
        result += D[i];
    }

    cout << result << "\n";
    return 0;
}
