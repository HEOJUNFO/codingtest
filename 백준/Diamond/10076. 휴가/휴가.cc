#include <bits/stdc++.h>
using namespace std;
 
// --- Persistent Segment Tree ---
// 이 구조체는 한 노드의 정보를 저장합니다.
struct Node {
    int left, right; // 자식 노드 인덱스
    int cnt;         // 이 구간에 포함된 원소의 개수
    long long sum;   // 이 구간의 원소들의 합
};
 
// 전역 상수 및 배열 (최대 노드 수는 대략 n*log(n) 정도로 잡음)
const int MAX_NODES = 2200000;
Node seg[MAX_NODES];
int segSize = 1; // 0번 노드를 null node로 사용하고, 실제 노드는 1번부터
 
// 전역 변수 (압축된 값 목록)
int m; // 압축된 값의 개수
vector<long long> compVals; // compVals[i] : i번째로 작은 원래 값
 
// 새 노드를 만들어 반환합니다.
int newNode(int left, int right, int cnt, long long sum) {
    int idx = segSize++;
    seg[idx].left = left;
    seg[idx].right = right;
    seg[idx].cnt = cnt;
    seg[idx].sum = sum;
    return idx;
}
 
// persistent segment tree의 update 함수 
// prev: 이전 버전의 root, pos: 압축된 인덱스, val: 원래 값
// 구간 [L, R] 내에서 update합니다.
int update(int prev, int pos, long long val, int L, int R) {
    int curr = newNode(0, 0, 0, 0);
    if(L == R) {
        seg[curr].cnt = seg[prev].cnt + 1;
        seg[curr].sum = seg[prev].sum + val;
        return curr;
    }
    int mid = (L + R) / 2;
    if(pos <= mid) {
        int newLeft = update(seg[prev].left, pos, val, L, mid);
        seg[curr].left = newLeft;
        seg[curr].right = seg[prev].right;
    } else {
        int newRight = update(seg[prev].right, pos, val, mid+1, R);
        seg[curr].right = newRight;
        seg[curr].left = seg[prev].left;
    }
    seg[curr].cnt = seg[ seg[curr].left ].cnt + seg[ seg[curr].right ].cnt;
    seg[curr].sum = seg[ seg[curr].left ].sum + seg[ seg[curr].right ].sum;
    return curr;
}
 
// --- Query : "k largest" 합 구하기 ---
// u, v: persistent segment tree의 두 버전 (구간 차분용)
// 구간 [L,R] (압축값 범위) 에서 k개의 가장 큰 원소의 합을 구합니다.
long long queryK (int u, int v, int k, int L, int R) {
    if(k <= 0) return 0;
    if(L == R) {
        // 해당 leaf의 원래 값은 compVals[L]
        return (long long) k * compVals[L];
    }
    int mid = (L + R) / 2;
    // 오른쪽 자식 구간 ([mid+1, R])의 원소 개수부터 가져옵니다.
    int cntRight = seg[ seg[u].right ].cnt - seg[ seg[v].right ].cnt;
    if(cntRight >= k)
        return queryK(seg[u].right, seg[v].right, k, mid+1, R);
    else {
        long long sumRight = seg[ seg[u].right ].sum - seg[ seg[v].right ].sum;
        int rem = k - cntRight;
        long long sumLeft = queryK(seg[u].left, seg[v].left, rem, L, mid);
        return sumRight + sumLeft;
    }
}
 
// 주어진 persistent tree (roots 배열 이용) 에서 구간 [l, r]의 k largest 합을 구합니다.
long long queryInterval(int l, int r, int k, const vector<int>& roots) {
    if(k <= 0) return 0;
    int u = roots[r];
    int v = (l > 0 ? roots[l-1] : 0);
    return queryK(u, v, k, 0, m-1);
}
 
// --- Main ---
 
// 전역 입력 변수
int n, start;
long long d;
 
// travelCost : 구간 [L,R] (L ≤ start ≤ R)에 대해
// 이동 비용 = (R - L) + min(start - L, R - start)
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n >> start >> d;
    vector<long long> A(n);
    for (int i=0; i<n; i++){
        cin >> A[i];
    }
 
    // 좌표 압축
    vector<long long> temp = A;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    m = temp.size();
    compVals = temp;
 
    // persistent segment tree의 null node (인덱스 0) 초기화
    seg[0].left = seg[0].right = 0;
    seg[0].cnt = 0;
    seg[0].sum = 0;
 
    // roots[i] : A[0..i]를 반영한 persistent tree의 root
    vector<int> roots(n, 0);
    int pos = int(lower_bound(compVals.begin(), compVals.end(), A[0]) - compVals.begin());
    roots[0] = update(0, pos, A[0], 0, m-1);
    for (int i=1; i<n; i++){
        int pos = int(lower_bound(compVals.begin(), compVals.end(), A[i]) - compVals.begin());
        roots[i] = update(roots[i-1], pos, A[i], 0, m-1);
    }
 
    // 람다: 구간 [L,R] (0 ≤ L ≤ start ≤ R < n) 에 대한 이동 비용 T = (R - L) + min(start - L, R - start)
    auto travelCost = [&](int L, int R) -> long long {
        long long diff = R - L;
        long long leftDist = start - L;
        long long rightDist = R - start;
        return diff + min(leftDist, rightDist);
    };
 
    long long ans = 0;
    // (특별히) 아무 이동도 없이 start 도시만 방문하는 경우
    ans = max(ans, A[start]);
 
    // [L,R] 구간이 start를 포함하고, T(L,R) <= d인 극단적 후보 구간들을 고려합니다.
    // 1) "좌측 확장" : L을 0부터 start까지 변화시키고, 이분검색으로 R (최대 feasible R)을 찾습니다.
    for (int L = 0; L <= start; L++){
        int lo = start, hi = n-1;
        int bestR = -1;
        while(lo <= hi){
            int mid = (lo + hi) / 2;
            long long cost = travelCost(L, mid);
            if(cost <= d){
                bestR = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if(bestR == -1) continue;
        int R = bestR;
        int blockLength = R - L + 1;
        long long cost = travelCost(L, R);
        long long avail = d - cost; // 남은 방문할 수 있는 날들
        int visits = (int) min((long long)blockLength, avail);
        if(visits <= 0) continue;
        long long cand = queryInterval(L, R, visits, roots);
        ans = max(ans, cand);
    }
 
    // 2) "우측 확장" : R을 start부터 n-1까지 변화시키고, 이분검색으로 L (최소 feasible L)을 찾습니다.
    for (int R = start; R < n; R++){
        int lo = 0, hi = start;
        int bestL = -1;
        while(lo <= hi){
            int mid = (lo + hi) / 2;
            long long cost = travelCost(mid, R);
            if(cost <= d){
                bestL = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        if(bestL == -1) continue;
        int L = bestL;
        int blockLength = R - L + 1;
        long long cost = travelCost(L, R);
        long long avail = d - cost;
        int visits = (int) min((long long)blockLength, avail);
        if(visits <= 0) continue;
        long long cand = queryInterval(L, R, visits, roots);
        ans = max(ans, cand);
    }
 
    cout << ans << "\n";
    return 0;
}
