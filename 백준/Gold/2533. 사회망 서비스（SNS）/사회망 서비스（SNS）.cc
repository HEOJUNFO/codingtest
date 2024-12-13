#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 1000000;

int N;
vector<int> adj[MAXN+1];
int dp[MAXN+1][2]; 
bool visited[MAXN+1];

void dfs(int cur) {
    visited[cur] = true;
    dp[cur][0] = 0; // cur를 얼리어답터로 안함
    dp[cur][1] = 1; // cur를 얼리어답터로 함 (자기 자신 카운트)
    
    for (auto &next: adj[cur]) {
        if(!visited[next]) {
            dfs(next);
            dp[cur][0] += dp[next][1]; // cur가 얼리어답터가 아니면 자식은 반드시 얼리어답터
            dp[cur][1] += min(dp[next][0], dp[next][1]); // cur가 얼리어답터면 자식은 최소값을 선택
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    for (int i = 0; i < N-1; i++) {
        int u,v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1);

    cout << min(dp[1][0], dp[1][1]) << "\n";
    return 0;
}
