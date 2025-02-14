#include <bits/stdc++.h>
using namespace std;
 
// Treap node with augmented data for alternating sum.
// 각 노드에는 key(원소 값), priority, subtree 크기, 그리고 
// 해당 subtree를 오름차순 순서로 나열했을 때, 
// 시작이 even(0번 인덱스)일 때의 짝수 위치 합(altEven)과 홀수 위치 합(altOdd)를 저장합니다.
struct Node {
    int key;
    int priority;
    int size;
    long long altEven, altOdd; // alternating sums
    Node *left, *right;
    Node(int _key) : key(_key), priority(rand()), size(1), altEven(_key), altOdd(0), left(nullptr), right(nullptr) {}
};
 
// Returns subtree size.
int getSize(Node* t) {
    return t ? t->size : 0;
}
 
// 업데이트: t를 루트로 하는 subtree의 size, altEven, altOdd를 재계산.
void update(Node* t) {
    if(!t) return;
    t->size = 1 + getSize(t->left) + getSize(t->right);
    int lSize = getSize(t->left);
    long long leftEven = t->left ? t->left->altEven : 0;
    long long leftOdd = t->left ? t->left->altOdd : 0;
    // 현재 노드의 위치는 lSize번째 (0-indexed)
    long long even_here = (lSize % 2 == 0) ? t->key : 0;
    long long odd_here = (lSize % 2 == 1) ? t->key : 0;
    long long rightEven = 0, rightOdd = 0;
    if(t->right) {
        // 오른쪽 subtree의 원소들은 현재 노드 뒤에 오므로, 
        // 시작 인덱스는 (lSize+1)번째가 됩니다.
        if ((lSize + 1) % 2 == 0) {
            rightEven = t->right->altEven;
            rightOdd = t->right->altOdd;
        } else {
            // 만약 (lSize+1)이 홀수라면 오른쪽 subtree의 짝/홀수 위치가 뒤집힙니다.
            rightEven = t->right->altOdd;
            rightOdd = t->right->altEven;
        }
    }
    t->altEven = leftEven + even_here + rightEven;
    t->altOdd = leftOdd + odd_here + rightOdd;
}
 
// 두 Treap를 합칩니다.
Node* merge(Node* leftTree, Node* rightTree) {
    if(!leftTree) return rightTree;
    if(!rightTree) return leftTree;
    if(leftTree->priority > rightTree->priority) {
        leftTree->right = merge(leftTree->right, rightTree);
        update(leftTree);
        return leftTree;
    } else {
        rightTree->left = merge(leftTree, rightTree->left);
        update(rightTree);
        return rightTree;
    }
}
 
// key 기준으로 t를 분할: left에 key < 'key', right에 key >= 'key'
void split(Node* t, int key, Node*& leftTree, Node*& rightTree) {
    if(!t) {
        leftTree = rightTree = nullptr;
        return;
    }
    if(t->key < key) {
        split(t->right, key, t->right, rightTree);
        leftTree = t;
    } else {
        split(t->left, key, leftTree, t->left);
        rightTree = t;
    }
    update(t);
}
 
// Treap에 node를 삽입합니다.
Node* treapInsert(Node* t, Node* node) {
    if(!t) return node;
    if(node->priority > t->priority) {
        split(t, node->key, node->left, node->right);
        update(node);
        return node;
    } else if(node->key < t->key) {
        t->left = treapInsert(t->left, node);
        update(t);
    } else {
        t->right = treapInsert(t->right, node);
        update(t);
    }
    return t;
}
 
// Treap에서 key를 가진 노드를 제거합니다.
Node* treapErase(Node* t, int key) {
    if(!t) return nullptr;
    if(t->key == key) {
        Node* res = merge(t->left, t->right);
        // t는 메모리 해제할 수 있으나, 여기서는 생략
        return res;
    } else if(key < t->key) {
        t->left = treapErase(t->left, key);
        update(t);
    } else {
        t->right = treapErase(t->right, key);
        update(t);
    }
    return t;
}
 
// Mo's algorithm에서 사용할 전역 Treap 포인터 (현재 구간에 포함된 원소들을 저장)
Node* treapRoot = nullptr;
 
// 전역 배열 A
int n, q;
vector<int> A;
 
// Mo's query 구조체
struct Query {
    int L, R, idx;
};
 
// block size: 보통 sqrt(n)
int BLOCK_SIZE;
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    srand((unsigned)time(NULL));
    
    cin >> n >> q;
    A.resize(n);
    for (int i = 0; i < n; i++){
        cin >> A[i];
    }
    
    vector<Query> queries(q);
    for (int i = 0; i < q; i++){
        int l, r;
        cin >> l >> r;
        queries[i] = {l, r, i};
    }
    
    BLOCK_SIZE = max(1, (int) sqrt(n));
    sort(queries.begin(), queries.end(), [](const Query &a, const Query &b) {
        int ablock = a.L / BLOCK_SIZE, bblock = b.L / BLOCK_SIZE;
        if(ablock == bblock) return a.R < b.R;
        return ablock < bblock;
    });
    
    vector<long long> ans(q, 0);
    int curL = 0, curR = 0; // current segment [curL, curR)
    // 현재 구간이 비어있으므로 treapRoot는 nullptr
    
    auto add = [&](int pos) {
        // A[pos]를 treap에 삽입.
        treapRoot = treapInsert(treapRoot, new Node(A[pos]));
    };
    auto remove = [&](int pos) {
        // A[pos]를 treap에서 제거.
        treapRoot = treapErase(treapRoot, A[pos]);
    };
    
    for (auto &query : queries) {
        int L = query.L, R = query.R;
        // 구간을 [L, R]로 맞춥니다.
        while (curR <= R) {
            add(curR);
            curR++;
        }
        while (curR > R+1) {
            curR--;
            remove(curR);
        }
        while (curL < L) {
            remove(curL);
            curL++;
        }
        while (curL > L) {
            curL--;
            add(curL);
        }
        // 현재 treapRoot는 구간 [L,R]의 원소들을 값 기준 정렬한 결과의 alternating 정보를 저장함.
        long long curAns = (treapRoot ? treapRoot->altEven : 0);
        const long long MOD = 1000000007;
        ans[query.idx] = curAns % MOD;
    }
    
    for (auto &res : ans)
        cout << res << "\n";
    
    return 0;
}
