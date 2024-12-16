#include <bits/stdc++.h>
using namespace std;

struct City {
    int id;
    int x, y;
};

double dist(const City &a, const City &b) {
    double dx = (double)a.x - b.x;
    double dy = (double)a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, K;
    cin >> N >> K;
    vector<City> cities(N);
    for (int i = 0; i < N; i++) {
        cin >> cities[i].x >> cities[i].y;
        cities[i].id = i+1;
    }
    
    // Sort cities by x-coordinate (as a simple heuristic)
    sort(cities.begin(), cities.end(), [](const City &a, const City &b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    
    // Divide into K clusters approximately evenly
    // For simplicity: just slice them into K segments
    vector<vector<City>> clusters(K);
    int base_size = N / K;
    int remainder = N % K;
    int start = 0;
    for (int i = 0; i < K; i++) {
        int cluster_size = base_size + (i < remainder ? 1 : 0);
        for (int j = 0; j < cluster_size; j++) {
            clusters[i].push_back(cities[start+j]);
        }
        start += cluster_size;
    }
    
    // For each cluster, construct a TSP route using a simple nearest neighbor heuristic
    // Start from the leftmost city in each cluster (already sorted by x)
    // We'll pick the first city as the start and then repeatedly pick the nearest unvisited city.
    for (int i = 0; i < K; i++) {
        auto &C = clusters[i];
        // nearest neighbor tour
        int csize = (int)C.size();
        if (csize == 1) {
            // trivial route
            cout << 1 << " " << C[0].id << "\n";
            continue;
        }
        
        vector<bool> visited(csize,false);
        vector<int> route;
        route.reserve(csize);
        
        // start from the first city in the sorted order
        int current = 0;
        visited[current] = true;
        route.push_back(current);
        for (int step = 1; step < csize; step++) {
            int next = -1;
            double best_dist = numeric_limits<double>::infinity();
            for (int cand = 0; cand < csize; cand++) {
                if (!visited[cand]) {
                    double d = dist(C[route.back()], C[cand]);
                    if (d < best_dist) {
                        best_dist = d;
                        next = cand;
                    }
                }
            }
            visited[next] = true;
            route.push_back(next);
        }
        
        // Output the route in the required format:
        // c_k p_{k,1} p_{k,2} ... p_{k,c_k}
        cout << csize;
        for (auto idx: route) {
            cout << " " << C[idx].id;
        }
        cout << "\n";
    }
    
    return 0;
}
