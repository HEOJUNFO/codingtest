#include <bits/stdc++.h>
using namespace std;
 
// 세그먼트 트리: 각 노드는 구간 내 “남아있는(파괴되지 않은) 빌딩의” 최대 높이와 그 인덱스를 저장
struct SegTree {
    int n;
    vector<pair<int,int>> tree; // (value, index)
 
    SegTree(int n) : n(n) {
        tree.resize(4*n, {0, -1});
    }
 
    // arr 배열을 바탕으로 세그먼트 트리 구성 (0-indexed)
    void build(vector<int> &arr, int idx, int l, int r) {
        if(l == r){
            tree[idx] = {arr[l], l};
            return;
        }
        int mid = (l + r) / 2;
        build(arr, idx*2, l, mid);
        build(arr, idx*2+1, mid+1, r);
        if(tree[idx*2].first >= tree[idx*2+1].first)
            tree[idx] = tree[idx*2];
        else
            tree[idx] = tree[idx*2+1];
    }
 
    // pos번 위치를 newVal(파괴시 0으로 처리)로 업데이트
    void update(int idx, int l, int r, int pos, int newVal) {
        if(l == r){
            tree[idx] = {newVal, pos};
            return;
        }
        int mid = (l + r) / 2;
        if(pos <= mid) update(idx*2, l, mid, pos, newVal);
        else update(idx*2+1, mid+1, r, pos, newVal);
        if(tree[idx*2].first >= tree[idx*2+1].first)
            tree[idx] = tree[idx*2];
        else
            tree[idx] = tree[idx*2+1];
    }
 
    // 구간 [ql, qr]에서 (최대값, 그 인덱스) 반환
    pair<int,int> query(int idx, int l, int r, int ql, int qr) {
        if(ql > r || qr < l) return {0, -1};
        if(ql <= l && r <= qr) return tree[idx];
        int mid = (l + r) / 2;
        auto leftAns = query(idx*2, l, mid, ql, qr);
        auto rightAns = query(idx*2+1, mid+1, r, ql, qr);
        if(leftAns.first >= rightAns.first)
            return leftAns;
        else
            return rightAns;
    }
 
    // 현재 남아있는(값 > 0인) 빌딩들 중, 가장 왼쪽 인덱스를 찾는다.
    int queryFirstAlive(int idx, int l, int r) {
        if(tree[idx].first == 0) return -1; // 이 구간에는 살아있는 빌딩이 없음.
        if(l == r) return l;
        int mid = (l + r) / 2;
        if(tree[idx*2].first > 0) return queryFirstAlive(idx*2, l, mid);
        else return queryFirstAlive(idx*2+1, mid+1, r);
    }
};
 
// 메인 – 게임 시뮬레이션
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<int> A(N);
    for(int i = 0; i < N; i++){
        cin >> A[i];
    }
 
    // 세그먼트 트리 초기화: 각 빌딩은 파괴되기 전 원래 높이를 유지
    SegTree seg(N);
    seg.build(A, 1, 0, N-1);
 
    // 각 라운드에 두 플레이어가 고른 발사 높이를 저장
    vector<pair<int,int>> rounds;
 
    // 남아있는 빌딩이 없을 때까지 시뮬레이션 진행
    while(true){
        int i = seg.queryFirstAlive(1, 0, N-1);
        if(i == -1) break; // 모두 파괴됨.
 
        // 플레이어1은 왼쪽 가장 앞의 빌딩 높이로 발사
        int h = A[i];
        pair<int,int> candidate = {0, -1};
        // i 다음 인덱스부터 남아있는 빌딩 중 최대 높이를 구함.
        if(i+1 < N){
            candidate = seg.query(1, 0, N-1, i+1, N-1);
        }
 
        // 만약 candidate의 높이가 h보다 크다면 pair로 처리
        if(candidate.first > h){
            rounds.push_back({h, candidate.first});
            // 두 빌딩 모두 파괴(세그먼트 트리에서는 해당 위치를 0으로 업데이트)
            seg.update(1, 0, N-1, i, 0);
            seg.update(1, 0, N-1, candidate.second, 0);
        } else {
            // 유효한 pairing 후보가 없으면 한 건물만 제거(두 플레이어 모두 같은 높이로 발사)
            rounds.push_back({h, h});
            seg.update(1, 0, N-1, i, 0);
        }
    }
 
    cout << rounds.size() << "\n";
    for(auto &p : rounds){
        cout << p.first << " " << p.second << "\n";
    }
 
    return 0;
}
