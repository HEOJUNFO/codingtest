#include <bits/stdc++.h>
using namespace std;
 
// 2D Fenwick Tree (BIT) that supports point updates and prefix sum queries
struct Fenw2D {
    int n;
    vector<vector<int>> ys;            // for each BIT node in x-dimension: list of y coordinates (compressed) that may be updated
    vector<vector<long long>> fenw;      // the internal BIT arrays (1-indexed in each node)
    
    Fenw2D() {}
    Fenw2D(int n) : n(n) {
        ys.assign(n+1, vector<int>());
        fenw.assign(n+1, vector<long long>());
    }
    
    void init(int n_) {
        n = n_;
        ys.assign(n+1, vector<int>());
        fenw.assign(n+1, vector<long long>());
    }
    
    // record a candidate update at BIT index x (for use during build)
    void add_candidate(int x, int y) {
        for(; x <= n; x += x & -x)
            ys[x].push_back(y);
    }
    
    // Build the internal data structure (sort & unique the candidate y-values, then initialize BIT arrays)
    void build(){
        for (int i = 1; i <= n; i++){
            sort(ys[i].begin(), ys[i].end());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            fenw[i].assign(ys[i].size()+1, 0);
        }
    }
    
    // update point (x,y) with delta (point update)
    void update(int x, int y, long long delta){
        for(; x <= n; x += x & -x){
            int pos = int(lower_bound(ys[x].begin(), ys[x].end(), y) - ys[x].begin()) + 1;
            for(; pos < (int)fenw[x].size(); pos += pos & -pos)
                fenw[x][pos] += delta;
        }
    }
    
    // prefix query: returns sum in region (1,1) to (x,y)
    long long query(int x, int y){
        long long res = 0;
        for(; x > 0; x -= x & -x){
            int pos = int(upper_bound(ys[x].begin(), ys[x].end(), y) - ys[x].begin());
            for(; pos > 0; pos -= pos & -pos)
                res += fenw[x][pos];
        }
        return res;
    }
};
 
// Helper for BIT that supports range queries on points (using inclusion‐exclusion).
long long rangeQuery(Fenw2D &F, int x1, int y1, int x2, int y2){
    if(x1 > x2 || y1 > y2) return 0;
    return F.query(x2, y2) - F.query(x1-1, y2) - F.query(x2, y1-1) + F.query(x1-1, y1-1);
}
 
// Structure to hold a query.
struct Query {
    int type; // 1 = add point, 2 = add rectangle
    // for type 1:
    int x, y;
    // for type 2:
    int x1, y1, x2, y2;
};
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int q;
    cin >> q;
    vector<Query> queries(q);
    // We collect all x and y coordinates for coordinate compression.
    vector<int> xs, ys;
    
    for (int i = 0; i < q; i++){
        int t;
        cin >> t;
        queries[i].type = t;
        if(t == 1){
            int x, y;
            cin >> x >> y;
            queries[i].x = x; queries[i].y = y;
            xs.push_back(x);
            ys.push_back(y);
        } else {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            queries[i].x1 = x1; queries[i].y1 = y1;
            queries[i].x2 = x2; queries[i].y2 = y2;
            xs.push_back(x1);
            xs.push_back(x2);
            // For difference updates we also need x2+1.
            xs.push_back(x2+1);
            ys.push_back(y1);
            ys.push_back(y2);
            ys.push_back(y2+1);
        }
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    
    // coordinate compression functions (1-indexed)
    auto compX = [&](int x) -> int {
        return int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
    };
    auto compY = [&](int y) -> int {
        return int(lower_bound(ys.begin(), ys.end(), y) - ys.begin()) + 1;
    };
    
    int n_x = xs.size();
    int n_y = ys.size();
    
    // We need two 2D BITs:
    // fenwPoint: will hold the points (from type 1 queries) so we can answer "how many points lie in a rectangle?"
    // fenwRect: will be updated for rectangle queries (type 2) in a difference–array style so that later a point query finds how many rectangles cover that point.
    Fenw2D fenwPoint(n_x), fenwRect(n_x);
    
    // Pre-candidate collection for fenwPoint (for point update positions)
    for (int i = 0; i < q; i++){
        if(queries[i].type == 1){
            int cx = compX(queries[i].x);
            int cy = compY(queries[i].y);
            for (int j = cx; j <= n_x; j += j & -j)
                fenwPoint.ys[j].push_back(cy);
        }
    }
    fenwPoint.build();
    
    // Pre-candidate collection for fenwRect (for rectangle difference updates)
    // For each rectangle (type 2), we plan to update:
    //   (cx1, cy1) += +1
    //   (cx2+1, cy1) += -1
    //   (cx1, cy2+1) += -1
    //   (cx2+1, cy2+1) += +1
    for (int i = 0; i < q; i++){
        if(queries[i].type == 2){
            int cx1 = compX(queries[i].x1);
            int cx2p = compX(queries[i].x2 + 1);
            int cy1 = compY(queries[i].y1);
            int cy2p = compY(queries[i].y2 + 1);
            for (int j = cx1; j <= n_x; j += j & -j)
                fenwRect.ys[j].push_back(cy1);
            for (int j = cx2p; j <= n_x; j += j & -j)
                fenwRect.ys[j].push_back(cy1);
            for (int j = cx1; j <= n_x; j += j & -j)
                fenwRect.ys[j].push_back(cy2p);
            for (int j = cx2p; j <= n_x; j += j & -j)
                fenwRect.ys[j].push_back(cy2p);
        }
    }
    fenwRect.build();
    
    // Process queries sequentially.
    // We maintain a cumulative answer "ans" which counts all pairs (point, rectangle)
    // such that the point lies inside (or on the border of) the rectangle.
    long long ans = 0;
    for (int i = 0; i < q; i++){
        if(queries[i].type == 1){
            // Type 1: add a point at (x, y)
            int cx = compX(queries[i].x);
            int cy = compY(queries[i].y);
            // When adding a point, count how many rectangles (added earlier) cover it.
            long long addRect = fenwRect.query(cx, cy);
            ans += addRect;
            // Add this point to fenwPoint.
            fenwPoint.update(cx, cy, 1);
            cout << ans << "\n";
        } else {
            // Type 2: add a rectangle with lower left (x1, y1) and upper right (x2, y2)
            int cx1 = compX(queries[i].x1);
            int cy1 = compY(queries[i].y1);
            int cx2 = compX(queries[i].x2);
            int cy2 = compY(queries[i].y2);
            int cx2p = compX(queries[i].x2 + 1);
            int cy2p = compY(queries[i].y2 + 1);
            // Count how many points (added earlier) lie inside the rectangle.
            long long cntPoints = rangeQuery(fenwPoint, cx1, cy1, cx2, cy2);
            ans += cntPoints;
            // Now update fenwRect with difference updates so that future point insertions count this rectangle.
            fenwRect.update(cx1, cy1, 1);
            fenwRect.update(cx2p, cy1, -1);
            fenwRect.update(cx1, cy2p, -1);
            fenwRect.update(cx2p, cy2p, 1);
            cout << ans << "\n";
        }
    }
    return 0;
}
