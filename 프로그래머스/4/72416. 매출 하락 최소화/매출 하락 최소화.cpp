#include <bits/stdc++.h>
using namespace std;

vector<int> sales_global;
vector<vector<int>> adj;
vector<vector<int>> dp;
vector<bool> visited;

// dp[v][0] : v를 선택하지 않았을 때 v 서브트리 커버 최소비용
// dp[v][1] : v를 선택했을 때 v 서브트리 커버 최소비용

int N;

void dfs(int v) {
    dp[v][0] = 0;
    dp[v][1] = sales_global[v-1]; // v는 1부터 시작, sales는 0부터 시작하므로 v-1

    if (adj[v].empty()) {
        // leaf node
        // DP[v][0] = 0 (already set)
        // DP[v][1] = sales[v] (already set)
        return;
    }

    int sum0 = 0; 
    int sum1 = sales_global[v-1]; // dp[v][1]에서 자식 가공 전
    bool allZeroBetter = true; // 모든 자식에 대해 DP[c][0] <= DP[c][1]인가?
    int diff_min = INT_MAX; // DP[c][1]-DP[c][0]의 최소값 찾기 (at least one child chosen scenario)

    for (auto c : adj[v]) {
        dfs(c);
        // dp[v][1] 계산
        // 자식 c에 대해 min(dp[c][0], dp[c][1])
        sum1 += min(dp[c][0], dp[c][1]);

        // dp[v][0] 계산을 위한 준비
        int child_min = min(dp[c][0], dp[c][1]);
        dp[v][0] += child_min;

        if (dp[c][0] < dp[c][1]) {
            // 자식 c의 경우 DP[c][0]이 DP[c][1]보다 작다는 것은
            // 이 자식의 서브트리에서 선택된 노드가 DP[c][0] 상태에서
            // 없을 수도 있다는 의미.
            // (리프일 경우 DP[c][0] =0 으로 아무도 안 선택)
            // 일단은 allZeroBetter 유지
        } else {
            // DP[c][1] <= DP[c][0] 인 자식이 하나라도 있으면
            // 최소 한 자식은 DP[c][1] 상태가 더 낫거나 같음.
            allZeroBetter = false;
        }

        // DP[c][1] - DP[c][0] 확인
        if (dp[c][1] > dp[c][0])
            diff_min = min(diff_min, dp[c][1] - dp[c][0]);
    }

    dp[v][1] = sum1;

    // v를 선택하지 않았는데 v가 팀장이면(자식이 있음) 최소 한 명은 자식 서브트리 중 선택되어야 함
    // allZeroBetter == true라면 자식들 전부 DP[c][0]으로 선택했다는 의미
    // 이 경우 diff_min을 통해 한 명을 DP[c][1]로 강제해야 함
    if (allZeroBetter) {
        // 모든 자식에 대해 dp[c][0] <= dp[c][1]
        // 그런데 한 명도 선택 안 한 상태일 수 있으므로(특히 리프일 때 dp[c][0]=0),
        // 최소 하나는 dp[c][1]로 바꿔야 한다.
        // diff_min이 갱신되지 않았다면, 자식 중 dp[c][1]-dp[c][0]이 존재하지 않을 수 있는데
        // 자식이 모두 리프이고 dp[c][0]=0, dp[c][1]>=0 일 때 diff_min은 dp[c][1]-0이 될 것.
        // 만약 자식이 전부 dp[c][0]<dp[c][1]이라면 diff_min은 INT_MAX 그대로일 것인데
        // 그런 경우는 dp[c][0]<dp[c][1]이지만 dp[c][1]>dp[c][0]이므로 diff_min 업데이트 되어야 함.
        // 즉, 논리상 diff_min은 반드시 갱신되어야 하는 상황.

        // 혹시 모를 안전 처리를 위해 diff_min == INT_MAX면
        // 자식 하나는 dp[c][1] 상태를 강제로 선택해야 하므로
        // 이 경우 자식중 하나에서 DP[c][1]-DP[c][0]를 구해 최소값을 찾는다.
        if (diff_min == INT_MAX) {
            diff_min = INT_MAX;
            for (auto c : adj[v]) {
                if (dp[c][1] >= dp[c][0]) {
                    diff_min = min(diff_min, dp[c][1]-dp[c][0]);
                }
            }
        }

        if (diff_min != INT_MAX) {
            dp[v][0] += diff_min;
        }
    }
}

int solution(vector<int> sales, vector<vector<int>> links) {
    N = (int)sales.size();
    sales_global = sales;
    adj.assign(N+1, {});

    // 트리 구성
    for (auto &link : links) {
        int a = link[0], b = link[1];
        adj[a].push_back(b);
    }

    dp.assign(N+1, vector<int>(2, 0));

    dfs(1);

    return min(dp[1][0], dp[1][1]);
}
