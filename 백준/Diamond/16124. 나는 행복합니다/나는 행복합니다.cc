#include <bits/stdc++.h>
using namespace std;
 
const int MOD = 998244353;
 
// 최대 n+1까지의 10^i mod MOD를 미리 계산 (n 최대 1e6)
const int MAXP = 1000005;
int p[MAXP];  // p[i] = 10^i mod MOD
 
// 세그먼트 트리의 노드 구조체
struct Node {
    int len;      // 구간 길이
    int val;      // 구간이 나타내는 정수 mod MOD
    int s[10];    // 각 자릿수별 가중치 합 (이미 자리수 가중치가 반영됨)
    bool lazy;    // lazy 전파가 남아있는지 여부
    int lazyMap[10]; // lazy 치환함수: 원래 d -> lazyMap[d]
};
 
// 전역 세그먼트 트리 배열 (구간 개수 4*n 정도 할당)
Node *seg;
 
// 문자열 S(0-indexed)를 이용해 [l,r] 구간을 담당하는 세그먼트 트리 노드 seg[idx]를 구축 (1-indexed 구간)
void build(const string &S, int idx, int l, int r) {
    if(l == r){
        seg[idx].len = 1;
        int d = S[l-1] - '0';
        seg[idx].val = d;
        for (int i = 0; i < 10; i++) seg[idx].s[i] = 0;
        seg[idx].s[d] = 1;
        seg[idx].lazy = false;
        for (int i = 0; i < 10; i++) seg[idx].lazyMap[i] = i;
        return;
    }
    int mid = (l + r) / 2;
    build(S, idx*2, l, mid);
    build(S, idx*2+1, mid+1, r);
    seg[idx].len = seg[idx*2].len + seg[idx*2+1].len;
    for (int d = 0; d < 10; d++){
        seg[idx].s[d] = ( (long long) seg[idx*2].s[d] * p[ seg[idx*2+1].len ] + seg[idx*2+1].s[d] ) % MOD;
    }
    seg[idx].val = ( (long long) seg[idx*2].val * p[ seg[idx*2+1].len ] + seg[idx*2+1].val ) % MOD;
    seg[idx].lazy = false;
    for (int i = 0; i < 10; i++) seg[idx].lazyMap[i] = i;
}
 
// 노드 seg[idx]에 mapping 함수를 적용 (mapping[d] : 원래 d가 치환되어야 할 값)
// 노드의 val과 s[]를 mapping에 따라 재계산하며, lazyMap도 합성합니다.
inline void applyMapping(int idx, const int mapping[10]) {
    int newVal = 0, newS[10];
    for (int i = 0; i < 10; i++) newS[i] = 0;
    for (int d = 0; d < 10; d++){
        newVal = ( newVal + (long long) mapping[d] * seg[idx].s[d] ) % MOD;
        newS[ mapping[d] ] = ( newS[ mapping[d] ] + seg[idx].s[d] ) % MOD;
    }
    seg[idx].val = newVal;
    for (int d = 0; d < 10; d++) seg[idx].s[d] = newS[d];
    if (!seg[idx].lazy) {
        seg[idx].lazy = true;
        for (int d = 0; d < 10; d++)
            seg[idx].lazyMap[d] = mapping[d];
    } else {
        // lazyMap 합성: 새 lazyMap = mapping o 기존 lazyMap
        for (int d = 0; d < 10; d++){
            seg[idx].lazyMap[d] = mapping[ seg[idx].lazyMap[d] ];
        }
    }
}
 
// lazy 값이 남아있다면 자식 노드로 전파
inline void pushDown(int idx) {
    if(!seg[idx].lazy) return;
    applyMapping(idx*2, seg[idx].lazyMap);
    applyMapping(idx*2+1, seg[idx].lazyMap);
    seg[idx].lazy = false;
    for (int d = 0; d < 10; d++) seg[idx].lazyMap[d] = d;
}
 
// 자식 노드를 합쳐서 현재 노드 값을 갱신
inline void pullUp(int idx) {
    seg[idx].len = seg[idx*2].len + seg[idx*2+1].len;
    for (int d = 0; d < 10; d++){
        seg[idx].s[d] = ( (long long) seg[idx*2].s[d] * p[ seg[idx*2+1].len ] + seg[idx*2+1].s[d] ) % MOD;
    }
    seg[idx].val = ( (long long) seg[idx*2].val * p[ seg[idx*2+1].len ] + seg[idx*2+1].val ) % MOD;
}
 
// [ql,qr] 구간에 대해 mapping 함수를 적용하는 업데이트 (mapping은 배열 mapping[0..9])
void update(int idx, int l, int r, int ql, int qr, const int mapping[10]) {
    if(ql <= l && r <= qr){
        applyMapping(idx, mapping);
        return;
    }
    pushDown(idx);
    int mid = (l + r) / 2;
    if(ql <= mid) update(idx*2, l, mid, ql, qr, mapping);
    if(qr > mid) update(idx*2+1, mid+1, r, ql, qr, mapping);
    pullUp(idx);
}
 
// 구간 [ql,qr]의 결과 노드를 반환하는 쿼리
// (결과 노드의 val가 바로 S[ql..qr]를 정수로 해석한 값임)
Node query(int idx, int l, int r, int ql, int qr) {
    if(ql <= l && r <= qr) {
        return seg[idx];
    }
    pushDown(idx);
    int mid = (l + r) / 2;
    if(qr <= mid)
        return query(idx*2, l, mid, ql, qr);
    else if(ql > mid)
        return query(idx*2+1, mid+1, r, ql, qr);
    else {
        Node leftNode = query(idx*2, l, mid, ql, qr);
        Node rightNode = query(idx*2+1, mid+1, r, ql, qr);
        Node res;
        res.len = leftNode.len + rightNode.len;
        for (int d = 0; d < 10; d++){
            res.s[d] = ( (long long) leftNode.s[d] * p[rightNode.len] + rightNode.s[d] ) % MOD;
        }
        res.val = ( (long long) leftNode.val * p[rightNode.len] + rightNode.val ) % MOD;
        res.lazy = false;
        for (int d = 0; d < 10; d++) res.lazyMap[d] = d;
        return res;
    }
}
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    string S;
    cin >> S;
    int n = S.size();
 
    // p[0]=1, p[i] = 10^i mod MOD (i=1..n)
    p[0] = 1;
    for (int i = 1; i <= n; i++){
        p[i] = (int)((long long)p[i-1]*10 % MOD);
    }
 
    // 세그먼트 트리 배열 할당 (크기 4*n+5)
    seg = new Node[4*n + 5];
    build(S, 1, 1, n);
 
    int Q;
    cin >> Q;
    while(Q--){
        int type;
        cin >> type;
        if(type == 2){
            int L, R;
            cin >> L >> R;
            Node ans = query(1, 1, n, L, R);
            cout << ans.val % MOD << "\n";
        } else {
            int L, R, A, B;
            cin >> L >> R >> A >> B;
            if(A == B) continue;
            // mapping: if digit==A then B, else 그대로
            int mapping[10];
            for (int d = 0; d < 10; d++){
                mapping[d] = (d == A ? B : d);
            }
            update(1, 1, n, L, R, mapping);
        }
    }
    return 0;
}
