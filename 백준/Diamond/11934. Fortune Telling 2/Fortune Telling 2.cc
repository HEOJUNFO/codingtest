#include <bits/stdc++.h>
using namespace std;
 
// Global variables and arrays:
int N, K;
vector<long long> A, B;
vector<int> T; // using 1-indexing for T: T[1..K]
 
// Segment tree variables:
int segSize;
vector<int> seg; // our segment tree array (size 2*segSize)
 
// Build the segment tree on T[1..K]. (We use segSize = smallest power of 2 ≥ K.)
void buildSegTree() {
    segSize = 1;
    while(segSize < K) segSize *= 2;
    seg.assign(2 * segSize, 0);
    for (int i = 0; i < K; i++){
        seg[segSize + i] = T[i+1]; // T is 1-indexed; leaves at indices segSize to segSize+K-1.
    }
    for (int i = K; i < segSize; i++){
        seg[segSize + i] = 0;
    }
    for (int i = segSize - 1; i >= 1; i--){
        seg[i] = max(seg[2*i], seg[2*i+1]);
    }
}
 
// Recursive query: given a segment tree node covering [l, r] and a query range [ql, r],
// returns the smallest index in [ql, r] (within current segment) such that T[index] >= v.
// If none exists, returns -1.
int query(int idx, int l, int r, int ql, int v) {
    if(r < ql) return -1; // no overlap
    if(l >= ql) {
        if(seg[idx] < v) return -1; // no element in this segment qualifies
        if(l == r) return l;  // leaf: this is the index we need.
    }
    int mid = (l+r) / 2;
    int leftAns = query(2*idx, l, mid, ql, v);
    if(leftAns != -1) return leftAns;
    return query(2*idx+1, mid+1, r, ql, v);
}
 
// Helper: given a query starting at index ql and a threshold v,
// returns the smallest j in [ql, K] with T[j] >= v, or -1 if not found.
int next_index(int ql, int v) {
    return query(1, 1, segSize, ql, v);
}
 
/*
  The fortune‐telling process is as follows:
  
    - Initially, card i shows A[i].
    - For each operation j (j = 1,...,K in order) the professor “checks” each card.
      If the number currently shown is ≤ T[j] (i.e. T[j] is at least that value),
      the card is flipped – that is, it changes to the number on its other side.
  
  For each card the simulation is independent. Note that if a card is flipped,
  then its “flip‐condition” changes (from checking A to checking B, or vice–versa).
  
  Our simulation for a card i works as:
  
      state = 0;   // 0 means currently showing A[i], 1 means showing B[i]
      pos = 1;     // next operation to consider (operations are 1-indexed)
      flips = 0;
      while (true) {
          let curVal = (state==0 ? A[i] : B[i]);
          j = next_index(pos, curVal);  // find first operation j (≥ pos) with T[j] ≥ curVal.
          if (j == -1) break;           // no further flip happens.
          flips++,
          pos = j+1;
          state = 1 - state;            // toggle the state.
      }
      final card value = (if flips is even then A[i], else B[i]).
  
  Finally, we sum the final values for all cards.
*/
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> K;
    A.resize(N);
    B.resize(N);
    for (int i = 0; i < N; i++){
        cin >> A[i] >> B[i];
    }
    T.resize(K+1);
    for (int j = 1; j <= K; j++){
        cin >> T[j];
    }
    buildSegTree();
 
    long long ans = 0;
    // Process each card independently.
    for (int i = 0; i < N; i++){
        int state = 0; // 0: showing A[i]; 1: showing B[i]
        int pos = 1;   // next operation index to consider
        int flips = 0;
        while (true) {
            int curVal = (state == 0 ? A[i] : B[i]);
            int j = next_index(pos, curVal);
            if(j == -1) break;
            flips++;
            pos = j + 1;
            state = 1 - state;
        }
        // If flips is even, the card ends showing A[i]; if odd, showing B[i].
        ans += (flips % 2 == 0 ? A[i] : B[i]);
    }
    
    cout << ans << "\n";
    return 0;
}
