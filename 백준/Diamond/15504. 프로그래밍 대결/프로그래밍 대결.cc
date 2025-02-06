#include <bits/stdc++.h>
using namespace std;
 
// 최소비용 최대유량(Min Cost Flow) 알고리즘 (Dijkstra + potential 사용)
 
struct Edge {
    int to, rev;
    int cap;
    int cost;
};
 
struct MinCostFlow {
    int n;
    vector<vector<Edge>> graph;
    vector<int> dist, potential, parent_v, parent_e;
    
    MinCostFlow(int n): n(n), graph(n), dist(n), potential(n), parent_v(n), parent_e(n) {}
    
    void add_edge(int s, int t, int cap, int cost) {
        Edge a = {t, (int)graph[t].size(), cap, cost};
        Edge b = {s, (int)graph[s].size(), 0, -cost};
        graph[s].push_back(a);
        graph[t].push_back(b);
    }
    
    // s에서 t로 flowLimit만큼의 유량을 보내는 최소 비용 유량 계산.
    pair<int, long long> min_cost_flow(int s, int t, int flowLimit) {
        const int INF = 1e9;
        int flow = 0;
        long long cost = 0;
        fill(potential.begin(), potential.end(), 0);
        while(flow < flowLimit){
            // Dijkstra로 최단 경로 탐색
            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            pq.push({0, s});
            while(!pq.empty()){
                auto cur = pq.top();
                pq.pop();
                int d = cur.first, v = cur.second;
                if(d != dist[v]) continue;
                for (int i = 0; i < graph[v].size(); i++){
                    Edge &e = graph[v][i];
                    if(e.cap > 0 && dist[e.to] > d + e.cost + potential[v] - potential[e.to]){
                        dist[e.to] = d + e.cost + potential[v] - potential[e.to];
                        parent_v[e.to] = v;
                        parent_e[e.to] = i;
                        pq.push({dist[e.to], e.to});
                    }
                }
            }
            if(dist[t] == INF) break; // 더 이상 유량 보낼 수 없으면 종료
            for (int v = 0; v < n; v++){
                if(dist[v] < INF)
                    potential[v] += dist[v];
            }
            int addFlow = flowLimit - flow;
            int v = t;
            while(v != s){
                addFlow = min(addFlow, graph[parent_v[v]][parent_e[v]].cap);
                v = parent_v[v];
            }
            flow += addFlow;
            v = t;
            while(v != s){
                Edge &e = graph[parent_v[v]][parent_e[v]];
                e.cap -= addFlow;
                graph[v][e.rev].cap += addFlow;
                v = parent_v[v];
            }
            cost += (long long) addFlow * potential[t];
        }
        return {flow, cost};
    }
};
 
