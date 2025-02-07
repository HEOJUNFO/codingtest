#include <bits/stdc++.h>
using namespace std;
 
// 전역 변수
int N;
// 각 노드에 대해 (인접 노드, 도로 가중치) 저장
vector<vector<pair<int,int>>> adj;
// 각 시작점 s에 대해 최종 점수 S[s]와 리셋 횟수 C[s]
vector<long long> S, C;
 
// DFS 함수 (starting point: start)
// cur: 현재 노드, par: 부모(역방향 방문 방지)
// curScore: s에서 cur까지 이동하며 현재 누적된 점수 (항상 >= 0)
// curResets: s에서 cur까지 이동하며 지금까지 리셋한 횟수
void dfs(int start, int cur, int par, long long curScore, long long curResets) {
    // 시작점이 아니면 결과 누적
    if(cur != start) {
        S[start] += curScore;
        C[start] += curResets;
    }
    // 인접한 노드를 순회
    for(auto &edge : adj[cur]){
        int nxt = edge.first;
        int w = edge.second;
        if(nxt == par) continue;
        long long nextScore = curScore + w;
        long long nextResets = curResets;
        // 점수가 음수가 되면 즉시 0으로 만들고 리셋횟수 증가
        if(nextScore < 0){
            nextScore = 0;
            nextResets++;
        }
        dfs(start, nxt, cur, nextScore, nextResets);
    }
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> N;
    // 노드는 1번부터 N번까지; 트리이므로 N-1개의 간선이 주어짐
    adj.assign(N+1, vector<pair<int,int>>());
    for (int i = 1; i <= N-1; i++){
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
 
    // S와 C 배열 초기화 (1번부터 N번까지 사용)
    S.assign(N+1, 0);
    C.assign(N+1, 0);
 
    // 각 노드를 시작점으로 하여 DFS 수행
    for (int i = 1; i <= N; i++){
        dfs(i, i, -1, 0LL, 0LL);
    }
 
    // 문제 출력 형식 – 둘 다 구한 경우 첫 줄에 1,
    // 그 다음 S[1..N] 그리고 C[1..N]을 출력한다.
    cout << 1 << "\n";
    for (int i = 1; i <= N; i++){
        cout << S[i] << (i == N ? "\n" : " ");
    }
    for (int i = 1; i <= N; i++){
        cout << C[i] << (i == N ? "\n" : " ");
    }
 
    return 0;
}
