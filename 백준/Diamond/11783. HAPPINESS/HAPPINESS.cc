#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// Structure for our segment tree nodes.
struct Node {
    ll tot; // total sum in this segment (i.e. sum_{v in segment} frequency*v)
    ll req; // required minimum starting value (i.e. the "gap" condition)
};
 
// A sufficiently low "negative infinity" value.
const ll NEG_INF = -LLONG_MAX/4;
 
// Combine two nodes (left A and right B).
// When processing coins in order (A then B), if you start with R, then after A you get R+tot_A.
// To process B you need R+tot_A >= req_B. Hence, overall requirement is: max(A.req, B.req - tot_A).
Node combine(const Node &A, const Node &B) {
    Node res;
    res.tot = A.tot + B.tot;
    res.req = max(A.req, B.req - A.tot);
    return res;
}
 
// Global segment tree container.
vector<Node> seg;
int segSize; // number of leaves in the segment tree
 
// Frequency array for each distinct coin value.
vector<ll> freq;
 
// The sorted list of distinct coin values.
vector<ll> distinctCoins;
 
// Build the segment tree recursively.
void buildTree(int idx, int l, int r) {
    if(l == r) {
        if(freq[l] > 0){
            seg[idx].tot = freq[l] * distinctCoins[l];
            seg[idx].req = distinctCoins[l] - 1;
        } else {
            seg[idx].tot = 0;
            seg[idx].req = NEG_INF;
        }
        return;
    }
    int mid = (l+r)/2;
    buildTree(idx*2, l, mid);
    buildTree(idx*2+1, mid+1, r);
    seg[idx] = combine(seg[idx*2], seg[idx*2+1]);
}
 
// Update a single position (in our compressed array) by delta.
void updateTree(int idx, int l, int r, int pos, ll delta) {
    if(pos < l || pos > r) return;
    if(l == r) {
        freq[pos] += delta;
        if(freq[pos] > 0) {
            seg[idx].tot = freq[pos] * distinctCoins[pos];
            seg[idx].req = distinctCoins[pos] - 1;
        } else {
            seg[idx].tot = 0;
            seg[idx].req = NEG_INF;
        }
        return;
    }
    int mid = (l+r)/2;
    if(pos <= mid) updateTree(idx*2, l, mid, pos, delta);
    else updateTree(idx*2+1, mid+1, r, pos, delta);
    seg[idx] = combine(seg[idx*2], seg[idx*2+1]);
}
 
// Query the entire segment tree.
Node queryAll() {
    return seg[1];
}
 
// Main
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    ll M;
    cin >> N >> M;
    vector<ll> initCoins(N);
    for (int i = 0; i < N; i++){
        cin >> initCoins[i];
    }
    int Q;
    cin >> Q;
 
    // We must collect all coin values that will ever appear (initial + events).
    vector<ll> allValues;
    for (int i = 0; i < N; i++){
        allValues.push_back(initCoins[i]);
    }
    // Read events and collect their coin values.
    vector<tuple<int,int, vector<ll>>> events;
    events.reserve(Q);
    for (int i = 0; i < Q; i++){
        int type, k;
        cin >> type >> k;
        vector<ll> coins(k);
        for (int j = 0; j < k; j++){
            cin >> coins[j];
            allValues.push_back(coins[j]);
        }
        events.push_back({type, k, coins});
    }
 
    // Compress coin values.
    sort(allValues.begin(), allValues.end());
    allValues.erase(unique(allValues.begin(), allValues.end()), allValues.end());
    distinctCoins = allValues;
    int nDistinct = distinctCoins.size();
 
    // Prepare the frequency array.
    freq.assign(nDistinct, 0);
    // Map coin value -> compressed index.
    unordered_map<ll,int> comp;
    comp.reserve(nDistinct*2);
    for (int i = 0; i < nDistinct; i++){
        comp[distinctCoins[i]] = i;
    }
    // Initialize frequencies with the starting coins.
    for (int i = 0; i < N; i++){
        int pos = comp[initCoins[i]];
        freq[pos]++;
    }
 
    // Build the segment tree.
    segSize = nDistinct;
    seg.assign(4 * segSize, {0, NEG_INF});
    buildTree(1, 0, segSize - 1);
 
    // A helper lambda to check happiness:
    // Niya is happy if she has no coins (empty set) or if the merged segment (processed from 0)
    // has a requirement <= 0.
    auto isHappy = [&]() -> bool {
        if(queryAll().tot == 0) return true;
        return (queryAll().req <= 0);
    };
 
    // Print initial happiness status.
    cout << (isHappy() ? 1 : 0) << "\n";
 
    // Process each event.
    for (int i = 0; i < Q; i++){
        int type, k;
        vector<ll> coins;
        tie(type, k, coins) = events[i];
        for (int j = 0; j < k; j++){
            int pos = comp[coins[j]];
            if(type == 1) { // wage: add coins
                updateTree(1, 0, segSize - 1, pos, 1);
            } else if(type == -1) { // shopping: remove coins
                updateTree(1, 0, segSize - 1, pos, -1);
            }
        }
        cout << (isHappy() ? 1 : 0) << "\n";
    }
 
    return 0;
}
