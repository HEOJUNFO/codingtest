#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <unordered_set>
using namespace std;

// 트리를 루트가 u, 부모가 p인 상태에서 canonical representation으로 인코딩
string encodeTree(int u, int p, const vector<vector<int>>& adj) {
    vector<string> children;
    for (int v : adj[u]) {
        if (v == p) continue;
        children.push_back(encodeTree(v, u, adj));
    }
    sort(children.begin(), children.end());
    string rep = "(";
    for (auto &s : children) {
        rep += s;
    }
    rep += ")";
    return rep;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    
    // 서로 다른 트리 위상의 canonical representation을 저장할 해시 집합
    unordered_set<string> uniqueTrees;
    uniqueTrees.reserve(T * 2);
    
    // 각 별자리(트리)를 처리
    for (int t = 0; t < T; t++){
        int s;
        cin >> s;
        vector<vector<int>> adj(s);
        for (int i = 0; i < s - 1; i++){
            int u, v;
            cin >> u >> v;
            // 무방향 그래프이므로 양쪽에 간선 추가
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        // s가 1이면 트리 하나의 canonical representation은 "()"로 고정됨
        if(s == 1){
            uniqueTrees.insert("()");
            continue;
        }
        
        // (s <= 30이므로) 각 노드를 제거했을 때 생기는 연결 컴포넌트의 크기를 구해서
        // 중심(centroid)을 찾는다.
        int bestBalance = s;
        vector<int> centroids;
        for (int u = 0; u < s; u++){
            int largest = 0;
            vector<bool> visited(s, false);
            visited[u] = true; // u를 제거한 것으로 처리
            for (int v : adj[u]){
                if (visited[v]) continue;
                int cnt = 0;
                function<void(int)> dfsComp = [&](int cur) {
                    visited[cur] = true;
                    cnt++;
                    for (int nxt : adj[cur]) {
                        if (!visited[nxt])
                            dfsComp(nxt);
                    }
                };
                dfsComp(v);
                largest = max(largest, cnt);
            }
            if(largest < bestBalance) {
                bestBalance = largest;
                centroids.clear();
                centroids.push_back(u);
            } else if(largest == bestBalance) {
                centroids.push_back(u);
            }
        }
 
        // 각 중심에 대해 canonical representation을 구한 뒤 사전순으로 최소인 것을 사용
        string bestRep;
        bool first = true;
        for (int c : centroids){
            string rep = encodeTree(c, -1, adj);
            if(first || rep < bestRep){
                bestRep = rep;
                first = false;
            }
        }
        uniqueTrees.insert(bestRep);
    }
    
    cout << uniqueTrees.size() << "\n";
    return 0;
}
