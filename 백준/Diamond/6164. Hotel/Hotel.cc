#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Node {
    int pre, suf, maxi; // pre: 구간 시작에서 연속 빈 방 수, suf: 구간 끝에서 연속 빈 방 수, maxi: 구간 내 최대 연속 빈 방 수
    int lazy; // -1: 없음, 0: 점유, 1: 빈 방
    int len;
};

int N, M;
vector<Node> seg;

void pushUp(int idx) {
    int left = idx * 2, right = idx * 2 + 1;
    seg[idx].pre = seg[left].pre;
    if(seg[left].pre == seg[left].len)
        seg[idx].pre = seg[left].len + seg[right].pre;
    seg[idx].suf = seg[right].suf;
    if(seg[right].suf == seg[right].len)
        seg[idx].suf = seg[right].len + seg[left].suf;
    seg[idx].maxi = max({seg[left].maxi, seg[right].maxi, seg[left].suf + seg[right].pre});
}

void apply(int idx, int val) {
    seg[idx].lazy = val;
    if(val == 1) { // 빈 방으로 설정
        seg[idx].pre = seg[idx].suf = seg[idx].maxi = seg[idx].len;
    } else { // 점유로 설정
        seg[idx].pre = seg[idx].suf = seg[idx].maxi = 0;
    }
}

void pushDown(int idx) {
    if(seg[idx].lazy != -1) {
        int left = idx * 2, right = idx * 2 + 1;
        apply(left, seg[idx].lazy);
        apply(right, seg[idx].lazy);
        seg[idx].lazy = -1;
    }
}

void build(int idx, int l, int r) {
    seg[idx].len = r - l + 1;
    seg[idx].lazy = -1;
    if(l == r) {
        seg[idx].pre = seg[idx].suf = seg[idx].maxi = 1; // 처음엔 모두 빈 방
        return;
    }
    int mid = (l + r) / 2;
    build(idx * 2, l, mid);
    build(idx * 2 + 1, mid + 1, r);
    pushUp(idx);
}

// [L, R] 구간을 val (0: 점유, 1: 빈 방)으로 업데이트
void update(int idx, int l, int r, int L, int R, int val) {
    if(R < l || r < L) return;
    if(L <= l && r <= R) {
        apply(idx, val);
        return;
    }
    pushDown(idx);
    int mid = (l + r) / 2;
    update(idx * 2, l, mid, L, R, val);
    update(idx * 2 + 1, mid + 1, r, L, R, val);
    pushUp(idx);
}

// 구간 [l, r] 내에서 연속 빈 방이 d개 이상인 가장 왼쪽 시작 위치를 찾는 함수
int query(int idx, int l, int r, int d) {
    if(l == r) {
        if(seg[idx].maxi >= d) return l;
        else return -1;
    }
    pushDown(idx);
    int mid = (l + r) / 2;
    if(seg[idx * 2].maxi >= d)
        return query(idx * 2, l, mid, d);
    else if(seg[idx * 2].suf + seg[idx * 2 + 1].pre >= d)
        return mid - seg[idx * 2].suf + 1;
    else if(seg[idx * 2 + 1].maxi >= d)
        return query(idx * 2 + 1, mid + 1, r, d);
    return -1;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    seg.resize(4 * (N + 1));
    build(1, 1, N);
    
    while(M--){
        int type;
        cin >> type;
        if(type == 1){
            int d;
            cin >> d;
            int pos = query(1, 1, N, d);
            if(pos == -1){
                cout << 0 << "\n";
            } else {
                cout << pos << "\n";
                // pos부터 d개의 방을 점유 처리
                update(1, 1, N, pos, pos + d - 1, 0);
            }
        } else { // type == 2, 체크아웃
            int x, d;
            cin >> x >> d;
            update(1, 1, N, x, x + d - 1, 1);
        }
    }
    return 0;
}
