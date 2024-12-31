#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
    vector<int> parent, rankVec;
    UnionFind(int n) : parent(n+1), rankVec(n+1, 0) {
        for(int i=0; i<=n; i++) parent[i] = i;
    }
    int find(int x) {
        if(parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if(a == b) return false;
        if(rankVec[a] < rankVec[b]) swap(a, b);
        parent[b] = a;
        if(rankVec[a] == rankVec[b]) rankVec[a]++;
        return true;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    while(true){
        int n, m, k;
        cin >> n >> m >> k;
        if(!cin || (n==0 && m==0 && k==0)) break;

        vector<pair<int, pair<int,int>>> edges; 
        // 간선 정보를 저장할 벡터
        // 색상을 정수로 맵핑: 빨간색(R)을 0, 파란색(B)을 1이라 하면
        // first: 색상(0 또는 1), second: (f, t)

        edges.reserve(m);

        for(int i=0; i<m; i++){
            char c;
            int f, t;
            cin >> c >> f >> t;
            int color = (c == 'B' ? 1 : 0);
            edges.push_back({color, {f, t}});
        }

        // ============ 1) 파란색 간선 개수 최소화(빨간색 우선) ============
        // color가 작은(0=R) 순으로 정렬 → 빨간색 우선
        sort(edges.begin(), edges.end(), [](auto &a, auto &b){
            return a.first < b.first; 
        });
        
        UnionFind ufMin(n);
        int usedBlueMin = 0, edgeCount = 0;
        for(auto &e : edges){
            int color = e.first;
            int f = e.second.first;
            int t = e.second.second;
            if(ufMin.unite(f, t)){
                edgeCount++;
                if(color == 1) usedBlueMin++;
                if(edgeCount == n-1) break;
            }
        }
        
        // ============ 2) 파란색 간선 개수 최대화(파란색 우선) ============
        // color가 큰(1=B) 순으로 정렬 → 파란색 우선
        sort(edges.begin(), edges.end(), [](auto &a, auto &b){
            return a.first > b.first; 
        });
        
        UnionFind ufMax(n);
        int usedBlueMax = 0;
        edgeCount = 0;
        for(auto &e : edges){
            int color = e.first;
            int f = e.second.first;
            int t = e.second.second;
            if(ufMax.unite(f, t)){
                edgeCount++;
                if(color == 1) usedBlueMax++;
                if(edgeCount == n-1) break;
            }
        }

        // ============ 결과 출력 ============
        // k가 [usedBlueMin, usedBlueMax] 범위 안에 있으면 1, 아니면 0
        if(usedBlueMin <= k && k <= usedBlueMax) cout << 1 << "\n";
        else cout << 0 << "\n";
    }

    return 0;
}
