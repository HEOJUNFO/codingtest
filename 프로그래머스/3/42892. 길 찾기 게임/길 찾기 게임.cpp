#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

struct Node {
    int x, y, idx;
    Node* left;
    Node* right;
};

Node* buildTree(vector<Node*>& nodes, int start, int end) {
    if (start > end) return nullptr;
    // start부터 end 사이에서 y값 기준으로 정렬되어 있으므로
    // 첫 번째 노드가 가장 y가 큰 노드가 되어 루트가 됨
    Node* root = nodes[start];
    int rootX = root->x;

    int leftStart = start + 1;
    int leftEnd = start;
    int rightStart = start + 1;
    int rightEnd = end;

    // 루트의 x를 기준으로 왼쪽 서브트리와 오른쪽 서브트리로 나눈다.
    // nodes는 y 내림차순, x 오름차순 정렬 상태이므로 단순히 rootX기준으로 나눌 수 없다.
    // 모든 노드에 대해 rootX보다 작은 x는 왼쪽, 큰 x는 오른쪽으로 분류한다.
    // 하지만 여기서 주의할 점은, 단순히 왼쪽 오른쪽 구분만 하면 되므로,
    // 한번 순회하면서 leftGroup, rightGroup으로 나누어 다시 빌드한다.
    vector<Node*> leftGroup, rightGroup;
    for (int i = start + 1; i <= end; i++) {
        if (nodes[i]->x < rootX) {
            leftGroup.push_back(nodes[i]);
        } else {
            rightGroup.push_back(nodes[i]);
        }
    }

    // leftGroup과 rightGroup을 각각 재귀적으로 트리 빌드
    if (!leftGroup.empty()) {
        root->left = buildTree(leftGroup, 0, (int)leftGroup.size()-1);
    } else {
        root->left = nullptr;
    }

    if (!rightGroup.empty()) {
        root->right = buildTree(rightGroup, 0, (int)rightGroup.size()-1);
    } else {
        root->right = nullptr;
    }

    return root;
}

void preorder(Node* root, vector<int>& result) {
    if (!root) return;
    result.push_back(root->idx);
    preorder(root->left, result);
    preorder(root->right, result);
}

void postorder(Node* root, vector<int>& result) {
    if (!root) return;
    postorder(root->left, result);
    postorder(root->right, result);
    result.push_back(root->idx);
}

vector<vector<int>> solution(vector<vector<int>> nodeinfo) {
    int N = (int)nodeinfo.size();
    vector<Node*> nodes(N);
    for (int i = 0; i < N; i++) {
        nodes[i] = new Node{nodeinfo[i][0], nodeinfo[i][1], i+1, nullptr, nullptr};
    }

    // y값을 기준으로 내림차순 정렬, y가 같다면 x 오름차순 정렬
    sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
        if (a->y == b->y) return a->x < b->x;
        return a->y > b->y;
    });

    Node* root = buildTree(nodes, 0, N-1);

    vector<int> pre;
    vector<int> post;
    preorder(root, pre);
    postorder(root, post);

    return {pre, post};
}

int main() {
    vector<vector<int>> nodeinfo = {{5,3},{11,5},{13,3},{3,5},{6,1},{1,3},{8,6},{7,2},{2,2}};
    vector<vector<int>> ans = solution(nodeinfo);
    // 출력 테스트
    for (auto v : ans[0]) cout << v << " ";
    cout << endl;
    for (auto v : ans[1]) cout << v << " ";
    cout << endl;
    return 0;
}
