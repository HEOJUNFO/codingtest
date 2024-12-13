#include <bits/stdc++.h>
using namespace std;

/**
 info[i]: i번 노드에 있는 동물 정보 (0: 양, 1: 늑대)
 edges: 트리의 간선 정보 (부모, 자식)
 
 풀이 개요:
 1. 트리 형태로 구성
 2. 백트래킹:
    - 현재까지 모은 양의 수, 늑대의 수를 관리
    - 후보 노드 집합(다음에 방문할 수 있는 노드들)을 관리하며,
      각 후보 노드를 방문할지 말지 선택
    - 방문 시 양/늑대 수 갱신, 자식 노드 후보에 추가
    - 늑대 ≥ 양 되는 경우 가지치기
    - 최대 양 수를 global로 갱신
*/

int n; 
vector<int> animal;           // info
vector<int> adj[50];          // 트리 구조, 자식 연결
int answer = 0;

void dfs(int sheep, int wolf, vector<int> candidates, vector<bool> &visited) {
    // 현재까지 모은 양의 최대 값 갱신
    answer = max(answer, sheep);

    for (int i = 0; i < (int)candidates.size(); i++) {
        int next = candidates[i];
        if (visited[next]) continue;

        int nsheep = sheep;
        int nwolf = wolf;
        if (animal[next] == 0) nsheep++;
        else nwolf++;

        if (nwolf >= nsheep) {
            // 늑대가 양 이상이면 이 경로는 더 진행 불가
            continue;
        }

        // next 노드 방문 처리
        vector<bool> nvisited = visited;
        nvisited[next] = true;

        // 다음 후보 목록 갱신
        vector<int> ncandidates = candidates;
        ncandidates.erase(ncandidates.begin() + i); // next 제거
        // next 자식들 추가
        for (auto &nx : adj[next]) {
            if (!nvisited[nx]) {
                ncandidates.push_back(nx);
            }
        }

        dfs(nsheep, nwolf, ncandidates, nvisited);
    }
}

int solution(vector<int> info, vector<vector<int>> edges) {
    n = (int)info.size();
    animal = info;

    // 트리 구성
    // 주어지는 형태는 부모->자식이 명확하게 있다고 했으니 그대로 사용
    for (auto &e : edges) {
        int p = e[0], c = e[1];
        adj[p].push_back(c);
    }

    vector<bool> visited(n, false);
    visited[0] = true; // 루트 방문
    // 루트 노드가 양이므로 sheep=1, wolf=0
    vector<int> candidates;
    // 루트의 자식들을 후보에 추가
    for (auto &c : adj[0]) candidates.push_back(c);

    dfs(1, 0, candidates, visited);

    return answer;
}

// main 함수 예제
// 아래 main은 제출 시 필요 없으며, 로컬 테스트용
int main() {
    vector<int> info1 = {0,0,1,1,1,0,1,0,1,0,1,1};
    vector<vector<int>> edges1 = {{0,1},{1,2},{1,4},{0,8},{8,7},{9,10},{9,11},{4,3},{6,5},{4,6},{8,9}};
    cout << solution(info1, edges1) << endl; // 결과: 5

    vector<int> info2 = {0,1,0,1,1,0,1,0,0,1,0};
    vector<vector<int>> edges2 = {{0,1},{0,2},{1,3},{1,4},{2,5},{2,6},{3,7},{4,8},{6,9},{9,10}};
    cout << solution(info2, edges2) << endl; // 결과: 5

    return 0;
}
