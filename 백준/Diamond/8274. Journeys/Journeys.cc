#include <bits/stdc++.h>
using namespace std;
 
struct Batch {
    int a, b, c, d;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, p;
    cin >> n >> m >> p;
    vector<Batch> batches(m);
    for (int i = 0; i < m; i++){
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        batches[i] = {a, b, c, d};
    }
    
    // Build an iterative segment tree.
    int base = 1;
    while(base < n) base *= 2;
    // segLeft: for each node, stores batch indices whose left interval [a,b] covers that segment.
    // segRight: similarly for right interval [c,d].
    vector<vector<int>> segLeft(2 * base), segRight(2 * base);
    
    // Insert each batch into segLeft for its left interval [a, b].
    for (int i = 0; i < m; i++){
        int L = batches[i].a, R = batches[i].b;
        // convert to 0-indexed leaf indices
        L = L - 1 + base;
        R = R - 1 + base;
        while(L <= R){
            if(L % 2 == 1){
                segLeft[L].push_back(i);
                L++;
            }
            if(R % 2 == 0){
                segLeft[R].push_back(i);
                R--;
            }
            L /= 2; R /= 2;
        }
    }
    
    // Insert each batch into segRight for its right interval [c, d].
    for (int i = 0; i < m; i++){
        int L = batches[i].c, R = batches[i].d;
        L = L - 1 + base;
        R = R - 1 + base;
        while(L <= R){
            if(L % 2 == 1){
                segRight[L].push_back(i);
                L++;
            }
            if(R % 2 == 0){
                segRight[R].push_back(i);
                R--;
            }
            L /= 2; R /= 2;
        }
    }
    
    // Prepare for BFS.
    // dist[i] will store the minimum cost from town p to town i.
    vector<int> dist(n+1, -1);
    deque<int> dq;
    dist[p] = 0;
    dq.push_back(p);
    
    // Use an ordered set to quickly find unvisited towns in a given interval.
    set<int> unvisited;
    for (int i = 1; i <= n; i++){
        if(i != p)
            unvisited.insert(i);
    }
    
    // For each batch, we want to process each side (left or right) only once.
    vector<bool> usedLeft(m, false), usedRight(m, false);
    
    // Standard BFS.
    while(!dq.empty()){
        int cur = dq.front();
        dq.pop_front();
        int dcur = dist[cur];
 
        // --- Process batches where town cur lies in the left interval ---
        int pos = cur - 1 + base;  // corresponding leaf in the seg tree.
        while(pos > 0){
            // For every batch stored at this node:
            for (int batchIndex : segLeft[pos]){
                if(!usedLeft[batchIndex]){
                    usedLeft[batchIndex] = true;
                    // Since cur is in [a,b] for this batch, relax all nodes in the opposite interval [c,d].
                    int L = batches[batchIndex].c, R = batches[batchIndex].d;
                    auto it = unvisited.lower_bound(L);
                    while(it != unvisited.end() && *it <= R){
                        int town = *it;
                        dist[town] = dcur + 1;
                        dq.push_back(town);
                        it = unvisited.erase(it);
                    }
                }
            }
            // Clear this node so that the batches are not processed again.
            segLeft[pos].clear();
            pos /= 2;
        }
 
        // --- Process batches where town cur lies in the right interval ---
        pos = cur - 1 + base;
        while(pos > 0){
            for (int batchIndex : segRight[pos]){
                if(!usedRight[batchIndex]){
                    usedRight[batchIndex] = true;
                    // Since cur is in [c,d], relax all nodes in the opposite left interval [a,b].
                    int L = batches[batchIndex].a, R = batches[batchIndex].b;
                    auto it = unvisited.lower_bound(L);
                    while(it != unvisited.end() && *it <= R){
                        int town = *it;
                        dist[town] = dcur + 1;
                        dq.push_back(town);
                        it = unvisited.erase(it);
                    }
                }
            }
            segRight[pos].clear();
            pos /= 2;
        }
    }
    
    // Output the minimum cost for each town.
    for (int i = 1; i <= n; i++){
        cout << dist[i] << "\n";
    }
    
    return 0;
}
