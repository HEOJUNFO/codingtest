#include <bits/stdc++.h>
using namespace std;
 
const long long MOD = 1000000007;
 
// 4x4 행렬 구조체
struct Matrix {
    long long m[4][4];
    Matrix() {
        memset(m, 0, sizeof(m));
    }
};
 
// 행렬 곱셈 (4x4)
Matrix multiply(const Matrix &A, const Matrix &B) {
    Matrix C;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            __int128 sum = 0;
            for (int k = 0; k < 4; k++){
                sum += A.m[i][k] * B.m[k][j];
            }
            C.m[i][j] = (long long)(sum % MOD);
        }
    }
    return C;
}
 
// 각 열의 상태에 따른 전이행렬 f(s)
// s는 bitmask (bit0: row1, bit1: row2)가 free이면 1, 제한이면 0.
Matrix getMat(int s) {
    Matrix M;
    // 모든 상태는 0로 초기화.
    // 가능한 incoming mask = i, outgoing mask = j, 0<= i,j < 4.
    // 유효하지 않은 경우에는 0.
    // 조건: incoming mask i must be subset of s.
    // 남은 free cells: r = s - i.
    // domino 배치를 통해 r를 덮고, 그 결과로 column i에서 column i+1으로 넘어가는 horizontal domino의 자리(즉, outgoing mask j)를 결정.
 
    // s = 0 : 아무 칸도 사용 불가. 유일한 경우: i == 0, j == 0.
    if(s == 0) {
        M.m[0][0] = 1;
        return M;
    }
    // s = 1 : 오직 row1만 사용 가능.
    if(s == 1) {
        // i must be subset of {bit0}. 즉, i = 0 또는 i = 1.
        // case i = 0: free cell = {row1}. 오직 horizontal domino 가능 (row1에서 column i와 i+1을 덮음)
        M.m[0][1] = 1; // outgoing mask = 1 (bit0 set)
        // case i = 1: 이미 채워진 경우, 아무것도 덮을 필요 없으므로 outgoing mask = 0.
        M.m[1][0] = 1;
        return M;
    }
    // s = 2 : 오직 row2만 사용 가능. (bit1)
    if(s == 2) {
        // 가능한 i: 0 또는 2.
        // case i = 0: free cell = {row2}. horizontal domino: outgoing mask = 2.
        M.m[0][2] = 1;
        // case i = 2: free cell 없음 → outgoing mask = 0.
        M.m[2][0] = 1;
        return M;
    }
    // s = 3 : 둘 다 사용 가능.
    if(s == 3) {
        // 가능한 incoming mask: 0, 1, 2, 3.
        // case i = 0: free = {row1, row2}.
        //   - option 1: vertical domino: 덮으면 outgoing mask = 0.
        M.m[0][0] = 1;
        //   - option 2: horizontal domino 둘: outgoing mask = (bit0+bit1) = 3.
        M.m[0][3] = 1;
        // case i = 1: (row1 already filled), free = {row2}.
        //   - must place horizontal domino on row2: outgoing mask = 2.
        M.m[1][2] = 1;
        // case i = 2: (row2 already filled), free = {row1}.
        //   - horizontal domino on row1: outgoing mask = 1.
        M.m[2][1] = 1;
        // case i = 3: (both already filled), free = {}.
        //   - no domino, outgoing mask = 0.
        M.m[3][0] = 1;
        return M;
    }
    return M; // never reach here
}
 
// 세그먼트 트리 (각 노드가 4x4 행렬)
struct SegTree {
    int n;
    vector<Matrix> tree;
 
    SegTree(int n) : n(n) {
        tree.resize(4 * n);
    }
 
    void build(vector<int> &s, int idx, int l, int r) {
        if(l == r) {
            tree[idx] = getMat(s[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(s, idx*2, l, mid);
        build(s, idx*2+1, mid+1, r);
        tree[idx] = multiply(tree[idx*2], tree[idx*2+1]);
    }
 
    void update(int idx, int l, int r, int pos, int newS) {
        if(l == r) {
            tree[idx] = getMat(newS);
            return;
        }
        int mid = (l + r) / 2;
        if(pos <= mid)
            update(idx*2, l, mid, pos, newS);
        else
            update(idx*2+1, mid+1, r, pos, newS);
        tree[idx] = multiply(tree[idx*2], tree[idx*2+1]);
    }
};
 
// bit mask for row: row1 -> bit0, row2 -> bit1
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, Q;
    cin >> N >> Q;
    // 초기 상태: 모든 열에 대해 s = 3 (양쪽 모두 사용 가능)
    vector<int> s(N+1, 3);
 
    SegTree seg(N);
    seg.build(s, 1, 1, N);
 
    // 쿼리 처리
    for(int i = 0; i < Q; i++){
        int t, x, y;
        cin >> t >> x >> y;
        // x: row (1 또는 2), y: column.
        int bit = (x == 1 ? 1 : 2); // row1 -> 1, row2 -> 2.
        if(t == 1){
            // 제한 추가: 해당 칸을 사용할 수 없도록 => 해당 bit를 0.
            s[y] &= ~bit;
        } else {
            // 제한 삭제: 해당 칸을 다시 사용 가능 => 해당 bit를 1.
            s[y] |= bit;
        }
        // 세그먼트 트리 update
        seg.update(1, 1, N, y, s[y]);
 
        // 전체 전이행렬의 (0,0) entry가 정답.
        long long ans = seg.tree[1].m[0][0] % MOD;
        cout << ans << "\n";
    }
    return 0;
}
