#include <bits/stdc++.h>
using namespace std;
 
// 질의 (query)를 저장할 구조체
struct Query {
    int l, r, idx;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, K;
    cin >> N >> K;
    vector<int> A(N+1);
    for (int i = 1; i <= N; i++){
        cin >> A[i];
    }
    
    int M;
    cin >> M;
    vector<Query> queries(M);
    for (int i = 0; i < M; i++){
        int l, r;
        cin >> l >> r;
        queries[i] = {l, r, i};
    }
    
    // Mo's 알고리즘을 위해 block size (대략 sqrt(N))로 나누고 정렬
    int blockSize = static_cast<int>(sqrt(N));
    sort(queries.begin(), queries.end(), [blockSize](const Query &a, const Query &b){
        int ablock = a.l / blockSize;
        int bblock = b.l / blockSize;
        if(ablock != bblock)
            return ablock < bblock;
        return a.r < b.r;
    });
 
    // --- 이터러티브 세그먼트 트리 (포인트 업데이트, 전체 구간 최대값 쿼리) ---
    int segSize = 1;
    while(segSize < K+1) segSize *= 2;
    vector<int> seg(2 * segSize, 0);
    auto seg_update = [&](int pos, int val) {
        pos += segSize;
        seg[pos] = val;
        for(pos /= 2; pos >= 1; pos /= 2)
            seg[pos] = max(seg[2*pos], seg[2*pos+1]);
    };
    auto seg_query = [&]() -> int {
        return seg[1];
    };
    // --------------------------------------------------------------------
 
    // 각 값 v (1 ≤ v ≤ K)에 대해 현재 구간에 등장하는 인덱스를 저장하는 deque
    vector<deque<int>> occ(K+1);
 
    // Mo's 알고리즘에서 현재 구간 [curL, curR] (초기엔 empty)
    int curL = 1, curR = 0;
    vector<int> ans(M, 0);
 
    // 구간에 index pos를 추가하는 함수
    auto add = [&](int pos) {
        int v = A[pos];
        // Mo's 알고리즘에서는 pos가 구간의 왼쪽 경계(L-1) 또는 오른쪽 경계(R+1)로 추가되므로,
        // 현재 occ[v]에 있는 값보다 작으면 front, 크면 back에 삽입하면 오름차순 유지가 가능합니다.
        if(occ[v].empty()){
            occ[v].push_back(pos);
        } else {
            if(pos < occ[v].front()){
                occ[v].push_front(pos);
            } else if(pos > occ[v].back()){
                occ[v].push_back(pos);
            } else {
                // 이 경우는 거의 발생하지 않지만 혹시 있을 경우
                auto it = lower_bound(occ[v].begin(), occ[v].end(), pos);
                occ[v].insert(it, pos);
            }
        }
        // occ[v]가 비어있지 않으면 gap = (가장 큰 index - 가장 작은 index)
        int new_gap = occ[v].empty() ? 0 : (occ[v].back() - occ[v].front());
        seg_update(v, new_gap);
    };
 
    // 구간에서 index pos를 제거하는 함수 (제거는 항상 구간의 양 끝에서 이루어집니다)
    auto remove = [&](int pos) {
        int v = A[pos];
        if(!occ[v].empty()){
            if(occ[v].front() == pos){
                occ[v].pop_front();
            } else if(occ[v].back() == pos){
                occ[v].pop_back();
            } else {
                // 이 경우는 없어야 함
                auto it = find(occ[v].begin(), occ[v].end(), pos);
                if(it != occ[v].end())
                    occ[v].erase(it);
            }
        }
        int new_gap = occ[v].empty() ? 0 : (occ[v].back() - occ[v].front());
        seg_update(v, new_gap);
    };
 
    // --- Mo's 알고리즘으로 쿼리 처리 ---
    for(auto &q : queries){
        int L = q.l, R = q.r;
        while(curR < R){
            curR++;
            add(curR);
        }
        while(curR > R){
            remove(curR);
            curR--;
        }
        while(curL < L){
            remove(curL);
            curL++;
        }
        while(curL > L){
            curL--;
            add(curL);
        }
        // 현재 세그먼트 트리의 전체 구간 최대값이 현재 구간 [L,R] 의 정답입니다.
        ans[q.idx] = seg_query();
    }
 
    for (int i = 0; i < M; i++){
        cout << ans[i] << "\n";
    }
 
    return 0;
}
