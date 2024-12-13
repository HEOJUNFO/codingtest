#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

long long solution(int n, vector<int> works) {
    long long answer = 0;

    // 작업량의 총합이 n 이하거나 작으면 모든 작업을 끝낼 수 있으므로 피로도는 0
    long long sum = 0;
    for (int w : works) {
        sum += w;
    }
    if (sum <= n) return 0;

    // 최대 힙 구성
    priority_queue<int> pq;
    for (int w : works) {
        pq.push(w);
    }

    // n 시간 동안 가장 큰 작업량부터 1씩 감소
    while (n > 0) {
        int top = pq.top();
        pq.pop();
        top = top - 1;
        if (top > 0) {
            pq.push(top);
        }
        n--;
    }

    // 남은 작업량들의 제곱 합 계산
    while (!pq.empty()) {
        long long w = pq.top();
        pq.pop();
        answer += w * w;
    }

    return answer;
}
