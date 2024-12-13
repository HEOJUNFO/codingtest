#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m; 
    cin >> n >> m;

    vector<vector<pair<int,int>>> adj(n+1); // adj[u] = {(v, w), ...}
    vector<int> indegree(n+1, 0);
    vector<vector<int>> reverse_adj(n+1); // 역방향 그래프

    for(int i = 0; i < m; i++){
        int u,v,w;
        cin >> u >> v >> w;
        adj[u].push_back({v,w});
        indegree[v]++;
        reverse_adj[v].push_back(u);
    }

    int start, end; 
    cin >> start >> end;

    // 1. 위상 정렬
    queue<int> q;
    for (int i = 1; i <= n; i++){
        if (indegree[i] == 0) q.push(i);
    }

    vector<int> dist(n+1, -1);
    dist[start] = 0;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (auto &nx : adj[u]) {
            int v = nx.first;
            int w = nx.second;
            if (dist[u] != -1 && dist[v] < dist[u] + w) {
                dist[v] = dist[u] + w;
            }
            indegree[v]--;
            if (indegree[v] == 0) q.push(v);
        }
    }

    // dist[end]가 임계경로 길이
    cout << dist[end] << "\n";

    // 2. 임계경로에 포함되는 간선 수 찾기
    // dist[u] + w = dist[v]를 만족하면 임계경로상의 간선
    vector<int> chk(n+1, 0); // 방문 체크
    int count_edge = 0;

    // 도착 도시에서 역추적 (BFS 또는 DFS)
    queue<int> rq;
    rq.push(end);
    chk[end] = 1;

    while(!rq.empty()){
        int v = rq.front(); rq.pop();
        for (auto u : reverse_adj[v]) {
            // 임계경로에 해당되는지 검사
            for (auto &edge : adj[u]) {
                if (edge.first == v && dist[u] + edge.second == dist[v]) {
                    count_edge++;
                    if (!chk[u]) {
                        chk[u] = 1;
                        rq.push(u);
                    }
                }
            }
        }
    }

    cout << count_edge << "\n";

    return 0;
}
