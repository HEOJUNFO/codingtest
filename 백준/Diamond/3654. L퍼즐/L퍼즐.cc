#include <bits/stdc++.h>
using namespace std;
 
// 검정 칸 하나당 두 개의 “부분” (부분 노드)를 만들기 위한 구조체
struct BlackPart {
    int r, c;
    int type; // 0: horizontal (가로) 선택, 1: vertical (세로) 선택
};
 
// 전역 변수 NIL : 왼쪽 노드에서의 dummy 노드의 인덱스로 사용 (나중에 Hopcroft-Karp에 사용)
int NIL;
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
    while(T--){
        int n, m;
        cin >> n >> m;
        vector<string> grid(n);
        for (int i = 0; i < n; i++){
            cin >> grid[i];
        }
 
        // 패턴 내의 'B'와 'W' 개수 세기
        int countB = 0, countW = 0;
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                if(grid[i][j] == 'B')
                    countB++;
                else if(grid[i][j] == 'W')
                    countW++;
            }
        }
 
        // 타일 하나는 검정 1개, 흰색 2개를 덮으므로 흰색 수가 2*검정 수여야 함.
        if(countW != 2 * countB){
            cout << "NO\n";
            continue;
        }
 
        // 각 검정 칸에 대해 “가로 선택”과 “세로 선택” 두 개의 노드를 생성
        vector<BlackPart> blackParts; // 왼쪽 파트 노드들
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                if(grid[i][j] == 'B'){
                    BlackPart bp;
                    bp.r = i; bp.c = j;
                    bp.type = 0; // 가로 선택
                    blackParts.push_back(bp);
                    bp.type = 1; // 세로 선택
                    blackParts.push_back(bp);
                }
            }
        }
 
        int leftSize = blackParts.size(); // 왼쪽 노드의 개수 = 2 * (검정 칸 수)
        int rightSize = n * m; // 오른쪽 노드는 격자상의 모든 칸 중 흰색인 칸 (id: r*m+c)
 
        // 왼쪽 노드(각 검정 칸의 부분 노드)에서 오른쪽 노드(흰 칸)로의 엣지 구성
        // – 가로 선택( type==0 )이면 좌측 또는 우측 인접 칸이 'W'여야 함.
        // – 세로 선택( type==1 )이면 상단 또는 하단 인접 칸이 'W'여야 함.
        vector<vector<int>> adj(leftSize);
        for (int i = 0; i < leftSize; i++){
            int r = blackParts[i].r;
            int c = blackParts[i].c;
            if(blackParts[i].type == 0){ 
                // 가로 선택: (r, c-1) 또는 (r, c+1)
                int nc = c - 1;
                if(nc >= 0 && grid[r][nc] == 'W'){
                    int rid = r * m + nc;
                    adj[i].push_back(rid);
                }
                nc = c + 1;
                if(nc < m && grid[r][nc] == 'W'){
                    int rid = r * m + nc;
                    adj[i].push_back(rid);
                }
            } else { 
                // 세로 선택: (r-1, c) 또는 (r+1, c)
                int nr = r - 1;
                if(nr >= 0 && grid[nr][c] == 'W'){
                    int rid = nr * m + c;
                    adj[i].push_back(rid);
                }
                nr = r + 1;
                if(nr < n && grid[nr][c] == 'W'){
                    int rid = nr * m + c;
                    adj[i].push_back(rid);
                }
            }
        }
 
        // Hopcroft–Karp 알고리즘 준비
        // matchL[u] : 왼쪽 노드 u가 매칭된 오른쪽 노드 (없으면 -1)
        // matchR[v] : 오른쪽 노드 v가 매칭된 왼쪽 노드 (없으면 -1)
        vector<int> matchL(leftSize, -1);
        vector<int> matchR(rightSize, -1);
 
        const int INF = 1e9;
        // dist[u] : 왼쪽 노드 u의 BFS 레벨 (dummy NIL 노드도 포함)
        vector<int> dist(leftSize + 1, INF);
 
        // NIL를 왼쪽 노드의 dummy 인덱스로 사용 (NIL = leftSize)
        NIL = leftSize;
 
        // BFS (left 쪽 미매칭 노드로부터 거리 계산)
        auto bfs = [&]() -> bool {
            deque<int> dq;
            for (int u = 0; u < leftSize; u++){
                if(matchL[u] == -1){
                    dist[u] = 0;
                    dq.push_back(u);
                } else {
                    dist[u] = INF;
                }
            }
            dist[NIL] = INF;
 
            while(!dq.empty()){
                int u = dq.front();
                dq.pop_front();
                if(u != NIL){
                    for (int v : adj[u]){
                        int u2 = matchR[v];
                        if(u2 == -1){
                            // v가 자유면 dummy NIL까지의 거리 갱신
                            dist[NIL] = dist[u] + 1;
                        } else if(dist[u2] == INF){
                            dist[u2] = dist[u] + 1;
                            dq.push_back(u2);
                        }
                    }
                }
            }
            return dist[NIL] != INF;
        };
 
        // DFS (증가 경로 찾기)
        function<bool(int)> dfs = [&](int u) -> bool {
            if(u != NIL){
                for (int v : adj[u]){
                    int u2 = matchR[v];
                    if(u2 == -1 || (dist[u2] == dist[u] + 1 && dfs(u2))){
                        matchL[u] = v;
                        matchR[v] = u;
                        return true;
                    }
                }
                dist[u] = INF;
                return false;
            }
            return true;
        };
 
        int matching = 0;
        while(bfs()){
            for (int u = 0; u < leftSize; u++){
                if(matchL[u] == -1 && dfs(u))
                    matching++;
            }
        }
 
        // 모든 왼쪽 노드(검정 칸의 두 부분)가 매칭되어야 올바른 타일 배치가 가능함.
        if(matching == leftSize)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}
