#include <bits/stdc++.h>
using namespace std;
 
const int MOD = 1000000007;
 
// 3x3 행렬과 3차원 벡터로 아핀변환을 표현한다.
struct Trans {
    // 행렬 M[3][3]와 벡터 v[3]
    int M[3][3];
    int v[3];
};
 
// 행렬 A, B의 곱 (3x3) : C = A * B
void matMul(const int A[3][3], const int B[3][3], int C[3][3]) {
    int temp[3][3] = {0};
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            long long sum = 0;
            for (int k = 0; k < 3; k++){
                sum += 1LL * A[i][k] * B[k][j];
                if(sum >= 8LL*MOD*MOD) sum %= MOD;
            }
            temp[i][j] = sum % MOD;
        }
    }
    memcpy(C, temp, sizeof(temp));
}
 
// 행렬 A (3x3)와 벡터 b (3)의 곱: res = A * b
void matVecMul(const int A[3][3], const int b[3], int res[3]) {
    int temp[3] = {0};
    for (int i = 0; i < 3; i++){
        long long sum = 0;
        for (int j = 0; j < 3; j++){
            sum += 1LL * A[i][j] * b[j];
        }
        temp[i] = sum % MOD;
    }
    memcpy(res, temp, sizeof(temp));
}
 
// 두 벡터의 덧셈: res = a + b
void vecAdd(const int a[3], const int b[3], int res[3]) {
    for (int i = 0; i < 3; i++){
        res[i] = (a[i] + b[i]) % MOD;
        if(res[i] < 0) res[i] += MOD;
    }
}
 
// 두 아핀변환의 합성 : T = B ∘ A, 즉 먼저 A를 적용한 후 B.
Trans combine(const Trans &A, const Trans &B) {
    Trans ret;
    // ret.M = B.M * A.M
    matMul(B.M, A.M, ret.M);
    // ret.v = B.M * A.v + B.v
    int tmp[3];
    matVecMul(B.M, A.v, tmp);
    for (int i = 0; i < 3; i++){
        ret.v[i] = (tmp[i] + B.v[i]) % MOD;
        if(ret.v[i] < 0) ret.v[i] += MOD;
    }
    return ret;
}
 
// 초기화: identity transformation
Trans makeIdentity() {
    Trans t;
    memset(t.M, 0, sizeof(t.M));
    for (int i = 0; i < 3; i++){
        t.M[i][i] = 1;
    }
    memset(t.v, 0, sizeof(t.v));
    return t;
}
 
// digit 문자 c ('0' ~ '9')에 대한 변환: 
// M_digit = [[10, d, 0], [0, 1, 0], [0, 0, 1]], v_digit = [0,0,0]
Trans makeDigit(char c) {
    Trans t;
    int d = c - '0';
    t.M[0][0] = 10; t.M[0][1] = d; t.M[0][2] = 0;
    t.M[1][0] = 0;  t.M[1][1] = 1; t.M[1][2] = 0;
    t.M[2][0] = 0;  t.M[2][1] = 0; t.M[2][2] = 1;
    for (int i = 0; i < 3; i++) t.v[i] = 0;
    return t;
}
 
// operator 문자 op ('+' 또는 '-')에 대한 변환:
// M_op = [[0,0,0],[0,0,0],[1,0,1]], v_op = [0, op_val, 0] (op_val = 1 if '+' else -1 mod MOD)
Trans makeOp(char op) {
    Trans t;
    memset(t.M, 0, sizeof(t.M));
    // M_op:
    t.M[2][0] = 1; t.M[2][1] = 0; t.M[2][2] = 1;
    // 나머지 행은 0.
    t.v[0] = 0;
    if(op == '+') t.v[1] = 1;
    else t.v[1] = MOD - 1;  // -1 mod MOD
    t.v[2] = 0;
    return t;
}
 
// 주어진 문자 c가 digit인지 operator인지에 따라 변환 생성
Trans makeTrans(char c) {
    if(isdigit(c)) return makeDigit(c);
    else return makeOp(c);
}
 
// 세그먼트 트리 (1-indexed)
int N;
string S;
vector<Trans> seg;
 
// 세그먼트 트리 빌드: node idx가 [l, r] 구간 담당
void build(int idx, int l, int r) {
    if(l == r) {
        seg[idx] = makeTrans(S[l-1]); // S는 0-indexed
        return;
    }
    int mid = (l + r) / 2;
    build(idx*2, l, mid);
    build(idx*2+1, mid+1, r);
    // 구간은 왼쪽부터 오른쪽 순서로 적용하므로 combine( left, right ) = right ∘ left.
    seg[idx] = combine(seg[idx*2], seg[idx*2+1]);
}
 
// 구간 [ql, qr]에 대해 변환을 구한다.
Trans query(int idx, int l, int r, int ql, int qr) {
    if(ql <= l && r <= qr) {
        return seg[idx];
    }
    int mid = (l + r) / 2;
    if(qr <= mid) return query(idx*2, l, mid, ql, qr);
    if(ql > mid) return query(idx*2+1, mid+1, r, ql, qr);
    Trans leftT = query(idx*2, l, mid, ql, qr);
    Trans rightT = query(idx*2+1, mid+1, r, ql, qr);
    return combine(leftT, rightT);
}
 
// 점 업데이트: pos 위치의 문자를 ch로 변경
void update(int idx, int l, int r, int pos, char ch) {
    if(l == r) {
        seg[idx] = makeTrans(ch);
        return;
    }
    int mid = (l + r) / 2;
    if(pos <= mid) update(idx*2, l, mid, pos, ch);
    else update(idx*2+1, mid+1, r, pos, ch);
    seg[idx] = combine(seg[idx*2], seg[idx*2+1]);
}
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> N;
    cin >> S;
    seg.resize(4 * N);
    build(1, 1, N);
 
    int Q;
    cin >> Q;
    while(Q--){
        int t, a, b;
        cin >> t >> a >> b;
        if(t == 1) { // 바꾸기: S[a..b]를 문자열 T로 대체
            string T;
            cin >> T;
            // T의 길이는 b - a + 1
            for (int i = 0; i < T.size(); i++){
                update(1, 1, N, a + i, T[i]);
            }
        } else { // 계산하기: S[a..b]로 계산한 결과 (AC 후 '=')
            Trans Tt = query(1, 1, N, a, b);
            // 초기 상태 X0 = [y, r, acc] = [0, 1, 0]
            // 최종 상태 = Tt.M * X0 + Tt.v.
            // X0 = [0,1,0] 이므로 각 성분은:
            // y = Tt.M[0][1] + Tt.v[0]
            // r = Tt.M[1][1] + Tt.v[1]  (사용하지 않음)
            // acc = Tt.M[2][1] + Tt.v[2]
            int y = (Tt.M[0][1] + Tt.v[0]) % MOD;
            int acc = (Tt.M[2][1] + Tt.v[2]) % MOD;
            if(y < 0) y += MOD;
            if(acc < 0) acc += MOD;
            // 최종 계산 결과 = acc + y (commit되지 않은 숫자 y를 commit)
            int ans = (acc + y) % MOD;
            cout << ans << "\n";
        }
    }
    return 0;
}
