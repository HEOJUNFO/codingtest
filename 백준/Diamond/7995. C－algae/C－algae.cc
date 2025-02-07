#pragma GCC optimize("O2")
#include <bits/stdc++.h>
using namespace std;
 
// Global variables for the current test case.
int n;
vector<vector<int>> graph; // graph[v] holds the sorted list of neighbors of vertex v.
vector<bool> inS;        // used for marking membership in the current set S during recursion.
 
// Recursive function to check if the induced subgraph on vertices in S (sorted vector<int>) is a c-algae.
bool checkCograph(const vector<int>& S) {
    if (S.size() <= 1) return true; // trivial case
    
    // Mark vertices in S for O(1) membership checking.
    for (int v : S)
        inS[v] = true;
    
    // === Step 1. 원래 그래프에서 연결 요소 구하기 ===
    vector<bool> visited(n, false);
    vector<vector<int>> comps; // 각 연결 컴포넌트를 저장
    for (int v : S) {
        if (!visited[v]) {
            vector<int> comp;
            queue<int> q;
            q.push(v);
            visited[v] = true;
            while (!q.empty()){
                int cur = q.front(); q.pop();
                comp.push_back(cur);
                // cur의 인접 정점 중 S에 포함된 정점만 고려
                for (int nb : graph[cur]) {
                    if (inS[nb] && !visited[nb]) {
                        visited[nb] = true;
                        q.push(nb);
                    }
                }
            }
            sort(comp.begin(), comp.end());
            comps.push_back(comp);
        }
    }
    // S의 표시를 해제
    for (int v : S)
        inS[v] = false;
    
    // 만약 원래 그래프에서 분리되어 있다면, 각 컴포넌트를 재귀적으로 검사
    if (comps.size() > 1) {
        for (auto &comp : comps) {
            if (!checkCograph(comp))
                return false;
        }
        return true;
    }
    
    // === Step 2. S는 원래 그래프에서 연결되어 있음 ===
    // c-algae의 경우 연결 그래프라면 S의 보(complement) 그래프는 반드시 분리되어 있어야 한다.
    // 보 그래프의 연결 컴포넌트를 구하기 위해 "남은 정점 리스트(rem)" 기법을 사용한다.
    vector<bool> visitedC(n, false);  // 보 그래프에서 방문 체크
    vector<vector<int>> compsC;         // 보 그래프의 연결 컴포넌트 저장
    for (int v : S) {
        if (!visitedC[v]) {
            vector<int> comp;
            queue<int> q;
            q.push(v);
            visitedC[v] = true;
            // rem: 아직 BFS로 발견되지 않은 S의 정점들 (초기엔 v를 제외한 S)
            vector<int> rem;
            for (int u : S)
                if(u != v)
                    rem.push_back(u);
            // rem은 S가 정렬되어 있으므로 정렬된 상태임.
            while (!q.empty()){
                int cur = q.front(); q.pop();
                comp.push_back(cur);
                // rem에서 cur와 보(complement)로 인접한 정점들을 모두 찾아 제거한다.
                vector<int> newRem;
                for (int u : rem) {
                    // u가 cur와 인접하지 않으면, 즉 보 그래프에서는 인접하므로 BFS 확장 대상
                    if (!binary_search(graph[cur].begin(), graph[cur].end(), u)) {
                        if (!visitedC[u]) {
                            visitedC[u] = true;
                            q.push(u);
                        }
                        // u는 newRem에 넣지 않음 (이미 BFS로 발견됨)
                    } else {
                        // u는 cur와 원래 간선이 있으므로 보에서는 인접하지 않으므로 남겨둔다.
                        newRem.push_back(u);
                    }
                }
                rem = move(newRem);
            }
            sort(comp.begin(), comp.end());
            compsC.push_back(comp);
        }
    }
    // 보 그래프가 연결되어 있으면 조건에 어긋남.
    if (compsC.size() == 1)
        return false;
    // 보 그래프의 각 컴포넌트에 대해 재귀적으로 검사
    for (auto &comp : compsC) {
        if (!checkCograph(comp))
            return false;
    }
    return true;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    while (T--){
        int m;
        cin >> n >> m;
        graph.assign(n, vector<int>());
        for (int i = 0; i < m; i++){
            int a, b;
            cin >> a >> b;
            a--; b--;
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
        // 각 정점의 인접 리스트를 정렬 (binary_search를 위해)
        for (int i = 0; i < n; i++){
            sort(graph[i].begin(), graph[i].end());
        }
 
        // 전역 inS 벡터 초기화
        inS.assign(n, false);
 
        // S: 전체 정점 집합 (0~n-1)을 정렬된 vector로 준비
        vector<int> S(n);
        for (int i = 0; i < n; i++)
            S[i] = i;
 
        bool ans = checkCograph(S);
        cout << (ans ? "TAK" : "NIE") << "\n";
    }
    return 0;
}
