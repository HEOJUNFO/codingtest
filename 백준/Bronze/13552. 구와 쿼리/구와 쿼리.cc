#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N; cin >> N;
    vector<array<int, 3>> points(N);
    for (int i = 0; i < N; i++) {
        cin >> points[i][0] >> points[i][1] >> points[i][2];
    }

    int M; cin >> M;
    while (M--) {
        int x, y, z, r;
        cin >> x >> y >> z >> r;
        long long rPow = (long long)r * r;
        int cnt = 0;
        for (int i = 0; i < N; i++) {
            long long dx = points[i][0] - x;
            long long dy = points[i][1] - y;
            long long dz = points[i][2] - z;
            long long dist = dx*dx + dy*dy + dz*dz;
            if (dist <= rPow) cnt++;
        }
        cout << cnt << "\n";
    }

    return 0;
}
