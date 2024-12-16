#include <bits/stdc++.h>
using namespace std;

int n, s;
int dp[55][1010];

void f(int rem, int c) {
    // rem : 남은 노드 수
    // c : 현재까지 만들어진 길이가 2인 경로 수
    if (rem == 0 && c == s) {
        cout << 1;
        exit(0);
    }
    if (rem == 0 || dp[rem][c] || c > s) return;
    dp[rem][c] = 1;
    for (int i = 1; i <= rem; i++) {
        // i개의 노드를 하나의 leaf에 붙일 때 추가되는 경로 수는 i*(i+1)/2
        f(rem - i, c + i*(i+1)/2);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> s;
    // 처음 2개의 노드로 이루어진 트리를 생각 (n-2개의 노드 남음, 현재 경로 수 0)
    f(n - 2, 0);
    cout << 0 << "\n";
    return 0;
}
