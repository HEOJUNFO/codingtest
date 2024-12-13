#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

long long ccw(const Point& A, const Point& B, const Point& C) {
    // CCW 판별을 위한 벡터 외적값 계산
    // (B - A) x (C - A)
    long long val = 1LL * (B.x - A.x) * (C.y - A.y) - 1LL * (B.y - A.y) * (C.x - A.x);
    return val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N; cin >> N;
    vector<Point> points(N);

    for (int i = 0; i < N; i++) {
        cin >> points[i].x >> points[i].y;
    }

    // x좌표 기준 정렬, x가 같으면 y가 작은 순으로
    sort(points.begin(), points.end(), [](const Point &A, const Point &B) {
        if (A.x == B.x) return A.y < B.y;
        return A.x < B.x;
    });

    // 볼록 껍질 구성: Andrew's monotone chain
    vector<Point> hull;

    // 하단 껍질
    for (int i = 0; i < N; i++) {
        while (hull.size() > 1 && ccw(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // 상단 껍질
    // 마지막 원소는 중복되므로 하나 빼고 시작
    for (int i = N - 2, t = (int)hull.size() + 1; i >= 0; i--) {
        while (hull.size() >= t && ccw(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // 마지막 점도 중복되므로 pop_back()
    hull.pop_back();

    cout << hull.size() << "\n";

    return 0;
}
