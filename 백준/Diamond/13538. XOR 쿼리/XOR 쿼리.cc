#include <iostream>
using namespace std;

const int MAX_VAL = 500000;  // 배열 원소의 최댓값
const int BITS = 20;         // x의 최대값이 500,000이므로 20비트면 충분
// persistent segment tree : 각 업데이트마다 약 22개 노드 생성 (500,000 * 22)
const int MAX_SEG_NODES = 500000 * 22 + 5;
// persistent trie : 최대 22개의 노드 per update
const int MAX_TRIE_NODES = 500000 * 22 + 5;

// --- Persistent Segment Tree ---
// 각 노드는 [l, r] 구간에 대해 구간내 원소의 개수를 저장
static int segLeft[MAX_SEG_NODES], segRight[MAX_SEG_NODES];
static int segCount[MAX_SEG_NODES];
int segNodeCount = 1; // 0번 노드는 null(빈) 노드로 사용

// seg_update : 이전 버전(prev)에서 pos 위치에 1을 더해 새로운 버전의 노드를 생성 (구간: [l, r])
int seg_update(int prev, int pos, int l, int r) {
    int cur = segNodeCount++;
    segCount[cur] = segCount[prev] + 1;
    if(l == r) return cur;
    int mid = (l + r) >> 1;
    if(pos <= mid) {
        segLeft[cur] = seg_update(segLeft[prev], pos, l, mid);
        segRight[cur] = segRight[prev];
    } else {
        segLeft[cur] = segLeft[prev];
        segRight[cur] = seg_update(segRight[prev], pos, mid+1, r);
    }
    return cur;
}
 
// seg_query : persistent tree node에서 [l,r] 구간 중 값이 <= x 인 원소의 개수 반환
int seg_query(int node, int l, int r, int x) {
    if(node == 0) return 0;
    if(r <= x) return segCount[node];
    int mid = (l + r) >> 1;
    if(x <= mid)
        return seg_query(segLeft[node], l, mid, x);
    else
        return segCount[segLeft[node]] + seg_query(segRight[node], mid+1, r, x);
}
 
// seg_kth : persistent segment tree 차분(구간 [L,R])을 이용하여 k번째 작은 원소를 구함
int seg_kth(int nodeR, int nodeL, int l, int r, int k) {
    if(l == r) return l;
    int mid = (l + r) >> 1;
    int countLeft = segCount[ segLeft[nodeR] ] - segCount[ segLeft[nodeL] ];
    if(k <= countLeft)
        return seg_kth(segLeft[nodeR], segLeft[nodeL], l, mid, k);
    else
        return seg_kth(segRight[nodeR], segRight[nodeL], mid+1, r, k - countLeft);
}
 
// --- Persistent Trie ---
// 각 노드는 0/1 자식 포인터와 해당 노드를 거치는 원소의 개수를 저장
static int trieLeft[MAX_TRIE_NODES], trieRight[MAX_TRIE_NODES];
static int trieCount[MAX_TRIE_NODES];
int trieNodeCount = 1; // 0번 노드를 초기 빈 노드로 사용
 
// trie_update : 이전 버전(prev)를 복사하면서 x 값을 새로 삽입 (bit: 현재 비트 위치)
int trie_update(int prev, int x, int bit) {
    int cur = trieNodeCount++;
    trieCount[cur] = trieCount[prev] + 1;
    trieLeft[cur] = trieLeft[prev];
    trieRight[cur] = trieRight[prev];
    if(bit < 0) return cur;
    int b = (x >> bit) & 1;
    if(b == 0)
        trieLeft[cur] = trie_update(trieLeft[prev], x, bit - 1);
    else
        trieRight[cur] = trie_update(trieRight[prev], x, bit - 1);
    return cur;
}
 
// trie_query : persistent trie의 차분(버전 nodeR - nodeL)을 이용하여, 
// subarray 내에서 x와 XOR 했을 때 최대가 되는 원소 y를 찾아 반환
int trie_query(int nodeR, int nodeL, int x, int bit) {
    if(bit < 0) return 0;
    int b = (x >> bit) & 1;
    if(b == 0) {
        // x의 bit가 0이면, y에서 해당 비트를 1로 선택하면 (0^1 = 1) 도움이 됨.
        int cnt = trieCount[ trieRight[nodeR] ] - trieCount[ trieRight[nodeL] ];
        if(cnt > 0)
            return (1 << bit) | trie_query(trieRight[nodeR], trieRight[nodeL], x, bit - 1);
        else
            return trie_query(trieLeft[nodeR], trieLeft[nodeL], x, bit - 1);
    } else {
        // x의 bit가 1이면, y에서 해당 비트를 0으로 선택하면 (1^0 = 1) 도움이 됨.
        int cnt = trieCount[ trieLeft[nodeR] ] - trieCount[ trieLeft[nodeL] ];
        if(cnt > 0)
            return trie_query(trieLeft[nodeR], trieLeft[nodeL], x, bit - 1);
        else
            return (1 << bit) | trie_query(trieRight[nodeR], trieRight[nodeL], x, bit - 1);
    }
}
 
// --- Main ---
 
// 최대 버전 번호는 배열에 들어가는 원소 수와 동일하므로, persistent root 배열 크기는 500,000+5 정도로 잡음.
const int MAX_VERSION = 500000 + 5;
static int segRoot[MAX_VERSION];   // segRoot[i] : i개의 원소를 가진 배열의 persistent segment tree 루트
static int trieRoot[MAX_VERSION];  // trieRoot[i] : i개의 원소를 가진 배열의 persistent trie 루트
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int M;
    cin >> M;
    
    // 초기 버전 (빈 배열)
    segRoot[0] = 0;
    trieRoot[0] = 0;
    int currentSize = 0; // 현재 배열 A의 크기
 
    // 각 쿼리를 처리
    for(int i = 0; i < M; i++){
        int type;
        cin >> type;
        if(type == 1){
            // "1 x": 배열 A의 끝에 x 추가
            int x; 
            cin >> x;
            currentSize++;
            segRoot[currentSize] = seg_update(segRoot[currentSize-1], x, 1, MAX_VAL);
            trieRoot[currentSize] = trie_update(trieRoot[currentSize-1], x, BITS - 1);
        }
        else if(type == 2){
            // "2 L R x": A[L]부터 A[R] 중에서 x와 XOR 했을 때 최대가 되는 y 출력
            int L, R, x;
            cin >> L >> R >> x;
            int res = trie_query(trieRoot[R], trieRoot[L-1], x, BITS - 1);
            cout << res << "\n";
        }
        else if(type == 3){
            // "3 k": 배열 A의 마지막 k개 제거
            int k; 
            cin >> k;
            currentSize -= k;
        }
        else if(type == 4){
            // "4 L R x": A[L]부터 A[R] 중에서 x보다 작거나 같은 원소의 개수 출력
            int L, R, x;
            cin >> L >> R >> x;
            int cnt = seg_query(segRoot[R], 1, MAX_VAL, x) - seg_query(segRoot[L-1], 1, MAX_VAL, x);
            cout << cnt << "\n";
        }
        else if(type == 5){
            // "5 L R k": A[L]부터 A[R] 중에서 k번째로 작은 원소 출력
            int L, R, k;
            cin >> L >> R >> k;
            int res = seg_kth(segRoot[R], segRoot[L-1], 1, MAX_VAL, k);
            cout << res << "\n";
        }
    }
    
    return 0;
}
