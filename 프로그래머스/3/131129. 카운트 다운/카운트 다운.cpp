#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

vector<int> solution(int target) {
    // 가능한 점수 계산
    vector<pair<int,bool>> throws; 
    // 싱글(1~20), 싱글이면 true
    for (int i = 1; i <= 20; i++) {
        throws.push_back({i, true});
    }
    // 더블(2~40)
    for (int i = 1; i <= 20; i++) {
        throws.push_back({i*2, false});
    }
    // 트리플(3~60)
    for (int i = 1; i <= 20; i++) {
        throws.push_back({i*3, false});
    }
    // 불(50), 싱글/불 카운트 증가 대상
    throws.push_back({50, true});
    
    // dp[i] = {최소 다트 수, 싱글/불 횟수 최댓값}
    // 최소 다트 수를 기준으로, 같을 경우 싱글/불 횟수가 많은 쪽 선택
    vector<pair<int,int>> dp(target+1, {numeric_limits<int>::max(), -1});
    dp[0] = {0,0};

    for (int i = 1; i <= target; i++) {
        for (auto &th : throws) {
            int score = th.first;
            bool isSingleOrBull = th.second;
            if (i - score >= 0 && dp[i - score].first != numeric_limits<int>::max()) {
                int candDarts = dp[i - score].first + 1;
                int candSingles = dp[i - score].second + (isSingleOrBull ? 1 : 0);
                
                if (candDarts < dp[i].first) {
                    dp[i].first = candDarts;
                    dp[i].second = candSingles;
                } else if (candDarts == dp[i].first && candSingles > dp[i].second) {
                    dp[i].second = candSingles;
                }
            }
        }
    }
    
    return {dp[target].first, dp[target].second};
}
