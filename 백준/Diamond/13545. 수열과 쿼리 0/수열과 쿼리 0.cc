#include <bits/stdc++.h>
using namespace std;
 
// --- 세그먼트 트리 (point update, 전체 구간의 최대값 query) ---
struct SegTree {
    int size;
    vector<int> tree;
    
    SegTree(int n) {
        size = 1;
        while(size < n) size *= 2;
        tree.assign(2*size, 0);
    }
    
    // pos 위치의 값을 val 로 갱신 (point update)
    void update(int pos, int val) {
        pos += size;
        tree[pos] = val;
        for(pos /= 2; pos >= 1; pos /= 2){
            tree[pos] = max(tree[2*pos], tree[2*pos+1]);
        }
    }
    
    // 전체 구간 [0, size-1]의 최대값 리턴
    int queryAll(){
        return tree[1];
    }
};
 
// --- 전역 변수들 ---
// 원래 배열의 크기 N, 누적합 배열의 길이는 M = N+1.
int N, M;
int offset; // 누적합값에 offset = N 을 더하여 음수가 없도록 함.
vector<int> prefixArr; // 누적합 P[0..M-1]
vector<int> Aarr;      // Aarr[i] = prefixArr[i] + offset (값의 범위는 [0, 2*N])
 
// Mo's algorithm에서는 prefix 배열의 인덱스 [0, M-1]에 대해 쿼리를 처리합니다.
//
// 각 누적합값(shifted value) v (0<= v < 2*N+1)에 대해, 현재 구간에 등장하는 인덱스들을
// 오름차순으로 유지하기 위해 deque<int>를 사용합니다.
int maxValRange; // = 2*N + 1
vector< deque<int> > deqs; 
 
// 전역 세그먼트 트리 (각 v에 대해 현재 deqs[v]가 보유하는 “gap = (마지막 인덱스 - 첫 인덱스)” 값을 관리)
SegTree* seg = nullptr;
 
// deqs[v]의 크기에 따라 후보값을 갱신: 만약 두 개 이상의 인덱스가 있으면 gap를, 아니면 0.
void updateCandidateForValue(int v) {
    int cand = 0;
    if(deqs[v].size() >= 2){
        cand = deqs[v].back() - deqs[v].front();
    } else {
        cand = 0;
    }
    seg->update(v, cand);
}
 
// Mo's algorithm 에서 현재 구간에 인덱스 idx (prefix 배열 상의 인덱스)를 추가한다.
void addIndex(int idx) {
    int v = Aarr[idx];
    if(deqs[v].empty()){
        deqs[v].push_back(idx);
    } else {
        // Mo's 알고리즘에서는 구간의 좌측에 추가할 때 idx는 현재 구간보다 작고,
        // 우측에 추가할 때는 현재 구간보다 큽니다.
        if(idx < deqs[v].front()){
            deqs[v].push_front(idx);
        } else if(idx > deqs[v].back()){
            deqs[v].push_back(idx);
        } else {
            // (이 경우는 거의 발생하지 않으므로 안전하게 삽입)
            auto it = deqs[v].begin();
            while(it != deqs[v].end() && *it < idx) it++;
            deqs[v].insert(it, idx);
        }
    }
    updateCandidateForValue(v);
}
 
// Mo's algorithm 에서 현재 구간에서 인덱스 idx 를 제거한다.
void removeIndex(int idx) {
    int v = Aarr[idx];
    if(!deqs[v].empty()){
        if(deqs[v].front() == idx){
            deqs[v].pop_front();
        } else if(deqs[v].back() == idx){
            deqs[v].pop_back();
        } else {
            // 혹시 중간에 있다면 선형탐색으로 제거
            for(auto it = deqs[v].begin(); it != deqs[v].end(); ++it){
                if(*it == idx){
                    deqs[v].erase(it);
                    break;
                }
            }
        }
    }
    updateCandidateForValue(v);
}
 
// --- Mo's algorithm 쿼리 구조체 ---
struct Query {
    int L, R, idx;
};
 
// Mo's 알고리즘에서 사용할 block size (대략 sqrt(M))
int BLOCK_SIZE;
 
bool cmpQueries(const Query &a, const Query &b) {
    int ablock = a.L / BLOCK_SIZE;
    int bblock = b.L / BLOCK_SIZE;
    if(ablock != bblock)
        return ablock < bblock;
    return a.R < b.R;
}
 
// --- Main ---
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> N;
    vector<int> arr(N);
    for (int i = 0; i < N; i++){
        cin >> arr[i];
    }
 
    // 누적합 배열 P (길이 M = N+1) 계산
    M = N + 1;
    prefixArr.resize(M);
    prefixArr[0] = 0;
    for (int i = 1; i < M; i++){
        prefixArr[i] = prefixArr[i-1] + arr[i-1]; // arr[i-1] 가 A_i
    }
 
    // offset = N (음수없게 만들기)
    offset = N;
    Aarr.resize(M);
    for (int i = 0; i < M; i++){
        Aarr[i] = prefixArr[i] + offset; // Aarr[i] ∈ [0, 2*N]
    }
 
    maxValRange = 2 * N + 1;
    deqs.resize(maxValRange);
 
    // 세그먼트 트리 초기화
    seg = new SegTree(maxValRange);
    for (int v = 0; v < maxValRange; v++){
        seg->update(v, 0);
    }
 
    // 쿼리 입력
    int Q;
    cin >> Q;
    vector<Query> queries(Q);
    for (int i = 0; i < Q; i++){
        int i_orig, j_orig;
        cin >> i_orig >> j_orig;
        // 원래 쿼리는 [i_orig, j_orig] (원소 1-indexed)
        // 누적합 배열에서는 구간 [i_orig-1, j_orig] 에 해당합니다.
        int L = i_orig - 1;
        int R = j_orig;
        queries[i] = {L, R, i};
    }
 
    BLOCK_SIZE = max(1, (int) sqrt(M));
    sort(queries.begin(), queries.end(), cmpQueries);
 
    int curL = 0, curR = -1; // 현재 구간 [curL, curR] (초기에는 빈 구간)
    vector<int> ans(Q, 0);
 
    // Mo's algorithm 으로 쿼리 처리
    for (int qi = 0; qi < Q; qi++){
        int L = queries[qi].L, R = queries[qi].R;
        while(curR < R) {
            curR++;
            addIndex(curR);
        }
        while(curR > R) {
            removeIndex(curR);
            curR--;
        }
        while(curL < L) {
            removeIndex(curL);
            curL++;
        }
        while(curL > L) {
            curL--;
            addIndex(curL);
        }
 
        // 현재 구간은 prefix 배열의 [L, R]입니다.
        // 전역 세그먼트 트리에서 모든 v에 대해 최대 candidate (즉, 최대 gap)를 구하면 이것이 쿼리의 정답입니다.
        int curCandidate = seg->queryAll();
        ans[queries[qi].idx] = curCandidate;
    }
 
    for (int i = 0; i < Q; i++){
        cout << ans[i] << "\n";
    }
 
    delete seg;
    return 0;
}
