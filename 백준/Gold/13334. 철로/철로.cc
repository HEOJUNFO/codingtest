#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin >> n;
    vector<pair<long long, long long>> intervals(n);
    for (int i = 0; i < n; i++) {
        long long h, o; cin >> h >> o;
        if (h > o) swap(h, o);
        intervals[i] = {h, o};
    }
    long long d; cin >> d;

    // 길이가 d를 초과하는 구간 제거
    vector<pair<long long, long long>> candidates;
    candidates.reserve(n);
    for (auto &it : intervals) {
        if (it.second - it.first <= d) {
            candidates.push_back(it);
        }
    }

    // end 기준 오름차순 정렬
    sort(candidates.begin(), candidates.end(), [](auto &a, auto &b) {
        return a.second < b.second;
    });

    priority_queue<long long, vector<long long>, greater<long long>> pq; 
    // 우선순위 큐에는 start 값을 저장

    int answer = 0;
    // 스위핑
    for (auto &interval : candidates) {
        long long start = interval.first;
        long long end = interval.second;
        long long limit = end - d; // 가능한 선분의 시작 지점

        // 현재 interval의 start를 큐에 추가
        pq.push(start);

        // start가 (end - d)보다 작은 것들은 제거
        while (!pq.empty() && pq.top() < limit) {
            pq.pop();
        }

        // 현재 end를 기준으로 가능한 커버 가능한 구간 개수 확인
        int count = (int)pq.size();
        if (count > answer) answer = count;
    }

    cout << answer << "\n";
    return 0;
}
