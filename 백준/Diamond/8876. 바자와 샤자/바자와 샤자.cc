#include <bits/stdc++.h>
using namespace std;
 
// 직접 구현한 gcd (0에 대해서도 올바르게 동작)
long long myGcd(long long a, long long b) {
    while(b){
        a %= b;
        swap(a, b);
    }
    return a;
}
 
// 업데이트되는 셀을 나타내는 Point 구조체
struct Point {
    int r, c;
    long long val;
};
 
// 전역 벡터 – 모든 (업데이트된) 점들을 저장
vector<Point> points;
// (r,c) 좌표를 key로 하여 points 배열 내의 인덱스를 저장
unordered_map<long long, int> pointIndex;
 
// (r,c)를 하나의 64비트 정수 key로 만드는 함수 
inline long long makeKey(int r, int c) {
    return (((long long) r) << 32) | (unsigned int)c;
}
 
// 2차원 세그먼트 트리의 외부 노드 구조체
struct Node {
    // 이 노드가 담당하는 점들의 (열, global_index) 쌍 – 열 기준 정렬됨.
    vector<pair<int,int>> data;
    // 내부 세그먼트 트리 (내부 배열의 각 노드는 해당 구간의 gcd)
    vector<long long> seg;
    int L, R; // 이 외부 노드가 points 배열의 인덱스 [L, R]를 커버함.
};
 
// 외부 세그먼트 트리 – 크기는 최대 (4 * points.size())
vector<Node> segTree;
 
// --- 내부 세그먼트 트리 관련 함수 ---
// nodeIdx : 외부 트리의 노드 번호, idx : 내부 세그먼트 트리 인덱스, [l, r]는 내부 배열 인덱스 구간.
// base: 해당 노드에 해당하는 점들의 현재 값을 담은 배열.
void buildInner(int nodeIdx, int idx, int l, int r, const vector<long long> &base) {
    if(l == r) {
        segTree[nodeIdx].seg[idx] = base[l];
        return;
    }
    int mid = (l+r)/2;
    buildInner(nodeIdx, idx*2, l, mid, base);
    buildInner(nodeIdx, idx*2+1, mid+1, r, base);
    segTree[nodeIdx].seg[idx] = myGcd(segTree[nodeIdx].seg[idx*2], segTree[nodeIdx].seg[idx*2+1]);
}
 
// 내부 세그먼트 트리에서 pos 위치의 값을 newVal로 갱신
void updateInner(int nodeIdx, int idx, int l, int r, int pos, long long newVal) {
    if(l == r) {
        segTree[nodeIdx].seg[idx] = newVal;
        return;
    }
    int mid = (l+r)/2;
    if(pos <= mid)
        updateInner(nodeIdx, idx*2, l, mid, pos, newVal);
    else
        updateInner(nodeIdx, idx*2+1, mid+1, r, pos, newVal);
    segTree[nodeIdx].seg[idx] = myGcd(segTree[nodeIdx].seg[idx*2], segTree[nodeIdx].seg[idx*2+1]);
}
 
// 내부 세그먼트 트리 질의 – [ql, qr] 구간의 gcd 리턴
long long queryInner(int nodeIdx, int idx, int l, int r, int ql, int qr) {
    if(ql > r || qr < l) return 0;
    if(ql <= l && r <= qr) return segTree[nodeIdx].seg[idx];
    int mid = (l+r)/2;
    long long leftRes = queryInner(nodeIdx, idx*2, l, mid, ql, qr);
    long long rightRes = queryInner(nodeIdx, idx*2+1, mid+1, r, ql, qr);
    return myGcd(leftRes, rightRes);
}
 
// --- 외부 세그먼트 트리 관련 함수 ---
// idx: 외부 세그먼트 트리의 인덱스, [L,R]는 points 배열 상의 구간.
void buildOuter(int idx, int L, int R) {
    segTree[idx].L = L;
    segTree[idx].R = R;
    if(L == R) {
        segTree[idx].data.push_back({ points[L].c, L });
        segTree[idx].seg.resize(4); // 단일 원소에 대한 내부 트리
        vector<long long> base(1);
        base[0] = points[L].val;
        buildInner(idx, 1, 0, 0, base);
        return;
    }
    int mid = (L+R)/2;
    buildOuter(idx*2, L, mid);
    buildOuter(idx*2+1, mid+1, R);
    auto &leftData = segTree[idx*2].data;
    auto &rightData = segTree[idx*2+1].data;
    auto &curData = segTree[idx].data;
    curData.resize(leftData.size() + rightData.size());
    merge(leftData.begin(), leftData.end(),
          rightData.begin(), rightData.end(),
          curData.begin(), [](const pair<int,int>& a, const pair<int,int>& b) {
              return a.first < b.first;
          });
    int n = curData.size();
    segTree[idx].seg.resize(4 * n);
    vector<long long> base(n);
    for (int i = 0; i < n; i++){
        base[i] = points[ curData[i].second ].val;
    }
    buildInner(idx, 1, 0, n-1, base);
}
 
// 외부 세그먼트 트리 업데이트 – points 배열에서 인덱스 pos (즉, (r,c) 좌표)에 대한 값 변경 newVal
void updateOuter(int idx, int L, int R, int pos, long long newVal) {
    auto &vec = segTree[idx].data;
    int colVal = points[pos].c;
    int posInVec = -1;
    auto it = std::lower_bound(vec.begin(), vec.end(), make_pair(colVal, -1), [](const pair<int,int>& a, const pair<int,int>& b){
        return a.first < b.first;
    });
    for (int i = it - vec.begin(); i < (int)vec.size() && vec[i].first == colVal; i++){
        if(vec[i].second == pos) { posInVec = i; break; }
    }
    if(posInVec != -1) {
        updateInner(idx, 1, 0, vec.size()-1, posInVec, newVal);
    }
    if(L == R) return;
    int mid = (L+R)/2;
    if(pos <= mid)
        updateOuter(idx*2, L, mid, pos, newVal);
    else
        updateOuter(idx*2+1, mid+1, R, pos, newVal);
}
 
