#include <bits/stdc++.h>
using namespace std;
 
// Each segment tree node will store a pair (S, M) where:
//   S = cumulative sum (i.e. sum of p's applied to that day)
//   M = maximum prefix sum achievable (i.e. best total if you “stop” before a negative drag)
struct Node {
    long long S; // maximum cumulative sum among leaves in this segment
    long long M; // maximum prefix sum among leaves in this segment
};
 
// For lazy propagation we store a lazy update as a pair (add, best)
// which represents the effect of a sequence of updates (applied in order)
// when starting from (0,0). For a single update with value p, the lazy update is:
//    (p, (p > 0 ? p : 0))
struct Lazy {
    long long add;
    long long best;
};
 
int segSize;
vector<Node> seg;
vector<Lazy> lazy;
 
// Merge two nodes (they are independent, so we take pointwise maximum).
Node combine(const Node &L, const Node &R) {
    Node res;
    res.S = max(L.S, R.S);
    res.M = max(L.M, R.M);
    return res;
}
 
// Apply a lazy update 'val' to node at index idx.
// For each leaf, the update changes (v, _) -> (v + val.add, max(oldPrefix, v + val.best)).
void applyLazy(int idx, const Lazy &val) {
    long long oldS = seg[idx].S;
    seg[idx].S += val.add;
    seg[idx].M = max(seg[idx].M, oldS + val.best);
    // Compose the lazy update into lazy[idx]: if lazy[idx] was (A,B), now becomes (A+val.add, max(B, A + val.best))
    lazy[idx].add += val.add;
    lazy[idx].best = max(lazy[idx].best, (lazy[idx].add - val.add) + val.best);
}
 
// Push lazy value from node idx to its children.
void pushDown(int idx) {
    if(lazy[idx].add != 0 || lazy[idx].best != 0) {
        int left = idx * 2, right = idx * 2 + 1;
        applyLazy(left, lazy[idx]);
        applyLazy(right, lazy[idx]);
        lazy[idx] = {0, 0};
    }
}
 
// Range update on day interval [qL, qR] with update value p.
// The lazy update corresponding to a single update is (p, (p > 0 ? p : 0)).
void updateRange(int idx, int segL, int segR, int qL, int qR, long long p) {
    if(qR < segL || segR < qL) return;
    if(qL <= segL && segR <= qR) {
        Lazy upd;
        upd.add = p;
        upd.best = (p > 0 ? p : 0);
        applyLazy(idx, upd);
        return;
    }
    pushDown(idx);
    int mid = (segL + segR) / 2;
    updateRange(idx * 2, segL, mid, qL, qR, p);
    updateRange(idx * 2 + 1, mid + 1, segR, qL, qR, p);
    seg[idx] = combine(seg[idx * 2], seg[idx * 2 + 1]);
}
 
// Query the segment tree on day interval [qL, qR].
Node queryRange(int idx, int segL, int segR, int qL, int qR) {
    if(qR < segL || segR < qL)
        return {LLONG_MIN/2, LLONG_MIN/2}; // "empty" node for max query
    if(qL <= segL && segR <= qR) {
        return seg[idx];
    }
    pushDown(idx);
    int mid = (segL + segR) / 2;
    Node leftNode = queryRange(idx * 2, segL, mid, qL, qR);
    Node rightNode = queryRange(idx * 2 + 1, mid + 1, segR, qL, qR);
    if(leftNode.S == LLONG_MIN/2) return rightNode;
    if(rightNode.S == LLONG_MIN/2) return leftNode;
    return combine(leftNode, rightNode);
}
 
// Structure for a player (each can be an invitee).
struct Player {
    int r, a, b;
    int p; // pleasantness
};
 
// For a leader query we need: available day interval [a, b],
// leader pleasantness p, threshold = r_leader - 1 (invite only those with rating <= r_leader-1),
// and the original index.
struct Query {
    int threshold;
    int a, b;
    int p;
    int idx;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n;
    cin >> n;
    vector<Player> players(n);
    for (int i = 0; i < n; i++){
        cin >> players[i].r >> players[i].p >> players[i].a >> players[i].b;
    }
 
    // All players serve as potential invitees.
    vector<Player> invite = players;
    sort(invite.begin(), invite.end(), [](const Player &A, const Player &B){
        return A.r < B.r;
    });
 
    // Build queries: for each leader (in input order) we want to compute
    // answer = leader.p + max_{d in [a, b]} ( max_{X < r_leader} S(d, X) ).
    // Note: Since choosing X = 0 yields 0, the query is always at least 0.
    vector<Query> queries(n);
    for (int i = 0; i < n; i++){
        queries[i] = {players[i].r - 1, players[i].a, players[i].b, players[i].p, i};
    }
    sort(queries.begin(), queries.end(), [](const Query &A, const Query &B){
        return A.threshold < B.threshold;
    });
 
    // Build segment tree over days [1, maxDay] (maxDay is 1e6).
    const int maxDay = 1000000;
    segSize = 1;
    while(segSize < maxDay) segSize *= 2;
    seg.assign(segSize * 2, {0, 0});
    lazy.assign(segSize * 2, {0, 0});
    // (All leaves start with (0,0) meaning “no invitee added yet”.)
 
    // Process queries offline.
    vector<long long> ans(n, 0);
    int invIdx = 0, invN = invite.size();
    // For each query (leader) with threshold = r_leader-1, add all invite players with rating <= threshold.
    for(auto &q : queries){
        while(invIdx < invN && invite[invIdx].r <= q.threshold){
            // For each invite player, update all days in [a, b] with update (p, (p > 0 ? p : 0)).
            updateRange(1, 1, segSize, invite[invIdx].a, invite[invIdx].b, invite[invIdx].p);
            invIdx++;
        }
        // Query the maximum achievable prefix sum over days [q.a, q.b].
        Node res = queryRange(1, 1, segSize, q.a, q.b);
        long long best = res.M; // (always ≥ 0, because “invite no one” gives 0)
        long long tot = q.p + best;
        ans[q.idx] = tot;
    }
 
    // Output the answers in the original input order.
    for (int i = 0; i < n; i++){
        cout << ans[i] << "\n";
    }
    return 0;
}
