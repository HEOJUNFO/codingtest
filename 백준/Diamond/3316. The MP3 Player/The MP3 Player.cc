#include <bits/stdc++.h>
using namespace std;
 
// Global Vmax will be used in our functions.
int Vmax;
 
// Each function F: [0,Vmax] -> [0,Vmax] is represented as a list of segments.
// Each segment covers an integer interval [L, R] (inclusive) and is either linear (slope==1)
// meaning f(x)= x + b or constant (slope==0) meaning f(x)= b.
struct Seg {
    int L, R; 
    int slope; // either 0 or 1
    int b;     // if slope==1, then f(x)= x+b; if slope==0, then f(x)= b.
};
 
// Merge adjacent segments if they have identical representation.
vector<Seg> mergeSegments(const vector<Seg>& segs) {
    if(segs.empty()) return segs;
    vector<Seg> ret;
    ret.push_back(segs[0]);
    for (size_t i = 1; i < segs.size(); i++){
        const Seg &prev = ret.back();
        const Seg &cur = segs[i];
        if(cur.L == prev.R+1 && cur.slope==prev.slope && cur.b==prev.b)
            ret.back().R = cur.R;
        else
            ret.push_back(cur);
    }
    return ret;
}
 
// Compose two functions F and G; return H = G o F, i.e. H(x) = G(F(x)).
// Both F and G are given as lists of segments.
vector<Seg> composeFuncs(const vector<Seg>& F, const vector<Seg>& G) {
    vector<Seg> H;
    // For each segment s in F, compute its contribution.
    for(auto s : F) {
        if(s.slope==0) { 
            // On s: F(x) = s.b (a constant).
            int y = s.b;
            for(auto t : G) {
                if(t.L <= y && y <= t.R) {
                    int outVal = (t.slope==0 ? t.b : y + t.b);
                    H.push_back({s.L, s.R, 0, outVal});
                    break;
                }
            }
        } else {
            // s.slope==1, so F(x)= x + s.b for x in [s.L, s.R].
            int yStart = s.L + s.b;
            int yEnd = s.R + s.b;
            for(auto t : G) {
                int interLow = max(yStart, t.L);
                int interHigh = min(yEnd, t.R);
                if(interLow <= interHigh) {
                    int xLow = interLow - s.b;
                    int xHigh = interHigh - s.b;
                    int segL = max(s.L, xLow);
                    int segR = min(s.R, xHigh);
                    if(segL <= segR) {
                        if(t.slope==0)
                            H.push_back({segL, segR, 0, t.b});
                        else
                            H.push_back({segL, segR, 1, s.b + t.b});
                    }
                }
            }
        }
    }
    sort(H.begin(), H.end(), [](const Seg &a, const Seg &b){ return a.L < b.L; });
    return mergeSegments(H);
}
 
// Evaluate F at x.
int evalF(const vector<Seg>& F, int x) {
    for(auto s : F) {
        if(s.L <= x && x <= s.R) {
            if(s.slope==0) return s.b;
            else return x + s.b;
        }
    }
    return -1; // should not happen
}
 
// Given function F (in piecewise form), return the maximum x in [0,Vmax] such that F(x)==target.
// Return -1 if no such x exists.
int solveFor(const vector<Seg>& F, int target) {
    int best = -1;
    for(auto s : F) {
        if(s.slope==0) {
            if(s.b == target) best = max(best, s.R);
        } else {
            int x = target - s.b;
            if(s.L <= x && x <= s.R)
                best = max(best, x);
        }
    }
    return best;
}
 
// Predefined functions for keys.
vector<Seg> identityFunc() {
    return { {0, Vmax, 1, 0} };
}
 
// For a plus key: f(x)= min(Vmax, x+1).
vector<Seg> plusFunc() {
    if(Vmax == 0) return { {0,0,0,0} };
    return { {0, Vmax-1, 1, 1}, {Vmax, Vmax, 0, Vmax} };
}
 
// For a minus key: f(x)= max(0, x-1).
vector<Seg> minusFunc() {
    if(Vmax == 0) return { {0,0,0,0} };
    return { {0, 0, 0, 0}, {1, Vmax, 1, -1} };
}
 