// 외부 세그먼트 트리 질의 – points 배열의 인덱스 [ql,qr] (즉, 행 구간에 해당) 중
// 내부에서 열이 [colL, colR]인 점들의 gcd를 구함.
long long queryOuter(int idx, int L, int R, int ql, int qr, int colL, int colR) {
    if(qr < L || ql > R) return 0;
    if(ql <= L && R <= qr) {
        auto &vec = segTree[idx].data;
        int lidx = (int)(std::lower_bound(vec.begin(), vec.end(), make_pair(colL, -1), [](const pair<int,int>& a, const pair<int,int>& b){
            return a.first < b.first;
        }) - vec.begin());
        int ridx = (int)(std::upper_bound(vec.begin(), vec.end(), make_pair(colR, INT_MAX), [](const pair<int,int>& a, const pair<int,int>& b){
            return a.first < b.first;
        }) - vec.begin()) - 1;
        if(lidx >= (int)vec.size() || ridx < 0 || lidx > ridx) return 0;
        return queryInner(idx, 1, 0, vec.size()-1, lidx, ridx);
    }
    int mid = (L+R)/2;
    long long leftRes = queryOuter(idx*2, L, mid, ql, qr, colL, colR);
    long long rightRes = queryOuter(idx*2+1, mid+1, R, ql, qr, colL, colR);
    return myGcd(leftRes, rightRes);
}
 
// points 배열은 행 기준 정렬 (행 같으면 열 기준) 해야 함.
bool cmpPoints(const Point &a, const Point &b) {
    if(a.r == b.r) return a.c < b.c;
    return a.r < b.r;
}
 
// main – 입력 처리 및 연산 수행
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int R, C, N;
    cin >> R >> C >> N;
    // 연산들을 저장합니다.
    // type 1: "1 P Q K" (업데이트)
    // type 2: "2 P Q U V" (질의)
    struct Op {
        int type;
        int P, Q, U, V;
        long long K; // 업데이트일 경우 새 값
    };
    vector<Op> ops;
    ops.reserve(N);
 
    // 업데이트 좌표들을 모으기
    vector<pair<int,int>> coords;
 
    for (int i=0; i<N; i++){
        int t;
        cin >> t;
        Op op;
        op.type = t;
        if(t==1){
            cin >> op.P >> op.Q >> op.K;
            ops.push_back(op);
            coords.push_back({op.P, op.Q});
        } else {
            cin >> op.P >> op.Q >> op.U >> op.V;
            ops.push_back(op);
        }
    }
 
    // 중복 제거 후 좌표 압축
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
 
    points.resize(coords.size());
    for (int i=0; i< (int)coords.size(); i++){
        points[i].r = coords[i].first;
        points[i].c = coords[i].second;
        points[i].val = 0;
        long long key = makeKey(points[i].r, points[i].c);
        pointIndex[key] = i;
    }
 
    // 2차원 세그먼리 트리 구축을 위해 points 배열을 행 기준 정렬 (행 같으면 열 순)
    sort(points.begin(), points.end(), cmpPoints);
    int M = points.size();
    if(M == 0){
        // 업데이트가 없으면 모든 질의에 0 출력
        for(auto &op: ops){
            if(op.type == 2){
                cout << 0 << "\n";
            }
        }
        return 0;
    }
 
    // 외부 세그먼트 트리 배열 할당 (인덱스 1부터 사용)
    segTree.resize(4 * M);
    buildOuter(1, 0, M-1);
 
    // points 배열는 행 기준 정렬되어 있으므로,
    // 질의 시 행 범위 [P, U]에 해당하는 global 인덱스 범위를 이분탐색으로 구한다.
    auto rowLower = [&](int target) -> int {
        int lo = 0, hi = M;
        while(lo < hi){
            int mid = (lo+hi)/2;
            if(points[mid].r < target) lo = mid+1;
            else hi = mid;
        }
        return lo;
    };
    auto rowUpper = [&](int target) -> int {
        int lo = 0, hi = M;
        while(lo < hi){
            int mid = (lo+hi)/2;
            if(points[mid].r <= target) lo = mid+1;
            else hi = mid;
        }
        return lo;
    };
 
    // 주의: 처음 pointIndex는 압축 순서(입력 순서) 기반이었으나,
    // points 배열을 정렬하였으므로 새 mapping을 만듭니다.
    unordered_map<long long,int> sortedPointIndex;
    for (int i = 0; i < M; i++){
        long long key = makeKey(points[i].r, points[i].c);
        sortedPointIndex[key] = i;
    }
 
    // 연산 처리
    for(auto &op: ops){
        if(op.type == 1){
            // 업데이트: 셀 (P,Q)를 op.K로 설정
            long long key = makeKey(op.P, op.Q);
            int pos = sortedPointIndex[key];
            points[pos].val = op.K;
            updateOuter(1, 0, M-1, pos, op.K);
        } else {
            // 질의: 직사각형 [(P,Q), (U,V)] 내의 gcd 계산
            int lower = rowLower(op.P);
            int upper = rowUpper(op.U) - 1;
            if(lower > upper) {
                cout << 0 << "\n";
            } else {
                long long ans = queryOuter(1, 0, M-1, lower, upper, op.Q, op.V);
                cout << ans << "\n";
            }
        }
    }
 
    return 0;
}
