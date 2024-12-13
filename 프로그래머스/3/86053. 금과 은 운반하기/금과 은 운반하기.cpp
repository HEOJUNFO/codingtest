#include <bits/stdc++.h>
using namespace std;

bool can_transport_in_time(long long mid, long long a, long long b,
                           const vector<long long> &g, const vector<long long> &s,
                           const vector<long long> &w, const vector<long long> &t) {
    long long total_gold = 0, total_silver = 0, total_combined = 0;
    int n = (int)g.size();

    for (int i = 0; i < n; i++) {
        long long max_trips = mid / (2LL * t[i]);
        long long remainder_trip = (mid / t[i]) % 2; // 편도 추가 운반 여부
        
        long long capacity = (max_trips + remainder_trip) * w[i];
        
        long long max_gold = min(g[i], capacity);
        long long max_silver = min(s[i], capacity);
        long long max_both = min(g[i] + s[i], capacity);

        total_gold += max_gold;
        total_silver += max_silver;
        total_combined += max_both;

        // 최적화: 중간에 이미 모자라면 더 볼 필요 없이 종료
        if (total_gold >= a && total_silver >= b && total_combined >= (a + b)) {
            // 조기 종료
            return true;
        }
    }

    return total_gold >= a && total_silver >= b && total_combined >= (a + b);
}

long long solution(int A, int B, vector<int> G, vector<int> S, vector<int> W, vector<int> T) {
    long long a = A, b = B;
    int n = (int)G.size();

    vector<long long> g(n), s(n), w_(n), t_(n);
    for (int i = 0; i < n; i++) {
        g[i] = G[i];
        s[i] = S[i];
        w_[i] = W[i];
        t_[i] = T[i];
    }

    long long left = 0;
    long long right = 1000000000000000LL; // 10^15, 충분히 큰 상한
    long long answer = right;

    while (left < right) {
        long long mid = (left + right) / 2;
        if (can_transport_in_time(mid, a, b, g, s, w_, t_)) {
            answer = min(answer, mid);
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return answer;
}

// 테스트 예제
int main() {
    {
        int A = 10, B = 10;
        vector<int> G = {100};
        vector<int> S = {100};
        vector<int> W = {7};
        vector<int> T = {10};
        cout << solution(A, B, G, S, W, T) << "\n"; // 50
    }

    {
        int A = 90, B = 500;
        vector<int> G = {70,70,0};
        vector<int> S = {0,0,500};
        vector<int> W = {100,100,2};
        vector<int> T = {4,8,1};
        cout << solution(A, B, G, S, W, T) << "\n"; // 499
    }

    return 0;
}
