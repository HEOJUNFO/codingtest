#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <map>
using namespace std;
typedef long long ll;
const int MOD = 1000003;

struct Edge {
    int u, v;
    ll w;
};

struct DSU {
    vector<int> parent;
    DSU(int n) : parent(n+1) {
        for (int i = 0; i <= n; i++) parent[i] = i;
    }
    int find(int a) {
        return parent[a] == a ? a : parent[a] = find(parent[a]);
    }
    void unionn(int a, int b) {
        a = find(a), b = find(b);
        if(a != b) parent[b] = a;
    }
};

ll modMul(ll a, ll b) {
    return ((a % MOD) * (b % MOD)) % MOD;
}

ll modPow(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while(exp > 0){
       if(exp & 1) res = modMul(res, base);
       base = modMul(base, base);
       exp >>= 1;
    }
    return res;
}

ll modInv(ll a) {
    return modPow(a, MOD-2);
}

// Gaussian elimination to compute determinant mod MOD
ll determinant(vector<vector<int>> mat) {
    int n = mat.size();
    ll det = 1;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        while(pivot < n && mat[pivot][i] == 0) pivot++;
        if(pivot == n) return 0;
        if(i != pivot) {
            swap(mat[i], mat[pivot]);
            det = (MOD - det) % MOD; // swap flips sign
        }
        det = modMul(det, mat[i][i]);
        int inv = modInv(mat[i][i]);
        for (int j = i; j < n; j++) {
            mat[i][j] = (int)(modMul(mat[i][j], inv) % MOD);
        }
        for (int k = i+1; k < n; k++){
            int factor = mat[k][i];
            for (int j = i; j < n; j++){
                mat[k][j] = (mat[k][j] - (ll)factor * mat[i][j]) % MOD;
                if(mat[k][j] < 0) mat[k][j] += MOD;
            }
        }
    }
    return (det % MOD + MOD) % MOD;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++){
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){
        return a.w < b.w;
    });
    
    DSU dsu(n);
    ll ans = 1;
    // 같은 가중치 그룹별로 처리
    for (int i = 0; i < m; ) {
        int j = i;
        while(j < m && edges[j].w == edges[i].w) j++;
        // 이 그룹에서 DSU상 서로 다른 집합을 연결하는 간선들만 후보에 넣는다.
        vector<pair<int,int>> candidates;
        for (int k = i; k < j; k++){
            int ru = dsu.find(edges[k].u);
            int rv = dsu.find(edges[k].v);
            if(ru == rv) continue; // 루프 혹은 이미 연결되어 있는 간선 무시
            candidates.push_back({ru, rv});
        }
        // 후보 간선의 DSU 대표들을 모아서 "슈퍼 정점"들에 대해 작은 그래프를 구성한다.
        unordered_map<int,int> compIndex; // DSU 대표 -> 0부터 시작하는 인덱스
        vector<int> nodes;
        for (auto &p : candidates) {
            if(compIndex.find(p.first) == compIndex.end()){
                compIndex[p.first] = nodes.size();
                nodes.push_back(p.first);
            }
            if(compIndex.find(p.second) == compIndex.end()){
                compIndex[p.second] = nodes.size();
                nodes.push_back(p.second);
            }
        }
        int tot = nodes.size();
        // 인접 리스트 구성 (여기서는 후보 간선이 최대 4개이므로 tot도 작을 것임)
        vector<vector<pair<int,int>>> graph(tot);
        for (auto &p : candidates) {
            int u = compIndex[p.first], v = compIndex[p.second];
            graph[u].push_back({v,1});
            graph[v].push_back({u,1});
        }
        // 각 연결 컴포넌트별로 Kirchhoff 행렬-트리 정리를 적용
        vector<int> compLabel(tot, -1);
        int compId = 0;
        for (int u = 0; u < tot; u++){
            if(compLabel[u] != -1) continue;
            // DFS로 한 컴포넌트 구성
            vector<int> compNodes;
            stack<int> st;
            st.push(u);
            compLabel[u] = compId;
            while(!st.empty()){
                int cur = st.top(); st.pop();
                compNodes.push_back(cur);
                for(auto &edge : graph[cur]){
                    int nxt = edge.first;
                    if(compLabel[nxt] == -1){
                        compLabel[nxt] = compId;
                        st.push(nxt);
                    }
                }
            }
            int r = compNodes.size();
            // 스패닝 트리 개수 = Kirchhoff's matrix-tree (r==1이면 1)
            ll ways = 1;
            if(r > 1){
                // 해당 컴포넌트의 정점에 대해 로컬 인덱스 부여
                unordered_map<int,int> localIndex;
                for (int i = 0; i < r; i++){
                    localIndex[compNodes[i]] = i;
                }
                // Laplacian 행렬 구성 (크기 r x r)
                vector<vector<int>> lap(r, vector<int>(r, 0));
                // 후보 간선들을 모두 순회하여 두 정점 모두 이 컴포넌트에 속하면 multiplicity 반영
                for (auto &p : candidates) {
                    int gu = compIndex[p.first], gv = compIndex[p.second];
                    if(compLabel[gu] == compId && compLabel[gv] == compId){
                        int u_local = localIndex[gu], v_local = localIndex[gv];
                        // 여러 간선이 있을 수 있으므로 합산 (무방향이므로 한 번만 반영하면 두 곳 반영)
                        lap[u_local][u_local] = (lap[u_local][u_local] + 1) % MOD;
                        lap[v_local][v_local] = (lap[v_local][v_local] + 1) % MOD;
                        lap[u_local][v_local] = (lap[u_local][v_local] - 1) % MOD;
                        if(lap[u_local][v_local] < 0) lap[u_local][v_local] += MOD;
                        lap[v_local][u_local] = (lap[v_local][u_local] - 1) % MOD;
                        if(lap[v_local][u_local] < 0) lap[v_local][u_local] += MOD;
                    }
                }
                // Kirchhoff 정리에 따라, Laplacian 행렬에서 임의의 한 행,열을 제거한 (r-1)x(r-1) 행렬의 행렬식이
                // 해당 컴포넌트의 스패닝 트리 개수
                vector<vector<int>> mat(r-1, vector<int>(r-1, 0));
                for (int a = 1; a < r; a++){
                    for (int b = 1; b < r; b++){
                        mat[a-1][b-1] = lap[a][b] % MOD;
                        if(mat[a-1][b-1] < 0) mat[a-1][b-1] += MOD;
                    }
                }
                ways = determinant(mat);
            }
            ans = (ans * ways) % MOD;
            compId++;
        }
        // 그룹 처리 후, 실제 DSU에서 이 그룹의 모든 후보 간선을 연결
        for (auto &p : candidates){
            dsu.unionn(p.first, p.second);
        }
        i = j;
    }
    ans = (ans % MOD + MOD) % MOD;
    cout << ans << "\n";
    return 0;
}
