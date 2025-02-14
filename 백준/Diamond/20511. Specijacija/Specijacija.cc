#include <bits/stdc++.h>
using namespace std;
 
// --- Piecewise function representation ---
//
// Every function f is defined on [1,dom] (domain size = dom)
// and is represented as a set of contiguous segments.
// On each segment [l,r] f is either “linear” (f(x)= x+off)
// or constant (f(x)= val).
 
struct Segment {
    int l, r;      // domain interval [l,r]
    bool isConst;  // true if f is constant on [l,r]
    int val;       // if constant: f(x) = val
    int off;       // if linear: f(x) = x + off
};
 
struct Func {
    int dom;              // domain is [1, dom]
    vector<Segment> segs; // segments covering [1, dom]
};
 
// Merge adjacent segments if possible.
void mergeFunc(Func &f) {
    if(f.segs.empty()) return;
    vector<Segment> ns;
    ns.push_back(f.segs[0]);
    for (size_t i = 1; i < f.segs.size(); i++){
        Segment &prev = ns.back();
        Segment cur = f.segs[i];
        if(prev.isConst == cur.isConst && prev.r + 1 == cur.l) {
            if(prev.isConst && prev.val == cur.val){
                prev.r = cur.r;
                continue;
            }
            if(!prev.isConst && prev.off == cur.off){
                prev.r = cur.r;
                continue;
            }
        }
        ns.push_back(cur);
    }
    f.segs = ns;
}
 
