#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int N, L;
vector<vector<int>> graph;

// 컴포넌트 내에서, 각 정점 u의 이웃 중 removed가 false인(즉, 존재하는) 이웃의 수를 구한다.
int localDegree(int u, const vector<bool>& removed) {
    int cnt = 0;
    for (int w : graph[u]) {
        if (!removed[w]) cnt++;
    }
    return cnt;
}

// 주어진 정점 집합(comp)이 체인(즉, 간단한 경로, 트리이면서 모든 정점의 localDegree<=2)인지 검사한다.
bool isChainComponent(const vector<int>& comp, const vector<bool>& removed) {
    int sz = comp.size();
    if(sz == 0) return true;   // 빈 컴포넌트는 고려하지 않음
    if(sz == 1) return true;   // 단일 정점는 체인으로 취급
    int edgeCount = 0;
    for (int u : comp) {
        int deg = localDegree(u, removed);
        if(deg > 2) return false; // 체인의 경우 각 정점의 degree는 최대 2
        edgeCount += deg;
    }
    // 간선은 양쪽에서 세었으므로 전체 간선수는 edgeCount/2여야 하고, 트리라면 (sz - 1)여야 한다.
    if(edgeCount/2 != sz - 1) return false;
    return true;
}

// 특정 정점 rem을 제거한 후, 전체 그래프의 모든 컴포넌트가 체인인지 판별
bool checkAllComponentsAfterRemoval(int rem) {
    vector<bool> removed(N, false);
    removed[rem] = true;
    vector<bool> visited(N, false);
    
    // 모든 정점(제거된 rem 제외)에 대해 컴포넌트별로 DFS를 수행한다.
    for (int i = 0; i < N; i++) {
        if (!removed[i] && !visited[i]) {
            vector<int> comp;
            // DFS (또는 BFS)로 컴포넌트를 구한다.
            stack<int> st;
            st.push(i);
            visited[i] = true;
            while(!st.empty()){
                int cur = st.top();
                st.pop();
                comp.push_back(cur);
                for (int nxt : graph[cur]) {
                    if (!removed[nxt] && !visited[nxt]) {
                        visited[nxt] = true;
                        st.push(nxt);
                    }
                }
            }
            // 구한 컴포넌트가 체인인지 확인
            if(!isChainComponent(comp, removed))
                return false;
        }
    }
    return true;
}

// CountCritical 호출 : 모든 정점 v에 대해, v를 제거했을 때 남은 컴포넌트가 모두 체인이라면 v는 "중요한 고리"
int CountCritical() {
    int cnt = 0;
    for (int v = 0; v < N; v++) {
        if(checkAllComponentsAfterRemoval(v))
            cnt++;
    }
    return cnt;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> L;
    graph.assign(N, vector<int>());
    
    // L개의 호출을 처리
    for (int i = 0; i < L; i++){
        int a, b;
        cin >> a;
        if(a == -1){
            // CountCritical 호출이면 결과를 출력
            cout << CountCritical() << "\n";
        } else {
            // Link(a, b) 호출 (a, b는 서로 다르고 아직 연결되지 않음을 보장)
            cin >> b;
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
    }
    return 0;
}
