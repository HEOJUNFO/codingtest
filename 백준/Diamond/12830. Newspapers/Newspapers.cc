#include <bits/stdc++.h>
using namespace std;
 
// 전역 변수들
int n, k;
vector<vector<pair<int,int>>> adj; // (인접 노드, 원래 도로의 c값)
 
// centroid decomposition에 필요한 전역 변수들
vector<bool> dead;
vector<int> subSize;
 
// 이분 탐색 시 현재 후보값 (평균)
double currentA;
 
// 최종 경로가 존재하면 found==true로 설정
bool found = false;
 
// [1] 서브트리 크기 계산 (DFS)
int dfs_size(int u, int parent) {
    subSize[u] = 1;
    for(auto &edge : adj[u]){
        int v = edge.first;
        if(v == parent || dead[v]) continue;
        subSize[u] += dfs_size(v, u);
    }
    return subSize[u];
}
 
// [2] centroid 찾기 (u를 루트로 한 서브트리 내에서)
int find_centroid(int u, int parent, int total) {
    for(auto &edge : adj[u]){
        int v = edge.first;
        if(v == parent || dead[v]) continue;
        if(subSize[v] > total/2)
            return find_centroid(v, u, total);
    }
    return u;
}
 
// [3] 중심(centroid)에서 시작하여 한 서브트리 내의 경로 정보를 수집하는 DFS
// 각 경로에 대해 (depth, sum) 쌍을 vec에 추가합니다.
// 여기서 sum은 경로 상의 각 도로의 (c - currentA)를 누적한 값입니다.
void dfs_collect(int u, int parent, int depth, double sum, vector<pair<int,double>> &vec) {
    vec.push_back({depth, sum});
    for(auto &edge : adj[u]){
        int v = edge.first, c = edge.second;
        if(v == parent || dead[v]) continue;
        dfs_collect(v, u, depth + 1, sum + (c - currentA), vec);
    }
}
 
// [4] centroid에서 시작한 경로들끼리 결합해 k개 이상 도로를 사용하는 경로가 (변환가중치 합) >=0 인지 검사
// centroid를 기준으로 인접한 각 서브트리별로 DFS로 (depth,sum)쌍들을 모으고, 
// 기존에 모은 “best” 배열과 결합하여 두 경로를 연결했을 때 길이 합이 k 이상이고 총합이 0 이상이면 valid하다고 판단합니다.
bool process_centroid(int centroid) {
    // best[d] : centroid에서 출발해 지금까지 결합한 경로들 중 깊이가 d인 (출발 경로)에서 얻은 최대 누적합.
    vector<double> best(1, 0.0); // best[0]=0 (centroid 자체)
 
    // 인접한 각 서브트리에 대해 처리
    for(auto &edge : adj[centroid]){
        int v = edge.first;
        if(dead[v]) continue;
        vector<pair<int,double>> vec;
        // centroid와 v를 잇는 도로부터 시작 : 깊이 1, 누적합 = (c - currentA)
        dfs_collect(v, centroid, 1, edge.second - currentA, vec);
 
        // (A) 해당 서브트리 내부에서 경로 하나만으로 조건 (길이>=k, sum>=0)을 만족하는지 확인
        for(auto &p : vec) {
            if(p.first >= k && p.second >= 0) return true;
        }
 
        // (B) 기존 best와 결합해 조건을 만족하는 경로가 생기는지 검사
        // best는 각 깊이에 대해 최대 누적합을 저장하고 있으므로, 
        // 현재 vec의 (d, s)와 결합하여 d + d' >= k이고 s + best[d'] >= 0 인지 확인합니다.
        int bs = best.size();
        vector<double> suf(bs);
        suf[bs - 1] = best[bs - 1];
        for(int i = bs - 2; i >= 0; i--) {
            suf[i] = max(best[i], suf[i+1]);
        }
        for(auto &p : vec) {
            int need = max(0, k - p.first);
            if(need < bs) {
                if(suf[need] + p.second >= 0) return true;
            }
        }
 
        // (C) vec의 결과를 best에 병합(갱신)
        int maxDepth = 0;
        for(auto &p : vec) {
            maxDepth = max(maxDepth, p.first);
        }
        if((int)best.size() <= maxDepth)
            best.resize(maxDepth + 1, -1e18);
        for(auto &p : vec) {
            best[p.first] = max(best[p.first], p.second);
        }
    }
    return false;
}
 
// [5] Centroid Decomposition – 분할 정복 방식으로 트리를 쪼개면서 검사
void decompose(int u) {
    int total = dfs_size(u, -1);
    int centroid = find_centroid(u, -1, total);
    // centroid를 기준으로 경로들 검사
    if(process_centroid(centroid)) {
        found = true;
        return;
    }
    dead[centroid] = true;
    for(auto &edge : adj[centroid]) {
        int v = edge.first;
        if(!dead[v])
            decompose(v);
        if(found) return;
    }
}
 
// [6] check(mid): candidate 평균 currentA = mid일 때, 조건을 만족하는 경로(길이>=k, 평균>=mid)가 존재하면 true 반환
bool check(double mid) {
    currentA = mid;
    found = false;
    dead.assign(n, false);
    subSize.assign(n, 0);
    decompose(0);
    return found;
}
 
// 메인 – 이분 탐색으로 최적 평균을 구함
// 답과의 절대/상대오차 1e-6 이하이면 정답 처리됩니다.
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n >> k;
    adj.resize(n);
    dead.assign(n, false);
    subSize.assign(n, 0);
    int a, b, c;
    int maxC = 0;
    for (int i = 0; i < n - 1; i++){
        cin >> a >> b >> c;
        // 입력은 1-indexed라고 가정하므로 0-index로 변환
        a--; b--;
        adj[a].push_back({b, c});
        adj[b].push_back({a, c});
        maxC = max(maxC, c);
    }
 
    // 이분 탐색: 답은 [0, maxC] 구간 내에 있음 (모든 c는 0 이상)
    double lo = 0.0, hi = maxC;
    for (int iter = 0; iter < 50; iter++){
        double mid = (lo + hi) / 2.0;
        if(check(mid))
            lo = mid;
        else
            hi = mid;
    }
 
    cout << fixed << setprecision(8) << lo << "\n";
    return 0;
}
