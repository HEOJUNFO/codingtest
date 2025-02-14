#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
 
struct Point {
    int x, y, color;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    vector<Point> points(N);
    for (int i = 0; i < N; i++){
        cin >> points[i].x >> points[i].y >> points[i].color;
        points[i].color--;  // 색깔을 0-indexed로 변환
    }
    
    // x 좌표 기준 오름차순 정렬
    sort(points.begin(), points.end(), [](const Point &a, const Point &b){
        return a.x < b.x;
    });
    
    // 이진 탐색: 정사각형의 한 변의 길이를 후보 mid로 가정
    int lo = 0, hi = 250000;
    int ans = hi;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;  // 후보 정사각형 한 변의 길이
        bool possible = false;
        
        // x 좌표에 대해 슬라이딩 윈도우 사용
        int j = 0;
        vector<int> cnt(K, 0);
        int distinct = 0;
        for (int i = 0; i < N; i++){
            // 현재 i번 점의 x좌표를 왼쪽 경계로 할 때, x좌표가 [points[i].x, points[i].x+mid]에 있는 점들을 포함
            while (j < N && points[j].x <= points[i].x + mid) {
                if (cnt[points[j].color] == 0)
                    distinct++;
                cnt[points[j].color]++;
                j++;
            }
            // 현재 윈도우에 모든 색이 포함되어 있다면, y 방향으로도 검사
            if (distinct == K) {
                // x 윈도우에 속하는 점들을 복사하여 y좌표 기준으로 정렬
                vector<pair<int,int>> window;
                for (int k = i; k < j; k++){
                    window.push_back({points[k].y, points[k].color});
                }
                sort(window.begin(), window.end());
                
                // y 좌표에 대해 슬라이딩 윈도우를 사용하여,
                // 윈도우의 y 차이가 mid 이하인 구간에서 모든 색이 등장하는지 확인
                int p = 0;
                vector<int> cntY(K, 0);
                int d2 = 0;
                for (int q = 0; q < (int)window.size(); q++){
                    int y_val = window[q].first;
                    int c = window[q].second;
                    if (cntY[c] == 0)
                        d2++;
                    cntY[c]++;
                    while (p <= q && window[q].first - window[p].first > mid) {
                        cntY[window[p].second]--;
                        if (cntY[window[p].second] == 0)
                            d2--;
                        p++;
                    }
                    if (d2 == K) {  // 모든 색이 포함되면 후보 mid로 가능한 정사각형 존재
                        possible = true;
                        break;
                    }
                }
                if (possible) break;
            }
            // 다음 x 윈도우로 넘어가기 위해 i번째 점 제거
            cnt[points[i].color]--;
            if (cnt[points[i].color] == 0)
                distinct--;
        }
        
        if (possible) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    
    cout << ans << "\n";
    return 0;
}
