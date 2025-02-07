#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// persistent segment tree 노드 구조체 (필요한 정보만 저장)
struct Node {
    int cnt;    // 이 노드(구간)에 포함된 원소 개수
    ll sum;     // 이 노드(구간)의 합계
    int left, right;  // 자식 노드의 인덱스
};

// n 최대 100,000이고, 각 persistent tree는 O(log n) 노드를 새로 생성하므로,
// 최대 노드 수를 n*(⌈log₂(n)⌉+2) 정도로 추정하여 약 210만 개로 잡는다.
const int MAX_NODES = 2100000;
Node seg[MAX_NODES];
int segSize = 0;  // 사용한 노드 수

// 각 입력 인덱스에 대한 persistent tree의 root를 저장 (n 최대 100,000)
int root[100005];

// 좌표 압축을 위한 벡터
vector<int> comp;

// 새로운 노드를 생성하여 리턴 (복사 비용 최소화를 위해 inline)
inline int newNode(int cnt, ll sum, int left, int right) {
    seg[segSize] = {cnt, sum, left, right};
    return segSize++;
}

// [l, r] 구간에 대해 초기 트리(모두 0 값)를 빌드 (공간 최적화를 위해 재귀 호출)
int buildTree(int l, int r) {
    int cur = newNode(0, 0, -1, -1);
    if(l == r) return cur;
    int mid = (l + r) / 2;
    seg[cur].left = buildTree(l, mid);
    seg[cur].right = buildTree(mid+1, r);
    return cur;
}
 
// prev 버전의 persistent tree를 복사하여, pos 위치(좌표압축된 값)에 업데이트 적용
int update(int prev, int l, int r, int pos) {
    int cur = newNode(seg[prev].cnt, seg[prev].sum, seg[prev].left, seg[prev].right);
    if(l == r) {
        seg[cur].cnt++;
        seg[cur].sum += comp[l];
        return cur;
    }
    int mid = (l + r) / 2;
    if(pos <= mid)
        seg[cur].left = update(seg[prev].left, l, mid, pos);
    else
        seg[cur].right = update(seg[prev].right, mid+1, r, pos);
    seg[cur].cnt = seg[seg[cur].left].cnt + seg[seg[cur].right].cnt;
    seg[cur].sum = seg[seg[cur].left].sum + seg[seg[cur].right].sum;
    return cur;
}
 
// rTree와 lTree(구간 [L,R]에 해당하는 persistent tree들의 차분)를 사용하여,
// 구간 전체 [l, r]에서 상위 k개 원소의 합을 질의한다.
// (여기서 comp[]는 오름차순 정렬되어 있으므로, 큰 값은 오른쪽에 있다.)
ll queryTopK(int rTree, int lTree, int l, int r, int k) {
    if(k <= 0) return 0;
    if(l == r) {
        return (ll) k * comp[l];
    }
    int mid = (l + r) / 2;
    // 오른쪽 자식에 포함된 원소 수 차이 (구간 [mid+1, r]에서의 개수)
    int cntRight = seg[ seg[rTree].right ].cnt - (lTree == -1 ? 0 : seg[ seg[lTree].right ].cnt);
    if(cntRight >= k)
        return queryTopK(seg[rTree].right, (lTree == -1 ? -1 : seg[lTree].right), mid+1, r, k);
    else {
        ll sumRight = seg[ seg[rTree].right ].sum - (lTree == -1 ? 0 : seg[ seg[lTree].right ].sum);
        return sumRight + queryTopK(seg[rTree].left, (lTree == -1 ? -1 : seg[lTree].left), l, mid, k - cntRight);
    }
}
 
// persistent tree에서 [L,R] 구간의 다중집합에 대해, 상위 k개 원소의 합을 질의한다.
// L==0이면 lTree는 빈 트리(-1)로 처리한다.
ll queryInterval(int L, int R, int k, int M) {
    if(L > R) return 0;
    if(L == 0)
        return queryTopK(root[R], -1, 0, M-1, k);
    else
        return queryTopK(root[R], root[L-1], 0, M-1, k);
}
 
// ---------------------- 문제 관련 함수 ----------------------

// 입력 도시의 관광지 개수를 저장할 배열
vector<ll> A;
// 전역 답안 변수
ll ans = 0;
// n: 도시 수, s: 시작 도시(0기반), d: 휴가 일수
int n, s, d;
 
// 구간 [L,R] (L ≤ s ≤ R)에서, 이동(여행)일수 T 계산: 구간 길이 + 양쪽 끝 중 가까운 쪽 추가 이동
int travelTime(int L, int R) {
    return (R - L) + min(s - L, R - s);
}
 
// 구간 [L,R]에서, 이동일수 T 내에 남은 방문일수 available = d – T 만큼
// 실제 방문할 수 있는 도시는 m = min(available, R–L+1)개이며,
// 이때 persistent 세그먼트 트리로 구간 [L,R] 내 상위 m개 관광지 합을 질의한다.
ll candidateVal(int L, int R, int M) {
    int T = travelTime(L, R);
    if(T > d) return -1;  // d일 내에 커버 불가
    int available = d - T;
    int m = min(available, R - L + 1);
    return queryInterval(L, R, m, M);
}
 
// Divide and Conquer 최적화 (R ∈ [s, n-1]에 대해 최적의 L ∈ [0, s]를 찾는다)
// optL, optR는 이전 R에서 최적 L의 범위가 단조증가함을 이용
void dnc(int lo, int hi, int optL, int optR, int M) {
    if(lo > hi) return;
    int mid = (lo + hi) / 2;
    int bestL = optL;
    ll bestVal = -1;
    // 왼쪽 끝 L은 0 ≤ L ≤ s 여야 함
    for (int Lcandidate = optL; Lcandidate <= min(optR, s); Lcandidate++) {
        ll curVal = candidateVal(Lcandidate, mid, M);
        if(curVal > bestVal) {
            bestVal = curVal;
            bestL = Lcandidate;
        }
    }
    ans = max(ans, bestVal);
    dnc(lo, mid - 1, optL, bestL, M);
    dnc(mid + 1, hi, bestL, optR, M);
}
 
// ---------------------- 메인 ----------------------
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> s >> d;  // n: 도시 수, s: 시작 도시(0기반), d: 휴가일수
    A.resize(n);
    for (int i = 0; i < n; i++){
        cin >> A[i];
        comp.push_back((int)A[i]);
    }
    // 좌표 압축 (관광지 개수의 중복 제거)
    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());
    int M = comp.size();  // 세그먼트 트리 리프 개수
    
    // persistent 세그먼트 트리 빌드: 공간 최적화를 위해 필요한 최소 노드만 생성
    segSize = 0;  // 전역 노드 카운터 초기화
    int base = buildTree(0, M-1);  // 모두 0인 초기 트리
    // root[0]에 첫 번째 도시 업데이트 (A[0])
    root[0] = update(base, 0, M-1, int(lower_bound(comp.begin(), comp.end(), A[0]) - comp.begin()));
    for (int i = 1; i < n; i++){
        int pos = int(lower_bound(comp.begin(), comp.end(), A[i]) - comp.begin());
        root[i] = update(root[i-1], 0, M-1, pos);
    }
    
    // 방문 구간은 반드시 시작 도시 s를 포함해야 하므로,
    // 왼쪽 끝 L ∈ [0, s] / 오른쪽 끝 R ∈ [s, n-1]에 대해 최적의 구간을 divide and conquer 최적화로 찾는다.
    dnc(s, n-1, 0, s, M);
    
    cout << ans << "\n";
    return 0;
}
