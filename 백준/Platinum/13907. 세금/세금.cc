#include <bits/stdc++.h>
using namespace std;

const int INF = 2000000000;
static int dist[1001][1001];
struct Edge {
    int to, cost;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K;
    cin >> N >> M >> K;
    int S, D;
    cin >> S >> D;
    S--; D--;

    vector<vector<Edge>> graph(N);
    for (int i = 0; i < M; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        a--; b--;
        graph[a].push_back({b, w});
        graph[b].push_back({a, w});
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = INF;
        }
    }

    // 다익스트라 (상태: (node, edge_count))
    dist[S][0] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    // 상태 인코딩: node + edge_count * 1000
    pq.push({0, S}); // cost=0, encoded_state=S(간선수=0)
    
    while(!pq.empty()) {
        int currC = pq.top().first;
        int encoded = pq.top().second;
        pq.pop();
        int currN = encoded % 1000;
        int edge_count = encoded / 1000;

        if (dist[currN][edge_count] < currC) continue;
        if (edge_count >= N-1) continue; // N-1 초과 간선은 고려할 필요 없음

        for (auto &ed : graph[currN]) {
            int nextN = ed.to;
            int nextC = ed.cost + currC;
            int next_edge_count = edge_count + 1;
            if (next_edge_count < N && dist[nextN][next_edge_count] > nextC) {
                dist[nextN][next_edge_count] = nextC;
                pq.push({nextC, nextN + next_edge_count * 1000});
            }
        }
    }

    // 초기 세금(인상 전)
    int total_tax = 0;
    {
        int ans = INF;
        for (int i = 0; i < N; i++) {
            if (dist[D][i] == INF) continue;
            ans = min(ans, dist[D][i] + i * total_tax);
        }
        cout << ans << '\n';
    }

    // 세금 인상 K번
    for (int i = 0; i < K; i++) {
        int p; cin >> p;
        total_tax += p;

        int ans = INF;
        for (int j = 0; j < N; j++) {
            if (dist[D][j] == INF) continue;
            int cost_with_tax = dist[D][j] + j * total_tax;
            ans = min(ans, cost_with_tax);
        }
        cout << ans << '\n';
    }

    return 0;
}
