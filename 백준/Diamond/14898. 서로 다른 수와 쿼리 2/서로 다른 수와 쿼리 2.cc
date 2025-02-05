#include <bits/stdc++.h>
using namespace std;
 
// persistent segment tree 를 위한 노드 구조체
struct Node {
    int l, r; // 좌/우 자식 노드의 인덱스
    int sum;  // 해당 구간의 합 (여기서는 업데이트한 값의 합)
};
 
// 최대 노드 개수 (N 최대 1e6, log2(N) ≒ 20~22 → 약 22e6 정도 필요)
const int MAX_NODE = 22000000;
static Node tree[MAX_NODE];
int nodeCount = 0; // 할당된 노드 수; 노드는 1번부터 사용합니다.
// tree[0]는 null 노드 (모든 값 0) 로 사용합니다.
 
// [s, e] 구간에서 pos 위치에 val을 더하는 업데이트 함수 (영속 구조로 새 노드 할당)
int update(int prev, int s, int e, int pos, int val) {
    int curr = ++nodeCount; // 새 노드 할당
    if(s == e) {
        tree[curr].sum = tree[prev].sum + val;
        tree[curr].l = tree[curr].r = 0;
        return curr;
    }
    int mid = (s + e) >> 1;
    if(pos <= mid) {
        tree[curr].l = update(tree[prev].l, s, mid, pos, val);
        tree[curr].r = tree[prev].r; // 오른쪽 자식은 그대로 복사
    } else {
        tree[curr].l = tree[prev].l;
        tree[curr].r = update(tree[prev].r, mid+1, e, pos, val);
    }
    tree[curr].sum = tree[ tree[curr].l ].sum + tree[ tree[curr].r ].sum;
    return curr;
}
 
// persistent segment tree 에서 [qs, qe] 구간 합을 구하는 함수
int query(int idx, int s, int e, int qs, int qe){
    if(qs > e || qe < s) return 0;
    if(qs <= s && e <= qe)
        return tree[idx].sum;
    int mid = (s + e) >> 1;
    return query(tree[idx].l, s, mid, qs, qe) + query(tree[idx].r, mid+1, e, qs, qe);
}
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    vector<int> A(N+1);
    for (int i = 1; i <= N; i++){
        cin >> A[i];
    }
    // 값의 범위가 최대 1e9 이므로 좌표압축 실시
    vector<int> compArr(A.begin()+1, A.end());
    sort(compArr.begin(), compArr.end());
    compArr.erase(unique(compArr.begin(), compArr.end()), compArr.end());
    for (int i = 1; i <= N; i++){
        // 압축된 값은 1부터 시작
        A[i] = int(lower_bound(compArr.begin(), compArr.end(), A[i]) - compArr.begin()) + 1;
    }
    int M = compArr.size();
 
    // L[i] : A[i]의 바로 이전 등장 위치 (없으면 0)
    vector<int> lastOccurrence(M+1, 0);
    vector<int> L_arr(N+1, 0);
    for (int i = 1; i <= N; i++){
        int val = A[i];
        L_arr[i] = lastOccurrence[val];  // 만약 이전에 없으면 0
        lastOccurrence[val] = i;
    }
 
    // persistent segment tree 구성
    // 각 버전은 1~i 까지의 L_arr 값을 반영하며,
    // 트리의 인덱스 범위는 [0, N] (L_arr[i]의 값은 0 ~ N)
    vector<int> root(N+1, 0);
    // null 노드(tree[0])는 이미 모두 0으로 초기화
    root[0] = 0;
    for (int i = 1; i <= N; i++){
        // index L_arr[i] 에 1을 더한다.
        root[i] = update(root[i-1], 0, N, L_arr[i], 1);
    }
 
    // 쿼리 처리
    // 각 쿼리는 두 정수 xi와 ri가 주어지고,
    // li = xi + Q(i-1) (첫 쿼리의 Q(0)=0) 로 정해집니다.
    // [l, r] 구간의 서로 다른 수의 개수는,
    // "i in [l, r] 중 L_arr[i] < l 인 i의 개수"와 같습니다.
    // 즉, answer = query(root[r], 0, N, 0, l-1) - query(root[l-1], 0, N, 0, l-1)
    int Q;
    cin >> Q;
    long long prevAns = 0; // Q0 = 0
    for (int i = 1; i <= Q; i++){
        int xi, ri;
        cin >> xi >> ri;
        int li = xi + prevAns; // li 계산
        // 문제 조건에 의해 1 ≤ li ≤ ri ≤ N 이라고 가정됨.
        int ans = query(root[ri], 0, N, 0, li-1) - query(root[li-1], 0, N, 0, li-1);
        cout << ans << "\n";
        prevAns = ans; // 다음 쿼리를 위해 이전 답 갱신
    }
 
    return 0;
}
