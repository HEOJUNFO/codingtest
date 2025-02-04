#include <bits/stdc++.h>
using namespace std;
struct Line {
    long long m, b;
    long long eval(long long x) const { return m * x + b; }
};
struct LiChao {
    struct Node {  
        Line line;
        Node *left, *right;
        Node(Line ln) : line(ln), left(NULL), right(NULL) {}
    };
    Node* root;
    long long minX, maxX;
    LiChao(long long lo, long long hi) : root(NULL), minX(lo), maxX(hi) {}
    void addLine(Line newLine) { addLineRec(root, minX, maxX, newLine); }
    // 내부 재귀 함수: 구간 [l, r)에서 노드를 관리
    void addLineRec(Node*& node, long long l, long long r, Line newLine) {
        if (!node) {  
            node = new Node(newLine);
            return;
        }
        long long mid = (l + r) / 2;
        // mid에서 더 좋은(큰) 직선을 node->line으로 유지
        if (node->line.eval(mid) < newLine.eval(mid))  
            swap(node->line, newLine);
        if (r - l <= 1) return;  // 구간 길이가 1이면 리프 노드
        if (node->line.eval(l) < newLine.eval(l)) {  
            // 새로운 직선이 l에서 더 크다면 left 자식에 삽입
            addLineRec(node->left, l, mid, newLine);
        } else if (node->line.eval(r - 1) < newLine.eval(r - 1)) {  
            // r-1에서 더 크다면 right 자식에 삽입
            addLineRec(node->right, mid, r, newLine);
        }
    }
    long long query(long long x) const { return queryRec(root, minX, maxX, x); }
    long long queryRec(Node* node, long long l, long long r, long long x) const {
        if (!node) return LLONG_MIN;
        long long mid = (l + r) / 2;
        long long res = node->line.eval(x);
        if (r - l <= 1) {  
            // 리프 노드 구간
            return res;
        }
        if (x < mid) {  
            // 왼쪽 구간에 속하는 x
            long long candidate = queryRec(node->left, l, mid, x);
            if (candidate > res) res = candidate;
        } else {  
            // 오른쪽 구간에 속하는 x
            long long candidate = queryRec(node->right, mid, r, x);
            if (candidate > res) res = candidate;
        }
        return res;
    }
};
struct Operation { int type; long long a, b, x, idx; };
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    vector<Operation> ops(N + 1);
    for (int i = 1; i <= N; ++i) {
        int t;
        cin >> t;
        ops[i].type = t;
        if (t == 1) {
            cin >> ops[i].a >> ops[i].b;
        } else if (t == 2) {
            cin >> ops[i].idx;
        } else if (t == 3) {
            cin >> ops[i].x;
        }
    }

    // 각 추가된 직선의 제거 시점을 계산
    vector<int> removeTime(N + 2, N + 1);  // 제거되지 않으면 N+1 (종료 이후까지 유효)
    for (int i = 1; i <= N; ++i) {
        if (ops[i].type == 2) {
            int addIdx = ops[i].idx;
            removeTime[addIdx] = i;  // addIdx에서 추가된 직선이 i에서 제거됨
        }
    }

    // 모든 질의 x값의 범위를 구해서 Li Chao Tree 범위로 사용
    long long minX = LLONG_MAX, maxX = LLONG_MIN;
    for (int i = 1; i <= N; ++i) {
        if (ops[i].type == 3) {
            minX = min(minX, ops[i].x);
            maxX = max(maxX, ops[i].x);
        }
    }
    if (minX == LLONG_MAX) {  // 질의가 없는 경우 (문제 조건상 최소 하나는 있을 것으로 가정)
        minX = 0;
        maxX = 0;
    } else {
        // 범위를 약간 확장 (maxX+1)하여 반개구간 [minX, maxX+1)으로 취급
        maxX += 1;
    }

    // 세그먼트 트리 구조 할당 (4*N 크기)
    int size = 1;
    while (size < N) size *= 2;
    vector<LiChao*> segTree(size * 2, nullptr);

    // 각 추가 연산의 직선을 해당 구간에 삽입
    function<void(int, int, int, int, int, const Line&)> addLine = [&](int node, int l, int r, int ql, int qr, const Line& line) {
        if (ql > r || qr < l) return;  // 구간 밖
        if (ql <= l && r <= qr) {
            // 노드 구간 전체가 직선의 활성 구간에 포함되는 경우
            if (!segTree[node]) {
                segTree[node] = new LiChao(minX, maxX);
            }
            segTree[node]->addLine(line);
        } else {
            int mid = (l + r) / 2;
            addLine(node * 2, l, mid, ql, qr, line);
            addLine(node * 2 + 1, mid + 1, r, ql, qr, line);
        }
    };

    for (int i = 1; i <= N; ++i) {
        if (ops[i].type == 1) {  // 직선 추가 연산
            Line newLine = { ops[i].a, ops[i].b };
            int L = i;
            int R = removeTime[i] - 1;  // [L, R] 구간 동안 활성
            if (R < L) continue;        // 만약 추가 후 바로 제거되어 활성 구간이 없다면 패스
            addLine(1, 1, size, L, R, newLine);
        }
    }

    // 모든 질의 처리 및 출력
    string output;
    output.reserve(N * 5);  // 예상 출력 크기 reservation (임의로 설정)
    for (int i = 1; i <= N; ++i) {
        if (ops[i].type == 3) {
            // 세그먼트 트리를 따라가며 각 노드에서 최대값 계산
            long long queryX = ops[i].x;
            long long best = LLONG_MIN;
            int node = 1, l = 1, r = size;
            // 루트부터 리프까지 이분 탐색하며 경로상의 노드들 검사
            while (true) {
                if (segTree[node]) {
                    long long candidate = segTree[node]->query(queryX);
                    if (candidate > best) best = candidate;
                }
                if (l == r) break;  // 리프 도달
                int mid = (l + r) / 2;
                if (i <= mid) {
                    node = node * 2;
                    r = mid;
                } else {
                    node = node * 2 + 1;
                    l = mid + 1;
                }
            }
            if (best == LLONG_MIN) {
                output += "EMPTY\n";
            } else {
                output += to_string(best) + "\n";
            }
        }
    }

    // 결과 한꺼번에 출력 (I/O 최소화)
    cout << output;
}
