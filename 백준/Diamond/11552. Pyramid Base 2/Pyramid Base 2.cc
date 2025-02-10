#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// Lazy segment tree for range-add and range-minimum query over [1, n]
struct SegTree {
    int n;
    vector<ll> tree, lazy;
    
    SegTree(int n) : n(n) {
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }
    
    // Apply update on current node
    void apply(int idx, int l, int r, ll val) {
        tree[idx] += val;
        lazy[idx] += val;
    }
    
    // Push lazy updates to children
    void pushDown(int idx, int l, int r) {
        if(lazy[idx] != 0) {
            int mid = (l + r) / 2;
            apply(idx * 2, l, mid, lazy[idx]);
            apply(idx * 2 + 1, mid + 1, r, lazy[idx]);
            lazy[idx] = 0;
        }
    }
    
    // Update range [ql, qr] by adding val
    void update(int idx, int l, int r, int ql, int qr, ll val) {
        if(ql > r || qr < l) return;
        if(ql <= l && r <= qr) {
            apply(idx, l, r, val);
            return;
        }
        pushDown(idx, l, r);
        int mid = (l + r) / 2;
        update(idx * 2, l, mid, ql, qr, val);
        update(idx * 2 + 1, mid + 1, r, ql, qr, val);
        tree[idx] = min(tree[idx * 2], tree[idx * 2 + 1]);
    }
    
    // Query the minimum value over the entire range
    ll query() {
        return tree[1];
    }
};
 
// Structure to hold each obstacle’s data.
struct Obstacle {
    int x1, y1, x2, y2;
    int cost;
};
 
// Structure for events in the sweep–line (applied in the x–dimension)
struct Event {
    int x;
    int y1, y2;
    int delta; // cost to add (or subtract)
};
 
/*
  The idea for a candidate side length L:
  - Allowed placements for the square’s bottom–left are
      x in [1, M - L + 1] and y in [1, N - L + 1].
  - For each obstacle (given by rectangle [x1,y1] to [x2,y2] with removal cost)
    the square will intersect the obstacle if the placement (x,y) lies in:
         x in [max(1, x1 - L + 1), min(M - L + 1, x2)]
         y in [max(1, y1 - L + 1), min(N - L + 1, y2)]
  - For each such obstacle (if these intervals are nonempty) we add an “event”
    that adds cost in that rectangle in the (x,y) placement domain.
  - Then, sweeping x and updating a segment tree over y we check if there is any 
    placement with a total cost <= budget B.
*/
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int M, N;
    cin >> M >> N;
    int B;
    cin >> B;
    int P;
    cin >> P;
    vector<Obstacle> obstacles(P);
    for (int i = 0; i < P; i++){
        cin >> obstacles[i].x1 >> obstacles[i].y1 
            >> obstacles[i].x2 >> obstacles[i].y2 
            >> obstacles[i].cost;
    }
    
    // Binary search for maximum side length L (from 1 to min(M, N))
    int lo = 0, hi = min(M, N), ans = 0;
    while(lo <= hi){
        int mid = (lo + hi) / 2;
        if(mid == 0) { // skip the trivial case; pyramid side length must be >=1
            lo = mid + 1;
            continue;
        }
 
        // Allowed bottom–left positions for a square of side length mid
        int Xmax = M - mid + 1;
        int Ymax = N - mid + 1;
 
        // Build events for obstacles that affect some placements.
        vector<Event> events;
        events.reserve(P * 2);
        for (int i = 0; i < P; i++){
            // Determine the x–range in placement space for which the square touches this obstacle
            int lx = max(1, obstacles[i].x1 - mid + 1);
            int rx = min(Xmax, obstacles[i].x2);
            int ly = max(1, obstacles[i].y1 - mid + 1);
            int ry = min(Ymax, obstacles[i].y2);
            if(lx <= rx && ly <= ry) {
                // When x reaches lx, add this obstacle’s cost to y in [ly, ry]
                events.push_back({lx, ly, ry, obstacles[i].cost});
                // When x reaches rx+1, subtract this cost (ending the effect)
                if(rx + 1 <= Xmax)
                    events.push_back({rx + 1, ly, ry, -obstacles[i].cost});
            }
        }
 
        // Sort events by their x coordinate
        sort(events.begin(), events.end(), [](const Event &a, const Event &b){
            return a.x < b.x;
        });
 
        // Set up the segment tree for the y–dimension [1, Ymax], initially all zeros.
        SegTree seg(Ymax);
 
        bool canPlace = false;
        int curX = 1;
        int idx = 0;
        int E = events.size();
 
        // Sweep over x positions (only jumping at event boundaries)
        while(curX <= Xmax) {
            // Process all events at current x position.
            while(idx < E && events[idx].x == curX) {
                seg.update(1, 1, Ymax, events[idx].y1, events[idx].y2, events[idx].delta);
                idx++;
            }
            // Check if at current x the minimum removal cost among all y placements is within budget.
            if(seg.query() <= B) {
                canPlace = true;
                break;
            }
            // Jump to the next x position where events occur; between these, state is constant.
            int nextX = Xmax + 1;
            if(idx < E) nextX = events[idx].x;
            curX = nextX;
        }
 
        if(canPlace){
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    
    cout << ans << "\n";
    return 0;
}
