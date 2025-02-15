#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1LL << 60;

// 전역 변수
int n, m;
vector<int> a;
vector<ll> P; // prefix sum, P[0]=0, P[i]= a[1]+...+a[i]

// --- Sparse Table (RMQ for array a, storing indices) ---
int LOG[200005];
vector<vector<int>> st; // st[i][j]: index of minimum in [i, i+2^j-1]

void buildSparseTable() {
    LOG[1] = 0;
    for (int i = 2; i <= n; i++) {
        LOG[i] = LOG[i/2] + 1;
    }
    int K = LOG[n] + 1;
    st.assign(n+1, vector<int>(K));
    for (int i = 1; i <= n; i++) {
        st[i][0] = i;
    }
    for (int j = 1; j < K; j++) {
        for (int i = 1; i + (1 << j) - 1 <= n; i++) {
            int idx1 = st[i][j-1], idx2 = st[i + (1 << (j-1))][j-1];
            // a[idx1]와 a[idx2] 중 더 작은 값의 인덱스를 저장
            st[i][j] = (a[idx1-1] <= a[idx2-1] ? idx1 : idx2);
        }
    }
}

int queryRMQ(int L, int R) { // [L,R]
    int j = LOG[R - L + 1];
    int idx1 = st[L][j], idx2 = st[R - (1 << j) + 1][j];
    return (a[idx1-1] <= a[idx2-1] ? idx1 : idx2);
}

// --- LiChao Tree (for lines of form f(x) = m * t + b) ---
// 여기서 각 “line”은 index x에 대해 f_x(t) = x * t - P[x-1] (x는 1-indexed)
// 질의 시 t = a[r] (어떤 상수)로 평가하며, 구간 내 x에 대해 최소값을 구할 것이다.
struct Line {
    ll m, b; // f(t) = m * t + b
};

struct LiChao {
    struct Node {
        Line line;
        Node *l, *r;
        Node(Line line) : line(line), l(nullptr), r(nullptr) {}
    };
    Node* root;
    ll L, R; // t의 정의역, 여기선 t ∈ [0, MAX_T]
    LiChao(ll L, ll R) : L(L), R(R) {
        // 초기에는 매우 큰 값을 갖는 선을 넣어둔다.
        root = new Node({0, INF});
    }
    
    ll eval(const Line &line, ll x) {
        return line.m * x + line.b;
    }
    
    void add_line(Line newline, Node*& node, ll l, ll r) {
        if(!node) {
            node = new Node(newline);
            return;
        }
        ll mid = (l + r) >> 1;
        bool leftBetter = eval(newline, l) < eval(node->line, l);
        bool midBetter  = eval(newline, mid) < eval(node->line, mid);
        if(midBetter) {
            swap(node->line, newline);
        }
        if(r - l == 1) return;
        if(leftBetter != midBetter) {
            add_line(newline, node->l, l, mid);
        } else {
            add_line(newline, node->r, mid, r);
        }
    }
    
    void add_line(Line newline) {
        add_line(newline, root, L, R);
    }
    
    ll query(Node* node, ll l, ll r, ll x) {
        if(!node) return INF;
        ll mid = (l + r) >> 1;
        ll ret = eval(node->line, x);
        if(r - l == 1) return ret;
        if(x < mid) ret = min(ret, query(node->l, l, mid, x));
        else ret = min(ret, query(node->r, mid, r, x));
        return ret;
    }
    
    ll query(ll x) {
        return query(root, L, R, x);
    }
};

// --- Segment Tree over indices [1,n] for LiChao trees ---
// 각 노드에는 해당 구간에 속하는 인덱스 i에 대해 line f_i(t)= i*t - P[i-1] 를 모두 넣은 LiChao tree를 구축한다.
int segSize;
vector<LiChao*> segTree; // 1-indexed 세그먼트 트리

// t의 정의역: a[i]는 최대 1e4이므로, t ∈ [0, 10001] (약간 여유있게)
const ll T_LOW = 0, T_HIGH = 10001;

