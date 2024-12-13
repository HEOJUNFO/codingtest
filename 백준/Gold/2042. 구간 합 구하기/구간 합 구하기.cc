#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int N, M, K;
vector<ll> arr;
vector<ll> tree; // 세그먼트 트리

// 세그먼트 트리 초기화
// node: 현재 노드 번호, start: 시작 구간, end: 끝 구간
ll init(int node, int start, int end) {
    if (start == end) {
        return tree[node] = arr[start];
    }
    int mid = (start + end) / 2;
    return tree[node] = init(node*2, start, mid) + init(node*2+1, mid+1, end);
}

// 단일 원소 업데이트
// idx: 수정할 인덱스, diff: 변경값 - 기존값
void update(int node, int start, int end, int idx, ll diff) {
    if (idx < start || idx > end) return; // 범위를 벗어남
    tree[node] += diff;
    if (start != end) {
        int mid = (start + end) / 2;
        update(node*2, start, mid, idx, diff);
        update(node*2+1, mid+1, end, idx, diff);
    }
}

// 구간 합 쿼리
// left, right: 구해야 하는 구간
ll query(int node, int start, int end, int left, int right) {
    if (right < start || left > end) return 0; // 범위를 벗어남
    if (left <= start && end <= right) return tree[node];
    int mid = (start + end) / 2;
    return query(node*2, start, mid, left, right) + query(node*2+1, mid+1, end, left, right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> K;
    arr.resize(N+1);
    for (int i = 1; i <= N; i++) {
        cin >> arr[i];
    }

    // 트리 크기 설정: 일반적으로 4*N 정도로 잡는다.
    tree.resize(4*N);
    init(1, 1, N);

    int total = M+K;
    for (int i = 0; i < total; i++) {
        ll a, b, c; 
        cin >> a >> b >> c;
        if (a == 1) {
            // b번째 수를 c로 바꾸기
            ll diff = c - arr[b];
            arr[b] = c;
            update(1, 1, N, b, diff);
        } else if (a == 2) {
            // b부터 c까지 합을 출력
            cout << query(1, 1, N, b, c) << "\n";
        }
    }

    return 0;
}