// The overall effect on volume is the composition of the functions corresponding to keys 2..N.
// (Key 1 is the unlocking key and does nothing.)
 
// We set up a segment tree whose leaves correspond (in order) to key presses 2..N.
// Initially all leaves hold identity (inactive). When a key “activates” (i.e. when T reaches its gap),
// we update its leaf with plusFunc() or minusFunc() as appropriate.
 
// Our segment tree is built iteratively.
 
// In the following, seg[i] (i from 0 to size-1 in the leaves) holds the function for a key.
// The overall function is the composition (in order) given by
//    F = f_N o f_{N-1} o ... o f_2.
 
// --- Now we process the input and “sweep” over candidate T values.
 
struct Event {
    int gap;   // gap = C[i] - C[i-1] for key press i (i>=2)
    int index; // actual key index (starting from 2)
    char op;   // '+' or '-'
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, V2;
    cin >> N >> Vmax >> V2;
    vector<pair<char,int>> keys(N);
    for (int i=0; i<N; i++){
        cin >> keys[i].first >> keys[i].second;
    }
    // Build events for keys 2..N.
    vector<Event> events;
    for (int i=1; i<N; i++){
        int gap = keys[i].second - keys[i-1].second;
        events.push_back({gap, i+1, keys[i].first});
    }
    // Sort events by gap (in increasing order).
    sort(events.begin(), events.end(), [](const Event &a, const Event &b){
        return a.gap < b.gap;
    });
 
    int nEvents = events.size();
    // Build an iterative segment tree.
    int size = 1;
    while(size < nEvents) size *= 2;
    vector< vector<Seg> > seg(2*size, identityFunc());
    for (int i = 0; i < nEvents; i++){
        seg[size + i] = identityFunc();
    }
    for (int i = nEvents; i < size; i++){
        seg[size + i] = identityFunc();
    }
    for (int i = size-1; i >= 1; i--){
        seg[i] = composeFuncs(seg[2*i], seg[2*i+1]);
    }
    auto getTotalFunc = [&](){
        return seg[1];
    };
    auto updateSeg = [&](int pos, const vector<Seg>& newFunc) {
        int i = pos + size;
        seg[i] = newFunc;
        for(i /= 2; i >= 1; i /= 2){
            seg[i] = composeFuncs(seg[2*i], seg[2*i+1]);
        }
    };
 
    // Now, we “sweep” over candidate values of T.
    // An event becomes active exactly when T >= gap.
    // In between such thresholds the overall transformation is constant.
    int bestT = -1, bestV1 = -1;
    int prevThreshold = 0;
    int iEvent = 0;
    int n = nEvents;
    while(iEvent < n){
        int d = events[iEvent].gap;
        // For T in [prevThreshold, d) the state is unchanged.
        if(prevThreshold <= d-1){
            int candidateT = d-1;
            vector<Seg> currF = getTotalFunc();
            int sol = solveFor(currF, V2);
            if(sol != -1){
                if(candidateT > bestT || (candidateT == bestT && sol > bestV1)){
                    bestT = candidateT;
                    bestV1 = sol;
                }
            }
        }
        // Now update all events with gap equal to d.
        while(iEvent < n && events[iEvent].gap == d){
            int pos = events[iEvent].index - 2; // mapping: key index i (>=2) -> pos = i-2.
            vector<Seg> newFunc;
            if(events[iEvent].op == '+')
                newFunc = plusFunc();
            else
                newFunc = minusFunc();
            updateSeg(pos, newFunc);
            iEvent++;
        }
        // Now the state for T = d.
        {
            int candidateT = d;
            vector<Seg> currF = getTotalFunc();
            int sol = solveFor(currF, V2);
            if(sol != -1){
                if(candidateT > bestT || (candidateT == bestT && sol > bestV1)){
                    bestT = candidateT;
                    bestV1 = sol;
                }
            }
        }
        prevThreshold = d+1;
    }
    // After processing all events, the current state holds for all T >= prevThreshold.
    // If in that state a valid solution exists, then T can be arbitrarily large.
    vector<Seg> currF = getTotalFunc();
    int sol = solveFor(currF, V2);
    if(sol != -1){
        cout << "infinity\n";
        return 0;
    }
    cout << bestT << " " << bestV1 << "\n";
    return 0;
}
