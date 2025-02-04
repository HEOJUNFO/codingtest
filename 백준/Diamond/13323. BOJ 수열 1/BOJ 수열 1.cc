#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

// Treap의 노드: key(원소값), priority, subtree 크기(sz)와 합(sum)
struct Node {
    ll key;
    int priority;
    int sz;
    ll sum;
    Node* left;
    Node* right;
};

inline int getSize(Node* root) {
    return root ? root->sz : 0;
}
inline ll getSum(Node* root) {
    return root ? root->sum : 0;
}
inline void update(Node* root) {
    if(root) {
        root->sz = 1 + getSize(root->left) + getSize(root->right);
        root->sum = root->key + getSum(root->left) + getSum(root->right);
    }
}

Node* newNode(ll key) {
    Node* node = new Node;
    node->key = key;
    node->priority = rand();
    node->sz = 1;
    node->sum = key;
    node->left = node->right = nullptr;
    return node;
}

// split 함수: root를 key 미만과 이상의 두 트리로 분리
void split(Node* root, ll key, Node*& left, Node*& right) {
    if(!root) { left = right = nullptr; return; }
    if(root->key < key) {
        split(root->right, key, root->right, right);
        left = root;
        update(left);
    } else {
        split(root->left, key, left, root->left);
        right = root;
        update(right);
    }
}

// 표준 Treap 삽입: node를 root에 삽입하여 새 root를 반환
Node* treapInsert(Node* root, Node* node) {
    if(!root) return node;
    if(node->priority > root->priority) {
        split(root, node->key, node->left, node->right);
        update(node);
        return node;
    } else if(node->key <= root->key) {
        root->left = treapInsert(root->left, node);
        update(root);
        return root;
    } else {
        root->right = treapInsert(root->right, node);
        update(root);
        return root;
    }
}

// 작은 트리 small의 모든 노드를 큰 트리 big에 삽입 (in-order 순회)
void mergeTreap(Node* small, Node*& big) {
    if(!small) return;
    mergeTreap(small->left, big);
    mergeTreap(small->right, big);
    small->left = small->right = nullptr;
    small->sz = 1;
    small->sum = small->key;
    big = treapInsert(big, small);
}

// k번째(0-indexed) 원소를 반환하는 함수
Node* getKth(Node* root, int k) {
    int leftSize = getSize(root->left);
    if(k < leftSize) return getKth(root->left, k);
    else if(k == leftSize) return root;
    else return getKth(root->right, k - leftSize - 1);
}

// k개의 원소(가장 작은 순서대로)의 합을 구함 (k>=0, k<=getSize(root))
ll getPrefixSum(Node* root, int k) {
    if(!root || k <= 0) return 0;
    int leftSize = getSize(root->left);
    if(k <= leftSize) return getPrefixSum(root->left, k);
    else if(k == leftSize + 1) return getSum(root->left) + root->key;
    else return getSum(root->left) + root->key + getPrefixSum(root->right, k - leftSize - 1);
}

// 주어진 Treap에 대해, 중앙값(인덱스 (n-1)/2를 선택) 기준의 비용(절대 오차 합) 계산
ll getCost(Node* root) {
    int n = getSize(root);
    int medianIndex = (n - 1) / 2; // 0-indexed
    Node* medianNode = getKth(root, medianIndex);
    ll med = medianNode->key;
    ll leftSum = getPrefixSum(root, medianIndex);
    int leftCount = medianIndex;
    ll total = getSum(root);
    ll costLeft = med * leftCount - leftSum;
    int rightCount = n - medianIndex - 1;
    ll medianAndLeft = getPrefixSum(root, medianIndex + 1);
    ll rightSum = total - medianAndLeft;
    ll costRight = rightSum - med * rightCount;
    return costLeft + costRight;
}

// 빠른 입출력을 위한 fread 기반의 입력 함수
static const int BUFSZ = 1 << 20;
static char buffer[BUFSZ];
static int pos = 0, len = 0;
inline int getChar() {
    if(pos == len) {
        len = (int)fread(buffer, 1, BUFSZ, stdin);
        if(len == 0) return EOF;
        pos = 0;
    }
    return buffer[pos++];
}
inline ll fastReadLL() {
    int c = getChar();
    while(c != EOF && (c < '0' || c > '9')) c = getChar();
    ll x = 0;
    while(c != EOF && (c >= '0' && c <= '9')) {
        x = x * 10 + (c - '0');
        c = getChar();
    }
    return x;
}

// 각 블록은 해당 Treap 포인터를 저장
struct Block {
    Node* treap;
};

//
// main – PAVA 알고리즘 수행
//
int main(){
    int n = (int)fastReadLL();
    vector<ll> A(n);
    for (int i = 0; i < n; i++){
        A[i] = fastReadLL();
    }
    
    // 변환: d[i] = A[i] - i (0-indexed)
    // 최종적으로 B[i] = C[i] + i가 되고, C는 비감소 수열이어야 함.
    vector<Block> stack;
    stack.reserve(n);
    
    for (int i = 0; i < n; i++){
        ll d = A[i] - i;
        Block b;
        b.treap = newNode(d);
        stack.push_back(b);
        
        // PAVA: 스택의 마지막 두 블록에 대해 중앙값이 non-decreasing하도록 병합
        while(stack.size() >= 2) {
            int sz = stack.size();
            Node* t1 = stack[sz - 2].treap;
            Node* t2 = stack[sz - 1].treap;
            ll m1 = getKth(t1, (getSize(t1)-1)/2)->key;
            ll m2 = getKth(t2, (getSize(t2)-1)/2)->key;
            if(m1 <= m2) break;
            // union-by-size: 더 작은 Treap의 노드를 더 큰 Treap에 삽입
            Node* mergedTreap = nullptr;
            if(getSize(t1) < getSize(t2)) {
                mergedTreap = t2;
                mergeTreap(t1, mergedTreap);
            } else {
                mergedTreap = t1;
                mergeTreap(t2, mergedTreap);
            }
            stack.pop_back();
            stack.pop_back();
            Block nb;
            nb.treap = mergedTreap;
            stack.push_back(nb);
        }
    }
    
    // 최종 남은 블록들의 비용을 모두 합산
    ll totalCost = 0;
    for(auto &b : stack) {
        totalCost += getCost(b.treap);
    }
    
    printf("%lld\n", totalCost);
    return 0;
}
