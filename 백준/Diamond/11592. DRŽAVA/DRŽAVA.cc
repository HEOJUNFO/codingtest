#include <bits/stdc++.h>
using namespace std;
 
// -------------------- Data Structures ---------------------
 
struct Point {
    double x, y;
    int idx;
    int residents; // resident count (not divisible by K)
};
 
int N, K;
vector<Point> pts;
 
// -------------------- KD‐tree ---------------------
// We use a simple 2D KD–tree (splitting alternates by x and y)
 
struct KDNode {
    int idx; // index into pts
    int dim; // splitting dimension: 0 for x, 1 for y
    KDNode *left, *right;
};
 
// Build KD–tree recursively; indices is the list of point indices.
KDNode* buildKDTree(vector<int>& indices, int depth) {
    if(indices.empty()) return nullptr;
    int dim = depth % 2;
    int mid = indices.size() / 2;
    auto comp = [dim](int a, int b) {
        return (dim==0 ? pts[a].x < pts[b].x : pts[a].y < pts[b].y);
    };
    nth_element(indices.begin(), indices.begin()+mid, indices.end(), comp);
    KDNode* node = new KDNode();
    node->idx = indices[mid];
    node->dim = dim;
    vector<int> leftIndices(indices.begin(), indices.begin()+mid);
    vector<int> rightIndices(indices.begin()+mid+1, indices.end());
    node->left = buildKDTree(leftIndices, depth+1);
    node->right = buildKDTree(rightIndices, depth+1);
    return node;
}
 
// For nearest–neighbor search we use a max–heap (keeping the closest k points by squared distance)
struct HeapElem {
    double dist;
    int idx;
};
 
struct HeapComp {
    bool operator()(const HeapElem &a, const HeapElem &b) {
        return a.dist < b.dist; // so that the largest distance is on top (max–heap)
    }
};
 
// Query the KD–tree for k nearest neighbors to (qx,qy) excluding the point with index queryIdx.
void kdQuery(KDNode* node, double qx, double qy, int queryIdx, int k, 
             priority_queue<HeapElem, vector<HeapElem>, HeapComp>& heap) {
    if(!node) return;
    int cur = node->idx;
    if(cur != queryIdx) {
        double dx = pts[cur].x - qx, dy = pts[cur].y - qy;
        double d = dx*dx + dy*dy;
        if(heap.size() < (size_t)k) {
            heap.push({d, cur});
        } else if(d < heap.top().dist) {
            heap.pop();
            heap.push({d, cur});
        }
    }
    int dim = node->dim;
    double diff = (dim==0 ? qx - pts[cur].x : qy - pts[cur].y);
    KDNode *first = diff < 0 ? node->left : node->right;
    KDNode *second = diff < 0 ? node->right : node->left;
    kdQuery(first, qx, qy, queryIdx, k, heap);
    double diff2 = diff*diff;
    if(heap.size() < (size_t)k || diff2 < heap.top().dist)
         kdQuery(second, qx, qy, queryIdx, k, heap);
}
 
// -------------------- Edge and Union–Find ---------------------
 
struct Edge {
    int u, v;
    double w;
};
 
// Union–find structure that also holds a DP bitmask (an int with K bits).
struct UF {
    vector<int> parent, rank;
    vector<int> dp; // dp[i] : bitmask for component rooted at i.
    int K;
    UF(int n, int K): parent(n), rank(n,0), dp(n,0), K(K) {
        for(int i=0; i<n; i++) parent[i] = i;
    }
    int find(int a) { return parent[a] = (parent[a]==a ? a : find(parent[a])); }
    // Combine two DP masks: newMask = maskA ∪ maskB ∪ { (i+j)%K for i in maskA, j in maskB }.
    int combineDP(int maskA, int maskB) {
        int res = maskA | maskB;
        for (int i = 0; i < K; i++) {
            if(maskA & (1 << i))
                for (int j = 0; j < K; j++)
                    if(maskB & (1 << j))
                        res |= (1 << ((i+j) % K));
        }
        return res;
    }
    bool unionSets(int a, int b) {
        a = find(a); b = find(b);
        if(a==b) return false;
        if(rank[a] < rank[b]) swap(a,b);
        parent[b] = a;
        if(rank[a]==rank[b]) rank[a]++;
        dp[a] = combineDP(dp[a], dp[b]);
        return true;
    }
};
 
// -------------------- Main ---------------------
 
// We generate candidate edges via KD–tree k–nearest–neighbors; then we sort them by weight and process in order.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> N >> K;
    pts.resize(N);
    for (int i = 0; i < N; i++){
        int xi, yi, ki;
        cin >> xi >> yi >> ki;
        pts[i].x = xi; pts[i].y = yi; pts[i].idx = i;
        pts[i].residents = ki;
    }
 
    // Build KD–tree on all points.
    vector<int> indices(N);
    for (int i = 0; i < N; i++) indices[i] = i;
    KDNode* kdRoot = buildKDTree(indices, 0);
 
    // For each point, find its k–nearest neighbors.
    int kNearest = 50; 
    vector<Edge> edges;
    edges.reserve((size_t)N * kNearest / 2);
    for (int i = 0; i < N; i++){
        priority_queue<HeapElem, vector<HeapElem>, HeapComp> heap;
        kdQuery(kdRoot, pts[i].x, pts[i].y, i, kNearest, heap);
        vector<HeapElem> cand;
        while(!heap.empty()){
            cand.push_back(heap.top());
            heap.pop();
        }
        for(auto &he : cand){
            int j = he.idx;
            if(i < j){
                double dist = sqrt(he.dist);
                edges.push_back({i, j, dist});
            }
        }
    }
    // (Optional: free the KD–tree memory.)
    vector<KDNode*> st; 
    if(kdRoot) st.push_back(kdRoot);
    while(!st.empty()){
        KDNode* node = st.back(); st.pop_back();
        if(node->left) st.push_back(node->left);
        if(node->right) st.push_back(node->right);
        delete node;
    }
 
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){
        return a.w < b.w;
    });
 
    // Initialize union–find.
    UF uf(N, K);
    for (int i = 0; i < N; i++){
        int rem = pts[i].residents % K; // nonzero by input guarantee
        uf.dp[i] = (1 << rem);
    }
 
    double ans = 0.0;
    // Process candidate edges (which cover the MST) in increasing order.
    for(auto &e : edges){
        if(uf.unionSets(e.u, e.v)){
            int root = uf.find(e.u);
            if(uf.dp[root] & 1){ // if bit 0 is set, some subset sums to a multiple of K
                ans = e.w;
                break;
            }
        }
    }
    cout << fixed << setprecision(3) << ans << "\n";
    return 0;
}
