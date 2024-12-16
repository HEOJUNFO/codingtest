#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N; cin >> N;
    vector<string> words(N);
    for (int i = 0; i < N; i++) cin >> words[i];

    // 1. 포함관계 제거: 어떤 단어가 다른 단어에 완전히 포함되어 있다면 제거
    {
        vector<bool> used(N, true);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i != j && used[i] && used[j]) {
                    if (words[j].find(words[i]) != string::npos) {
                        used[i] = false;
                    }
                }
            }
        }
        vector<string> filtered;
        for (int i = 0; i < N; i++) {
            if (used[i]) filtered.push_back(words[i]);
        }
        words = move(filtered);
        N = (int)words.size();
    }

    // N이 1인 경우 그냥 단어 출력 후 종료
    if (N == 1) {
        cout << words[0] << "\n";
        return 0;
    }

    // 2. 각 단어 i에서 j로 갈 때의 최대 겹침 길이를 precompute
    vector<vector<int>> overlap(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) continue;
            // words[i]의 suffix와 words[j]의 prefix 최대 매칭 길이
            int mx = min((int)words[i].size(), (int)words[j].size());
            for (int len = mx; len > 0; len--) {
                if (words[i].compare((int)words[i].size()-len, len, words[j], 0, len) == 0) {
                    overlap[i][j] = len;
                    break;
                }
            }
        }
    }

    // 3. dp[mask][last] : mask에 해당하는 단어들을 모두 포함하고, 마지막 단어가 last인 최단 슈퍼스트링
    // 문자열 길이도 중요하지만, 사전순까지 고려해야 하므로 dp에 문자열 자체를 저장
    // dp 상태 수: 2^N * N <= 4096(최대) 이므로 가능
    // 초기값: dp[1<<i][i] = words[i]
    // 점화식: dp[mask][j] = min_lex( dp[mask_without_i][i] + (words[j]의 overlap을 반영한 suffix) ) 

    // dp 배열을 문자열로 관리
    // 빈 문자열이면 아직 계산 안 된 상태
    vector<vector<string>> dp(1 << N, vector<string>(N, ""));

    for (int i = 0; i < N; i++) {
        dp[1 << i][i] = words[i];
    }

    for (int mask = 0; mask < (1 << N); mask++) {
        for (int i = 0; i < N; i++) {
            if (dp[mask][i].empty()) continue; // 아직 이 상태 없음
            // i에서 j로 갈 수 있는 경우
            for (int j = 0; j < N; j++) {
                if (mask & (1 << j)) continue; // 이미 j 사용
                int nextMask = mask | (1 << j);
                // i에서 j를 붙일 때 overlap[i][j] 만큼 겹침
                string candidate = dp[mask][i] + words[j].substr(overlap[i][j]);
                // dp[nextMask][j] 갱신
                if (dp[nextMask][j].empty()) {
                    dp[nextMask][j] = candidate;
                } else {
                    // 길이 비교 후 더 짧은 것, 같으면 사전순으로 앞선 것
                    if (candidate.size() < dp[nextMask][j].size()) {
                        dp[nextMask][j] = candidate;
                    } else if (candidate.size() == dp[nextMask][j].size()) {
                        if (candidate < dp[nextMask][j]) {
                            dp[nextMask][j] = candidate;
                        }
                    }
                }
            }
        }
    }

    // 모든 단어를 사용한 상태: mask = (1<<N)-1 에서 가장 짧고 사전순으로 앞서는 것 선택
    int fullMask = (1 << N) - 1;
    string ans;
    for (int i = 0; i < N; i++) {
        if (!dp[fullMask][i].empty()) {
            if (ans.empty()) ans = dp[fullMask][i];
            else {
                if (dp[fullMask][i].size() < ans.size()) ans = dp[fullMask][i];
                else if (dp[fullMask][i].size() == ans.size() && dp[fullMask][i] < ans) ans = dp[fullMask][i];
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