// Evaluate function g at x.
int applyFunc(const Func &g, int x) {
    int lo = 0, hi = (int)g.segs.size() - 1;
    while(lo <= hi) {
        int mid = (lo + hi) / 2;
        if(g.segs[mid].l <= x && x <= g.segs[mid].r) {
            if(g.segs[mid].isConst) return g.segs[mid].val;
            else return x + g.segs[mid].off;
        } else if(x < g.segs[mid].l) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return -1; // should not happen
}
 
// Compose two functions: given f: [1,A] -> [1,B] and g: [1,B] -> Z,
// define h(x) = g(f(x)) for x in [1,A].
Func composeFunc(const Func &f, const Func &g) {
    Func h;
    h.dom = f.dom; // domain remains
    for(auto seg : f.segs) {
        if(seg.isConst) {
            int c = seg.val;
            int gval = applyFunc(g, c);
            Segment ns;
            ns.l = seg.l; ns.r = seg.r; ns.isConst = true; ns.val = gval;
            h.segs.push_back(ns);
        } else {
            // f is linear: f(x)= x+seg.off for x in [seg.l, seg.r].
            int L_y = seg.l + seg.off, R_y = seg.r + seg.off;
            // Partition [L_y, R_y] according to pieces of g.
            for(auto gs : g.segs) {
                int I_l = max(gs.l, L_y), I_r = min(gs.r, R_y);
                if(I_l > I_r) continue;
                int X_l = I_l - seg.off, X_r = I_r - seg.off;
                X_l = max(X_l, seg.l); X_r = min(X_r, seg.r);
                if(X_l > X_r) continue;
                Segment ns;
                ns.l = X_l; ns.r = X_r;
                if(gs.isConst) {
                    ns.isConst = true; ns.val = gs.val;
                } else {
                    ns.isConst = false; ns.off = seg.off + gs.off;
                }
                h.segs.push_back(ns);
            }
        }
    }
    sort(h.segs.begin(), h.segs.end(), [](const Segment &a, const Segment &b){
        return a.l < b.l;
    });
    mergeFunc(h);
    return h;
}
 
// Global dp table: dp[k][lev] for 0 <= k <= maxK and lev such that lev - (1<<k) >= 1.
// dp[k][lev] is a Func mapping [1, lev] -> [1, lev - (1<<k)].
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, q, t;
    cin >> n >> q >> t;
    int maxLevel = n + 1;
    vector<int> a(n+1);
    for (int i = 1; i <= n; i++){
        cin >> a[i];
    }
    // Compute spec[i] = a[i] - ( (i-1)*i/2 ) for i = 1..n.
    // (Recall: the parameter a[i] comes from level i; the parent of a vertex in level i+1 uses spec[i].)
    vector<int> spec(n+1, 0);
    for (int i = 1; i <= n; i++){
        spec[i] = a[i] - (i*(i-1)/2);
    }
 
    // Maximum k for binary lifting.
    int maxK = 0;
    while((1 << (maxK+1)) <= maxLevel) maxK++;
 
    // dp[0][lev] for lev=2..maxLevel.
    vector<vector<Func>> dp(maxK+1, vector<Func>(maxLevel+1));
    for (int lev = 2; lev <= maxLevel; lev++){
        dp[0][lev].dom = lev;
        dp[0][lev].segs.clear();
        int thresh = spec[lev-1]; // parent's function from level lev uses spec from level lev-1.
        if(thresh >= 1){
            Segment seg;
            seg.l = 1; seg.r = thresh;
            seg.isConst = false; seg.off = 0; // f(x)= x
            dp[0][lev].segs.push_back(seg);
        }
        if(thresh < lev){
            Segment seg;
            seg.l = thresh+1; seg.r = lev;
            seg.isConst = false; seg.off = -1; // f(x)= x-1
            dp[0][lev].segs.push_back(seg);
        }
        mergeFunc(dp[0][lev]);
    }
 
    // Build dp for k >= 1.
    for (int k = 1; k <= maxK; k++){
        for (int lev = (1 << k) + 1; lev <= maxLevel; lev++){
            int midLev = lev - (1 << (k-1));
            dp[k][lev] = composeFunc(dp[k-1][lev], dp[k-1][midLev]);
        }
    }
 
    // Helper: given a vertex (lev, pos), lift it upward by r levels.
    auto lift = [&](int lev, int pos, int r) -> pair<int,int> {
        for (int k = maxK; k >= 0; k--){
            int jump = (1 << k);
            if(jump <= r && lev - jump >= 1){
                pos = applyFunc(dp[k][lev], pos);
                lev -= jump;
                r -= jump;
            }
        }
        return {lev, pos};
    };
 
    // Parent: one level up.
    auto parentOf = [&](int lev, int pos) -> pair<int,int> {
        int newPos = applyFunc(dp[0][lev], pos);
        return {lev-1, newPos};
    };
 
    // Convert (lev,pos) to vertex label.
    auto labelOf = [&](int lev, int pos) -> long long {
        return (long long)lev * (lev - 1) / 2 + pos;
    };
 
    // Given vertex label x, recover (lev, pos).
    auto getLevelPos = [&](long long x) -> pair<int,int> {
        int lo = 1, hi = maxLevel, lev = 1;
        while(lo <= hi){
            int mid = (lo + hi) / 2;
            long long lowBound = (long long)mid * (mid - 1) / 2 + 1;
            long long highBound = (long long)mid * (mid + 1) / 2;
            if(x < lowBound) hi = mid - 1;
            else if(x > highBound) lo = mid + 1;
            else { lev = mid; break; }
        }
        int pos = (int)(x - (long long)lev * (lev - 1) / 2);
        return make_pair(lev, pos);
    };
 
    long long lastAns = 0;
    for (int i = 0; i < q; i++){
        long long tx, ty;
        cin >> tx >> ty;
        long long X = tx, Y = ty;
        // If t==1, queries are online.
        if(t == 1){
            long long totalV = (long long)maxLevel * (maxLevel + 1) / 2;
            X = ((tx - 1 + t * lastAns) % totalV) + 1;
            Y = ((ty - 1 + t * lastAns) % totalV) + 1;
        }
        auto pp1 = getLevelPos(X);
        auto pp2 = getLevelPos(Y);
        int L1 = pp1.first, pos1 = pp1.second;
        int L2 = pp2.first, pos2 = pp2.second;
 
        // Lift the deeper vertex so both are on the same level.
        if(L1 > L2){
            auto rpair = lift(L1, pos1, L1 - L2);
            L1 = rpair.first; pos1 = rpair.second;
        } else if(L2 > L1){
            auto rpair = lift(L2, pos2, L2 - L1);
            L2 = rpair.first; pos2 = rpair.second;
        }
 
        // Now both vertices are at the same level.
        // If already equal, that's the LCA.
        while(L1 > 1 && pos1 != pos2){
            int jumpFound = 0;
            for (int k = maxK; k >= 0; k--){
                int jump = (1 << k);
                if(L1 - jump < 1) continue;
                int newPos1 = applyFunc(dp[k][L1], pos1);
                int newPos2 = applyFunc(dp[k][L1], pos2);
                if(newPos1 != newPos2){
                    pos1 = newPos1; pos2 = newPos2;
                    L1 -= jump; L2 -= jump;
                    jumpFound = 1;
                    break;
                }
            }
            if(!jumpFound){
                auto par1 = parentOf(L1, pos1);
                auto par2 = parentOf(L2, pos2);
                L1 = par1.first; pos1 = par1.second;
                L2 = par2.first; pos2 = par2.second;
            }
        }
        long long ans = labelOf(L1, pos1);
        cout << ans << "\n";
        lastAns = ans;
    }
    return 0;
}
