#include <bits/stdc++.h>
using namespace std;
 
// 음의 무한대를 나타내기 위해 (LLONG_MAX가 정의되어 있으므로)
const long long MINF = -LLONG_MAX;
 
// 세그먼트 트리의 노드 구조체
struct Node {
    int L, R;   // 구간 [L, R]
    int cnt;    // 해당 구간에 남아있는 식물의 수
    long long mx; // 해당 구간에 있는 식물들의 v 값 중 최대값
};
 
// 전역 세그먼트 트리 (정원 수 최대 1e5 → 4*N 정도의 크기)
vector<Node> seg;
 
// 각 정원(1~N)에 대해, 심어진 식물의 v 값(= h - k*t)을 저장하는 multiset
vector<multiset<long long>> leafData;
 
// 세그먼트 트리 초기화 (재귀적으로 노드의 구간 정보와 값을 세팅)
void buildTree(int idx, int L, int R) {
    seg[idx].L = L; seg[idx].R = R;
    if(L == R){
        seg[idx].cnt = leafData[L].size();
        seg[idx].mx = (leafData[L].empty() ? MINF : *leafData[L].rbegin());
        return;
    }
    int mid = (L+R)/2;
    buildTree(idx*2, L, mid);
    buildTree(idx*2+1, mid+1, R);
    seg[idx].cnt = seg[idx*2].cnt + seg[idx*2+1].cnt;
    seg[idx].mx = max(seg[idx*2].mx, seg[idx*2+1].mx);
}
 
// 정원 pos의 multiset에 변화가 있을 때 leaf를 갱신하는 함수
void updateLeaf(int idx, int pos) {
    if(seg[idx].L == seg[idx].R){
        seg[idx].cnt = leafData[pos].size();
        seg[idx].mx = (leafData[pos].empty() ? MINF : *leafData[pos].rbegin());
        return;
    }
    int mid = (seg[idx].L + seg[idx].R) / 2;
    if(pos <= mid) updateLeaf(idx*2, pos);
    else updateLeaf(idx*2+1, pos);
    seg[idx].cnt = seg[idx*2].cnt + seg[idx*2+1].cnt;
    seg[idx].mx = max(seg[idx*2].mx, seg[idx*2+1].mx);
}
 
// 구간 [ql, qr] 내에서, cutoff 보다 큰 v 값을 가진 식물들을 모두 삭제한다.
void removeRange(int idx, int ql, int qr, long long cutoff) {
    if(seg[idx].R < ql || seg[idx].L > qr) return; // 구간 밖이면 종료
    if(ql <= seg[idx].L && seg[idx].R <= qr) {
        if(seg[idx].mx <= cutoff) return; // 이 구간에 모두 cutoff 이하이므로 삭제할 식물 없음.
        if(seg[idx].L == seg[idx].R) {
            int pos = seg[idx].L;
            // multiset은 오름차순 정렬되므로 cutoff 보다 큰 원소는 뒤쪽에 몰려 있다.
            auto it = leafData[pos].lower_bound(cutoff+1);
            if(it != leafData[pos].end()){
                leafData[pos].erase(it, leafData[pos].end());
                seg[idx].cnt = leafData[pos].size();
                seg[idx].mx = (leafData[pos].empty() ? MINF : *leafData[pos].rbegin());
            }
            return;
        }
    }
    removeRange(idx*2, ql, qr, cutoff);
    removeRange(idx*2+1, ql, qr, cutoff);
    seg[idx].cnt = seg[idx*2].cnt + seg[idx*2+1].cnt;
    seg[idx].mx = max(seg[idx*2].mx, seg[idx*2+1].mx);
}
 
// 구간 [ql, qr]에 있는 식물의 총 개수를 반환
int queryRange(int idx, int ql, int qr) {
    if(seg[idx].R < ql || seg[idx].L > qr) return 0;
    if(ql <= seg[idx].L && seg[idx].R <= qr) return seg[idx].cnt;
    return queryRange(idx*2, ql, qr) + queryRange(idx*2+1, ql, qr);
}
 
// 메인 – 빠른 입출력을 사용
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    long long k;
    cin >> N >> M >> k;
    leafData.resize(N+1);
    seg.resize(4*(N+1));
    // 초기에 모든 정원은 비어있으므로 세그먼트 트리 빌드
    buildTree(1, 1, N);
 
    // 작업은 날짜가 증가하는 순서대로 주어진다.
    for (int i = 0; i < M; i++){
        int type;
        cin >> type;
        if(type == 1){
            // 1 t x h : t일에 정원 x에 높이 h인 식물을 추가
            long long t, h;
            int x;
            cin >> t >> x >> h;
            // 식물의 v = h - k*t
            long long v = h - k*t;
            leafData[x].insert(v);
            updateLeaf(1, x);
        } else if(type == 2){
            // 2 t l r h : t일에 정원 l~r의 식물 중, (v + k*t) > h 인 (즉, v > h - k*t) 식물을 삭제
            long long t, h;
            int l, r;
            cin >> t >> l >> r >> h;
            long long cutoff = h - k*t;
            removeRange(1, l, r, cutoff);
        } else if(type == 3){
            // 3 t l r : t일에 정원 l~r에 남은 식물의 개수를 출력
            long long t;
            int l, r;
            cin >> t >> l >> r;
            int ans = queryRange(1, l, r);
            cout << ans << "\n";
        }
    }
 
    return 0;
}