void buildSegTree(vector<ll>& linesIndex) {
    // segTree의 크기는 4*n 정도.
    segSize = n;
    segTree.resize(4 * segSize, nullptr);
    // 재귀적으로 빌드.
    function<void(int, int, int)> build = [&](int idx, int l, int r) {
        segTree[idx] = new LiChao(T_LOW, T_HIGH);
        if(l == r) {
            // 인덱스 l에 해당하는 line
            ll m_val = l; // slope
            ll b_val = -P[l-1];
            segTree[idx]->add_line({m_val, b_val});
            return;
        }
        int mid = (l + r) >> 1;
        build(idx*2, l, mid);
        build(idx*2+1, mid+1, r);
        // 두 자식의 선들을 합치기
        // 자식에 들어간 모든 선을 현재 노드에 추가한다.
        // (합치는 비용은 O(k log(range)) – 전체 O(n log n) 예상)
        // 먼저 왼쪽 자식의 선들을 수집.
        vector<Line> lines;
        function<void(typename LiChao::Node*, ll, ll)> collect = [&](LiChao::Node* node, ll L, ll R) {
            if(!node) return;
            lines.push_back(node->line);
            if(node->l) collect(node->l, L, (L+R)/2);
            if(node->r) collect(node->r, (L+R)/2, R);
        };
        collect(segTree[idx*2]->root, T_LOW, T_HIGH);
        collect(segTree[idx*2+1]->root, T_LOW, T_HIGH);
        for(auto &ln : lines) {
            segTree[idx]->add_line(ln);
        }
    };
    build(1, 1, segSize);
}

ll querySeg(int idx, int l, int r, int ql, int qr, ll t) {
    if(ql > r || qr < l) return INF;
    if(ql <= l && r <= qr) {
        return segTree[idx]->query(t);
    }
    int mid = (l + r) >> 1;
    return min(querySeg(idx*2, l, mid, ql, qr, t),
               querySeg(idx*2+1, mid+1, r, ql, qr, t));
}

// --- 분할정복 함수 ---
// solve(ql, qr, I, j): 고정 j, I (쿼리의 행 수) 에 대해, x ∈ [ql, qr]에서
// F(x) = (P[j]-P[x-1]) + (I - (j - x + 1)) * (min_{k=x}^{j} a[k])
// 의 최소값을 구한다. (여기서 rmq는 a의 구간 최소 인덱스 질의)
ll solveDC(int ql, int qr, int I, int j) {
    if(ql > qr) return INF;
    int r = queryRMQ(ql, qr); // 구간 [ql,qr]에서 최소 a값의 인덱스
    // 구간 [ql, r]에 대해, x ∈ [ql, r]이면, min_{k=x}^{j} a[k] = a[r] (r이 포함되므로)
    // 그러므로 F(x) = (P[j]-P[x-1]) + (I - (j - x + 1)) * a[r]
    // = P[j] + (I - j - 1)*a[r] + (x * a[r] - P[x-1])
    ll baseConst = P[j] + (ll)(I - j - 1) * a[r-1];
    // querySeg로 qx = min_{x in [ql, r]} (x * a[r] - P[x-1])
    ll cand1 = baseConst + querySeg(1, 1, segSize, ql, r, a[r-1]);
    ll cand2 = solveDC(r+1, qr, I, j);
    return min(cand1, cand2);
}

// --- 메인 쿼리 처리 함수 ---
// 쿼리 (I, j) 에 대해, x 범위는 [j - I + 1, j] (I <= j)
ll processQuery(int I, int j) {
    int L = j - I + 1;
    return solveDC(L, j, I, j);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    a.resize(n);
    for (int i = 0; i < n; i++){
        cin >> a[i];
    }
    // P[0]=0, P[i]= a[0]+...+a[i-1] (i>=1)
    P.resize(n+1);
    P[0] = 0;
    for (int i = 1; i <= n; i++){
        P[i] = P[i-1] + a[i-1];
    }
    
    buildSparseTable();
    buildSegTree(P);  // segTree 구축: P를 이용하여 -P[x-1]를 저장
    
    cin >> m;
    while(m--){
        int I, j;
        cin >> I >> j; // 1 ≤ I ≤ j ≤ n
        // f(I,j) = 답
        ll ans = processQuery(I, j);
        cout << ans << "\n";
    }
    
    return 0;
}
