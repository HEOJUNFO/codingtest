#include <bits/stdc++.h>
using namespace std;

// ---------------------- Dinic 알고리즘 구현부 ----------------------
struct Edge {
    int to, rev;
    long long cap;
};

struct Dinic {
    vector<vector<Edge>> graph;
    vector<int> level, it;  // level: 레벨 그래프, it: 현재 탐색 중인 간선 인덱스

    Dinic(int n) {
        graph.resize(n);
        level.resize(n);
        it.resize(n);
    }

    // u->v 로 용량 c인 간선을 추가 (무방향처럼 쓰려면 v->u도 capacity c로 추가)
    void addEdge(int u, int v, long long c) {
        Edge a{v, (int)graph[v].size(), c};
        Edge b{u, (int)graph[u].size(), 0}; // 역방향 간선: 초기 용량 0
        graph[u].push_back(a);
        graph[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : graph[u]) {
                if (level[e.to] == -1 && e.cap > 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return (level[t] != -1);
    }

    long long sendFlow(int u, long long flow, int t) {
        if (!flow) return 0;
        if (u == t) return flow;

        for (int &i = it[u]; i < (int)graph[u].size(); i++) {
            auto &e = graph[u][i];
            if (level[e.to] == level[u] + 1 && e.cap > 0) {
                long long curFlow = min(flow, e.cap);
                long long tempFlow = sendFlow(e.to, curFlow, t);

                if (tempFlow > 0) {
                    // 잔여 용량 업데이트
                    e.cap -= tempFlow;
                    graph[e.to][e.rev].cap += tempFlow;
                    return tempFlow;
                }
            }
        }
        return 0;
    }

    long long maxFlow(int s, int t) {
        long long total = 0;
        // 레벨 그래프를 만들 수 있을 때마다 흐름을 보냄
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (long long flow = sendFlow(s, LLONG_MAX, t)) {
                total += flow;
            }
        }
        return total;
    }
};
// ---------------------- Dinic 알고리즘 구현 끝 ----------------------

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // forced[i] = 0 (자유), 1 (A 강제), 2 (B 강제)
    vector<int> forced(N+1, 0);
    for(int i = 1; i <= N; i++){
        cin >> forced[i];
    }

    // 슬픔 정도 w[i][j]
    vector<vector<int>> w(N+1, vector<int>(N+1, 0));
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= N; j++){
            cin >> w[i][j];
        }
    }

    // 정점: s = 0, 사람들 = 1..N, t = N+1
    int s = 0, t = N+1;
    Dinic dinic(t+1); // 그래프 크기: 0~(N+1)

    const long long INF = 1e10; // N 최대 500, w[i][j] 최대 1000, 합 고려해 넉넉히

    // 1. 강제 조건에 따른 간선 생성
    for(int i = 1; i <= N; i++){
        if(forced[i] == 1){
            // i는 무조건 A -> s->i 용량 무한대
            dinic.addEdge(s, i, INF);
        }
        else if(forced[i] == 2){
            // i는 무조건 B -> i->t 용량 무한대
            dinic.addEdge(i, t, INF);
        }
    }

    // 2. 사람 사이의 간선 생성
    //    i < j 로만 반복하며, w[i][j] > 0 일 때 양방향 간선 생성
    for(int i = 1; i <= N; i++){
        for(int j = i+1; j <= N; j++){
            if(w[i][j] > 0){
                // i->j, j->i 용량 = w[i][j]
                dinic.addEdge(i, j, w[i][j]);
                dinic.addEdge(j, i, w[i][j]);
            }
        }
    }

    // 3. 최대 유량(= 최소 컷) 구하기
    long long minCutValue = dinic.maxFlow(s, t);

    // 4. 최소 컷에 따른 A/B 분할 결정
    //    잔여 용량이 남아있는 간선을 통해 s에서 도달 가능한 정점 = A
    //    그 외 = B
    vector<bool> visited(t+1, false);
    // 잔여 그래프에서 BFS/DFS
    queue<int>q;
    q.push(s);
    visited[s] = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &e : dinic.graph[u]){
            if(e.cap > 0 && !visited[e.to]){
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }

    // 분할 결과 A/B 저장
    vector<int> A, B;
    for(int i = 1; i <= N; i++){
        if(visited[i]) A.push_back(i);
        else B.push_back(i);
    }

    // 5. 출력
    // 최소 슬픔 합
    cout << minCutValue << "\n";

    // A 진영
    if(!A.empty()){
        for(int i=0; i<(int)A.size(); i++){
            cout << A[i];
            if(i+1 < (int)A.size()) cout << " ";
        }
    }
    cout << "\n";

    // B 진영
    if(!B.empty()){
        for(int i=0; i<(int)B.size(); i++){
            cout << B[i];
            if(i+1 < (int)B.size()) cout << " ";
        }
    }
    cout << "\n";

    return 0;
}
