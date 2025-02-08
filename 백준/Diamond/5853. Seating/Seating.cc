#include <bits/stdc++.h>
using namespace std;
 
// Data structure for our segment tree node.
struct Node {
    int l, r;      // the range of seats this node covers
    int left;      // number of contiguous empty seats from the left boundary
    int right;     // number of contiguous empty seats from the right boundary
    int maxi;      // maximum contiguous empty seats within this interval
    int lazy;      // lazy propagation flag: -1 means none, 0 means occupied, 1 means empty
};
 
const int MAXN = 500000;
Node seg[4 * MAXN + 5];
 
// Build the segment tree over seats [L, R]. Initially, all seats are empty.
void build(int idx, int L, int R) {
    seg[idx].l = L;
    seg[idx].r = R;
    seg[idx].lazy = -1;  // no lazy update pending
    if(L == R){
        seg[idx].left = seg[idx].right = seg[idx].maxi = 1;
        return;
    }
    int mid = (L + R) >> 1;
    build(idx * 2, L, mid);
    build(idx * 2 + 1, mid + 1, R);
    int leftLen = mid - L + 1;
    int rightLen = R - mid;
    seg[idx].left = seg[idx*2].left;
    if(seg[idx*2].left == leftLen)
        seg[idx].left += seg[idx*2+1].left;
    seg[idx].right = seg[idx*2+1].right;
    if(seg[idx*2+1].right == rightLen)
        seg[idx].right += seg[idx*2].right;
    seg[idx].maxi = max({seg[idx*2].maxi, seg[idx*2+1].maxi, seg[idx*2].right + seg[idx*2+1].left});
}
 
// Push lazy updates to children.
void pushDown(int idx) {
    if(seg[idx].lazy != -1) {
        int lazyVal = seg[idx].lazy;
        int leftIdx = idx * 2, rightIdx = idx * 2 + 1;
        seg[leftIdx].lazy = lazyVal;
        seg[rightIdx].lazy = lazyVal;
        int L = seg[leftIdx].l, R = seg[leftIdx].r;
        int lenL = R - L + 1;
        int L2 = seg[rightIdx].l, R2 = seg[rightIdx].r;
        int lenR = R2 - L2 + 1;
        if(lazyVal == 1) { // mark as empty
            seg[leftIdx].left = seg[leftIdx].right = seg[leftIdx].maxi = lenL;
            seg[rightIdx].left = seg[rightIdx].right = seg[rightIdx].maxi = lenR;
        } else { // mark as occupied
            seg[leftIdx].left = seg[leftIdx].right = seg[leftIdx].maxi = 0;
            seg[rightIdx].left = seg[rightIdx].right = seg[rightIdx].maxi = 0;
        }
        seg[idx].lazy = -1;
    }
}
 
// Update the seats in the range [ql, qr] to val (1 means empty, 0 means occupied).
void update(int idx, int ql, int qr, int val) {
    int L = seg[idx].l, R = seg[idx].r;
    if(ql <= L && R <= qr) {
        seg[idx].lazy = val;
        int len = R - L + 1;
        if(val == 1) {
            seg[idx].left = seg[idx].right = seg[idx].maxi = len;
        } else {
            seg[idx].left = seg[idx].right = seg[idx].maxi = 0;
        }
        return;
    }
    pushDown(idx);
    int mid = (L + R) >> 1;
    if(ql <= mid)
        update(idx * 2, ql, qr, val);
    if(qr > mid)
        update(idx * 2 + 1, ql, qr, val);
    int leftLen = mid - L + 1;
    int rightLen = R - mid;
    seg[idx].left = seg[idx*2].left;
    if(seg[idx*2].left == leftLen)
        seg[idx].left += seg[idx*2+1].left;
    seg[idx].right = seg[idx*2+1].right;
    if(seg[idx*2+1].right == rightLen)
        seg[idx].right += seg[idx*2].right;
    seg[idx].maxi = max({seg[idx*2].maxi, seg[idx*2+1].maxi, seg[idx*2].right + seg[idx*2+1].left});
}
 
// Query for the leftmost starting position where there is a contiguous block of at least p empty seats.
// Precondition: seg[1].maxi >= p.
int query(int idx, int p) {
    int L = seg[idx].l, R = seg[idx].r;
    if(L == R)
        return L;
    pushDown(idx);
    int mid = (L + R) >> 1;
    int leftIdx = idx * 2, rightIdx = idx * 2 + 1;
    if(seg[leftIdx].maxi >= p)
        return query(leftIdx, p);
    else if(seg[leftIdx].right + seg[rightIdx].left >= p)
        return mid - seg[leftIdx].right + 1;
    else
        return query(rightIdx, p);
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    build(1, 1, N);
    long long turnedAway = 0;
    for (int i = 0; i < M; i++){
        char type;
        cin >> type;
        if(type == 'A'){
            int p;
            cin >> p;
            // Check if there is any contiguous empty block of size >= p
            if(seg[1].maxi < p) {
                turnedAway++;
            } else {
                int pos = query(1, p);
                // Occupy seats from pos to pos+p-1
                update(1, pos, pos+p-1, 0);
            }
        } else { // 'L' event: seats in the interval become empty.
            int a, b;
            cin >> a >> b;
            update(1, a, b, 1);
        }
    }
    cout << turnedAway << "\n";
    return 0;
}
