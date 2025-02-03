#include <bits/stdc++.h>
using namespace std;
 
// persistent segment tree – 각 노드에는 구간의 길이, 좌측부터 연속한 1의 길이(lval), 우측부터 연속한 1의 길이(rval), 그리고 구간 내 최대 연속 1의 개수(best)를 저장합니다.
 
struct Node {
    int left, right; // 자식 노드 인덱스
    int len;         // 구간 길이
    int lval;        // 구간 왼쪽 끝부터 연속 1의 개수
    int rval;        // 구간 오른쪽 끝부터 연속 1의 개수
    int best;        // 구간 내 최대 연속 1의 개수
};
 
// 전역 persistent 세그먼트 트리 저장소
const int MAX_NODES = 5000000; // N<=100,000, logN≈18 → 여유 있게 할당
vector<Node> segTree;
int nodeCount = 0;
 
// [l, r] 구간에 대해 초기 트리(모두 0)를 재귀적으로 생성
int build(int l, int r) {
    int idx = nodeCount++;
    segTree[idx].left = segTree[idx].right = -1;
    segTree[idx].len = r - l + 1;
    if(l == r){
        segTree[idx].lval = 0;
        segTree[idx].rval = 0;
        segTree[idx].best = 0;
    } else {
        int mid = (l + r) / 2;
        segTree[idx].left = build(l, mid);
        segTree[idx].right = build(mid+1, r);
        // 두 자식의 값은 모두 0이므로 그대로
        segTree[idx].lval = 0;
        segTree[idx].rval = 0;
        segTree[idx].best = 0;
    }
    return idx;
}
 
// 두 구간의 정보를 합치는 함수 (query시 사용)
struct Info {
    int len, lval, rval, best;
};
 
Info combineInfo(const Info &L, const Info &R) {
    Info res;
    res.len = L.len + R.len;
    res.lval = (L.lval == L.len ? L.lval + R.lval : L.lval);
    res.rval = (R.rval == R.len ? R.rval + L.rval : R.rval);
    res.best = max({L.best, R.best, L.rval + R.lval});
    return res;
}
 
// persistent update: 이전 버전 prev의 트리에서 pos 위치의 값을 1로 갱신하고 새 노드(경로)를 생성합니다.
int update(int prev, int l, int r, int pos) {
    int idx = nodeCount++;
    segTree[idx] = segTree[prev]; // prev 노드 복사
    if(l == r) {
        segTree[idx].lval = 1;
        segTree[idx].rval = 1;
        segTree[idx].best = 1;
    } else {
        int mid = (l + r) / 2;
        if(pos <= mid) {
            int newLeft = update(segTree[prev].left, l, mid, pos);
            segTree[idx].left = newLeft;
        } else {
            int newRight = update(segTree[prev].right, mid+1, r, pos);
            segTree[idx].right = newRight;
        }
        int leftIdx = segTree[idx].left, rightIdx = segTree[idx].right;
        int leftLen = segTree[leftIdx].len, rightLen = segTree[rightIdx].len;
 
        int leftL = segTree[leftIdx].lval, rightL = segTree[rightIdx].lval;
        int leftR = segTree[leftIdx].rval, rightR = segTree[rightIdx].rval;
        int leftBest = segTree[leftIdx].best, rightBest = segTree[rightIdx].best;
 
        segTree[idx].lval = (leftL == leftLen ? leftL + rightL : leftL);
        segTree[idx].rval = (rightR == rightLen ? rightR + leftR : rightR);
        segTree[idx].best = max({leftBest, rightBest, leftR + rightL});
    }
    return idx;
}
 
// persistent 세그먼트 트리의 root 버전에서, 구간 [ql,qr]에 대해 정보를 쿼리합니다.
Info query(int root, int l, int r, int ql, int qr) {
    if(ql > r || qr < l) {
        return {0,0,0,0};
    }
    if(ql <= l && r <= qr) {
        return {segTree[root].len, segTree[root].lval, segTree[root].rval, segTree[root].best};
    }
    int mid = (l + r) / 2;
    Info leftInfo = query(segTree[root].left, l, mid, ql, qr);
    Info rightInfo = query(segTree[root].right, mid+1, r, ql, qr);
    if(leftInfo.len == 0) return rightInfo;
    if(rightInfo.len == 0) return leftInfo;
    return combineInfo(leftInfo, rightInfo);
}
 
// --- 메인 ---
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<int> h(N+1);
    for (int i = 1; i <= N; i++){
        cin >> h[i];
    }
 
    int Q;
    cin >> Q;
    struct Query { int l, r, w, idx; };
    vector<Query> queries(Q);
    for (int i = 0; i < Q; i++){
        int l, r, w;
        cin >> l >> r >> w;
        queries[i] = {l, r, w, i};
    }
 
    // persistent 세그먼트 트리의 각 버전은 “h[i] >= X”인 인덱스를 1로 표시한 상태입니다.
    // 먼저, 각 높이에 대해 해당 인덱스들을 모읍니다.
    unordered_map<int, vector<int>> positions;
    for (int i = 1; i <= N; i++){
        positions[h[i]].push_back(i);
    }
    // 고유 높이들을 내림차순으로 정렬합니다.
    vector<int> uniq;
    for (auto &p: positions){
        uniq.push_back(p.first);
    }
    sort(uniq.begin(), uniq.end(), greater<int>());
    int m = uniq.size();
 
    // persistent 세그먼트 트리용 메모리 미리 할당
    segTree.resize(MAX_NODES);
    nodeCount = 0;
    int base = build(1, N); // 초기 트리: 모두 0
    // 각 고유 높이에 대해, 그 높이 이상의 막대들을 업데이트한 결과를 하나의 버전으로 저장합니다.
    vector<int> version(m);
    for (int val: uniq) {
        sort(positions[val].begin(), positions[val].end());
    }
    int curVersion = base;
    for (int i = 0; i < m; i++){
        int val = uniq[i];
        for (int pos: positions[val]) {
            curVersion = update(curVersion, 1, N, pos);
        }
        version[i] = curVersion;
    }
 
    // [l, r] 구간 내에서 연속된 1의 최대 길이가 w 이상이면, 해당 버전(즉, 임계값 X)이 후보가 됩니다.
    // uniq는 내림차순이므로, 인덱스가 작을수록 더 큰 높이를 의미합니다.
    // 각 쿼리에 대해 persistent 세그먼트 트리의 해당 버전을 query하여 조건을 만족하는지 확인하고,
    // 이분 탐색을 통해 최대 높이를 찾습니다.
    vector<int> ans(Q, 0);
    for (int i = 0; i < Q; i++){
        int l = queries[i].l, r = queries[i].r, w = queries[i].w;
        int lo = 0, hi = m; // hi==m이면 조건을 만족하는 후보 없음 (문제조건상 w<=구간길이이므로 최소 1은 답임)
        while(lo < hi){
            int mid = (lo + hi) / 2;
            Info res = query(version[mid], 1, N, l, r);
            if(res.best >= w)
                hi = mid;
            else
                lo = mid + 1;
        }
        int resVal = (lo < m ? uniq[lo] : 0);
        ans[queries[i].idx] = resVal;
    }
 
    for (int i = 0; i < Q; i++){
        cout << ans[i] << "\n";
    }
 
    return 0;
}