// --- Main ---
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N; 
    cin >> N;
    vector<int> A(N), H(N), L(N);
    for (int i=0; i<N; i++){
        cin >> A[i];
    }
    for (int i=0; i<N; i++){
        cin >> H[i];
    }
    for (int i=0; i<N; i++){
        cin >> L[i];
    }
 
    // 챔피언(실력 최대값인 참가자)를 찾고, 참가자들을 실력 내림차순으로 정렬합니다.
    // 정렬 후 인덱스 0에 챔피언이 오게 됩니다.
    vector<int> idx(N);
    for (int i=0; i<N; i++){
        idx[i] = i;
    }
    sort(idx.begin(), idx.end(), [&](int i, int j){
        return A[i] > A[j];
    });
 
    vector<int> A_sorted(N), H_sorted(N), L_sorted(N);
    for (int i=0; i<N; i++){
        A_sorted[i] = A[idx[i]];
        H_sorted[i] = H[idx[i]];
        L_sorted[i] = L[idx[i]];
    }
    // 챔피언은 인덱스 0 (A_sorted[0] 최대)
    // 매칭에서 부모 후보(왼쪽)는 인덱스 0..N-1, 자식(오른쪽)는 인덱스 1..N-1 (즉, 챔피언은 자식이 될 수 없음)
    // 에지 (부모 i -> 자식 j)는 i < j (정렬순서)일 때 자동으로 A_sorted[i] > A_sorted[j]입니다.
 
    // 각 참가자의 부모 역할 최대 횟수 (capacity)
    vector<int> cap(N, 0);
    cap[0] = L_sorted[0];           // 챔피언은 L_sorted[0]번 가능
    for (int i=1; i<N; i++){
        cap[i] = L_sorted[i] - 1;     // 비챔피언은 이미 1번 패배하므로 L_sorted[i]-1번 승리 가능
    }
 
    // 네트워크 플로우 그래프 구성
    // 노드 번호 할당:
    // source: 0
    // 왼쪽(부모 후보): 노드 1 ~ N (정렬상의 인덱스 i는 노드 i+1)
    // 오른쪽(자식): 노드 N+1 ~ (2*N - 1) (정렬상의 인덱스 j (j>=1)는 노드 N+j)
    // sink: 2*N
    int source = 0;
    int numLeft = N;      // 왼쪽 노드 개수 (챔피언 포함)
    int numRight = N - 1; // 오른쪽 노드: 인덱스 1~N-1 (비챔피언)
    int sink = 2 * N;
    int totalNodes = 2 * N + 1; // 0 ~ 2*N
 
    MinCostFlow mcf(totalNodes);
 
    // source -> 왼쪽 노드 (부모 후보) : capacity = cap, cost = 0
    for (int i = 0; i < numLeft; i++){
        int node = 1 + i; // 정렬상 i번 참가자에 대응
        mcf.add_edge(source, node, cap[i], 0);
    }
 
    // 왼쪽(부모 후보) -> 오른쪽(자식)
    // 오른쪽 노드 j (정렬상 참가자 인덱스 j, j>=1)는 노드 번호 = N + j.
    // 가능한 에지: 모든 i, j with i < j.
    // 에지의 가중치: (A_sorted[j] XOR A_sorted[i]) - H_sorted[i]
    // 최대 비용 매칭을 위해 mincostflow에서는 cost에 - (가중치)를 넣습니다.
    for (int j = 1; j < N; j++){
        int rightNode = N + j; // j번 참가자(비챔피언)에 대응
        for (int i = 0; i < j; i++){
            int leftNode = 1 + i;
            int edgeWeight = (A_sorted[j] ^ A_sorted[i]) - H_sorted[i];
            int cost = -edgeWeight; // min_cost flow에서는 -가중치를 넣음
            mcf.add_edge(leftNode, rightNode, 1, cost);
        }
    }
 
    // 오른쪽 노드 -> sink : capacity = 1, cost = 0
    for (int j = 1; j < N; j++){
        int rightNode = N + j;
        mcf.add_edge(rightNode, sink, 1, 0);
    }
 
    // 오른쪽(자식) 노드는 챔피언을 제외한 모든 참가자이므로 총 N-1개.
    int flowNeeded = numRight;
    auto result = mcf.min_cost_flow(source, sink, flowNeeded);
    int flowSent = result.first;
    long long minCost = result.second;
    if(flowSent < flowNeeded){
        // 문제 조건을 만족하는 대진표가 항상 존재함 (따라서 이 경우는 발생하지 않음)
        cout << "Impossible\n";
        return 0;
    }
 
    // minCost는 선택한 에지들의 cost의 합인데 cost = -((A_j XOR A_i) - H_i)
    // 따라서 매칭으로 얻은 총 "순이익"은 -minCost.
    long long maxMatchingWeight = -minCost;
    // 앞서 최종 대회의 재미 = (매칭으로 얻은 총 순이익) - (비챔피언들의 H 합)
    long long sumHNonChampion = 0;
    for (int i = 1; i < N; i++){
        sumHNonChampion += H_sorted[i];
    }
    long long answer = maxMatchingWeight - sumHNonChampion;
    cout << answer << "\n";
 
    return 0;
}
