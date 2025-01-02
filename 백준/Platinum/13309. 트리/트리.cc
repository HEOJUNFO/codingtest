#include <bits/stdc++.h>
using namespace std;

/*
 * 연결 성분 관리는 comp[v] 배열에 저장.
 * removedEdge[x] = true  => "x의 부모와 x를 잇는 에지"가 제거됨을 의미.
 * 트리의 루트(1번)는 부모가 없으므로 removedEdge[1]은 사용하지 않아도 됨(어차피 제거할 에지 없음).
 */

// 전역 혹은 적절한 범위 내 static 등으로 잡아도 무방
static const int MAXN = 200000;
vector<int> adj[MAXN+1];     // 인접 리스트
int parentNode[MAXN+1];      // parentNode[i] = i의 부모
bool removedEdge[MAXN+1];    // i의 부모 에지가 제거되었는지 여부
int compID[MAXN+1];          // compID[i] = i번 노드의 연결 성분 번호

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    // 부모 정보 입력 (2~N번 노드)
    // 루트는 1번 노드라서 부모가 없음.
    parentNode[1] = 0; // 편의상 0으로 처리
    for(int i = 2; i <= N; i++){
        int p; cin >> p;
        parentNode[i] = p;
        // 양방향 연결
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    // 초기 연결 성분: 전부 1번으로 통일
    for(int i = 1; i <= N; i++){
        compID[i] = 1;
        removedEdge[i] = false; // 아직 아무 에지도 제거되지 않음
    }

    // 이후 새로 배정할 연결 성분 ID
    int nextComp = 2;

    auto bfs = [&](int start, int newCompID) {
        queue<int>q;
        q.push(start);
        compID[start] = newCompID;

        while(!q.empty()){
            int cur = q.front(); 
            q.pop();

            for(int nx : adj[cur]){
                // 이미 같은 컴포넌트로 세팅되어 있으면 스킵
                if(compID[nx] == newCompID) continue;

                // cur - nx 간 에지가 살아있는지 판별
                // 방법1) parentNode[nx] == cur 이고 removedEdge[nx] == false
                // 혹은  parentNode[cur] == nx 이고 removedEdge[cur] == false
                bool edgeAlive = false;
                
                // nx가 cur의 자식인 경우
                if(parentNode[nx] == cur && !removedEdge[nx]) {
                    edgeAlive = true;
                }
                // cur가 nx의 자식인 경우
                if(parentNode[cur] == nx && !removedEdge[cur]) {
                    edgeAlive = true;
                }

                if(edgeAlive){
                    compID[nx] = newCompID;
                    q.push(nx);
                }
            }
        }
    };

    while(Q--){
        int b, c, d;
        cin >> b >> c >> d;

        // 연결 여부 판별
        bool connected = (compID[b] == compID[c]);

        // 출력
        cout << (connected ? "YES\n" : "NO\n");

        if(d == 1){
            if(connected){
                // b와 parent[b] 간 에지를 제거
                // 단, b == 1(루트)이면 제거할 에지가 없다.
                if(b != 1 && !removedEdge[b]){
                    removedEdge[b] = true;
                    // b 기준으로 서브트리를 새 컴포넌트 번호로 BFS 갱신
                    bfs(b, nextComp);
                    nextComp++;
                }
            }
            else {
                // c와 parent[c] 간 에지를 제거
                if(c != 1 && !removedEdge[c]){
                    removedEdge[c] = true;
                    // c 기준으로 서브트리를 새 컴포넌트 번호로 BFS 갱신
                    bfs(c, nextComp);
                    nextComp++;
                }
            }
        }
    }

    return 0;
}
