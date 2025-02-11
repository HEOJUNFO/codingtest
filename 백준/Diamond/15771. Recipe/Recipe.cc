#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// We choose a very low negative infinity.
const ll NEG_INF = -1000000000000000000LL; // about -1e18
// Our queries are over x = C[j] and C[j] <= 10000, so we use [0, 10001)
const int X_MIN = 0, X_MAX = 10001;
 
// A candidate “line” (from a buying day) is defined by:
//   f(x) = m*x + b, where m = F[k] + k and b = dp[k-1].
// (We also record m as an integer A so that later we can “filter” candidates.)
struct Line {
    ll m, b; // f(x) = m*x + b
    int A;   // candidate’s A value (equals m)
};
 
// Evaluate a line at x.
ll eval(const Line &line, int x) {
    return line.m * (ll)x + line.b;
}
 
// LiChao tree node.
struct LiChaoNode {
    int l, r; // covers interval [l, r)
    Line line;
    LiChaoNode *left, *right;
    // max_m is the maximum slope (i.e. candidate A value) among all lines in this subtree.
    int max_m;
    LiChaoNode(int l, int r, const Line &line) : l(l), r(r), line(line), left(nullptr), right(nullptr), max_m((int)line.m) {}
};
 
// LiChao tree supports adding lines and querying for 
// max{ f(x) } among lines that satisfy a minimum slope condition.
struct LiChaoTree {
    LiChaoNode *root;
    LiChaoTree() {
        // Create a dummy line that always returns -infinity.
        Line dummy;
        dummy.m = -1000000000LL; 
        dummy.b = NEG_INF;
        dummy.A = (int)dummy.m;
        root = new LiChaoNode(X_MIN, X_MAX, dummy);
    }
 
    // Add a line to the tree.
    void add_line(Line new_line) {
        add_line(root, new_line);
    }
 
    void add_line(LiChaoNode* node, Line new_line) {
        int l = node->l, r = node->r;
        int m = (l + r) >> 1;
        bool leftBetter = eval(new_line, l) > eval(node->line, l);
        bool midBetter  = eval(new_line, m) > eval(node->line, m);
        if(midBetter){
            swap(node->line, new_line);
        }
        if(r - l == 1) {
            node->max_m = max((int)node->line.m, node->max_m);
            return;
        }
        if(leftBetter != midBetter) {
            if(node->left == nullptr) {
                node->left = new LiChaoNode(l, m, new_line);
            } else {
                add_line(node->left, new_line);
            }
        } else {
            if(node->right == nullptr) {
                node->right = new LiChaoNode(m, r, new_line);
            } else {
                add_line(node->right, new_line);
            }
        }
        int leftMax = node->left ? node->left->max_m : -1000000000;
        int rightMax = node->right ? node->right->max_m : -1000000000;
        node->max_m = max({ (int)node->line.m, leftMax, rightMax });
    }
 
    // Query for the maximum f(x) among lines with slope >= threshold.
    ll query(int x, int threshold) {
        return query(root, x, threshold);
    }
 
    ll query(LiChaoNode* node, int x, int threshold) {
        if(node == nullptr) return NEG_INF;
        if(node->max_m < threshold) return NEG_INF;
        ll ret = NEG_INF;
        if(node->line.m >= threshold)
            ret = eval(node->line, x);
        int m = (node->l + node->r) >> 1;
        ll leftVal = query(node->left, x, threshold);
        ll rightVal = query(node->right, x, threshold);
        return max({ret, leftVal, rightVal});
    }
};
 
// Main – we now set up the DP.
// We want dp[j] = max_{1<=k<=j with A[k]>=L[j]+j} { dp[k-1] + (A[k]-j)*C[j] }
//           = -j*C[j] + max_{1<=k<=j with A[k]>=L[j]+j} { dp[k-1] + A[k]*C[j] }.
// We “store” each candidate from a buying day k as a line 
//   f_k(x) = A[k]*x + dp[k-1],
// and then for a cooking day j we query at x = C[j] while only considering those
// lines with A[k] >= (L[j]+j).
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<int> F(N+1), C(N+1), L_req(N+1);
    for (int i = 1; i <= N; i++){
        cin >> F[i];
    }
    for (int i = 1; i <= N; i++){
        cin >> C[i];
    }
    for (int i = 1; i <= N; i++){
        cin >> L_req[i];
    }
 
    // dp[j] = best total taste if we have finished cooking on day j.
    // dp[0] = 0 (before any day, fridge is empty).
    vector<ll> dp(N+1, NEG_INF);
    dp[0] = 0;
 
    LiChaoTree tree;
    // On day 1 we must buy (since fridge is empty).
    // Candidate for buying day 1: slope = A[1] = F[1]+1, intercept = dp[0]=0.
    {
        Line line;
        line.m = F[1] + 1;
        line.b = dp[0];
        line.A = (int)line.m;
        tree.add_line(line);
    }
 
    // Process each day j as a potential cooking day.
    for (int j = 1; j <= N; j++){
        // For a valid cooking on day j, the candidate must satisfy:
        // A[k] >= L_req[j] + j.
        int threshold = L_req[j] + j;
        // Query at x = C[j] for the best candidate value dp[k-1] + A[k]*C[j]
        ll bestCandidate = tree.query(C[j], threshold);
        if(bestCandidate == NEG_INF)
            dp[j] = NEG_INF;
        else
            dp[j] = bestCandidate - (ll)j * C[j];
 
        // After cooking on day j the fridge becomes empty.
        // (Except on the last day we don’t buy again.)
        if(j < N && dp[j] != NEG_INF){
            // The next buying day is j+1.
            // The candidate line will be: slope = F[j+1] + (j+1), intercept = dp[j].
            Line newLine;
            newLine.m = F[j+1] + (j+1);
            newLine.b = dp[j];
            newLine.A = (int)newLine.m;
            tree.add_line(newLine);
        }
    }
 
    if(dp[N] == NEG_INF)
        cout << "Impossible\n";
    else
        cout << dp[N] << "\n";
 
    return 0;
}
