#include <bits/stdc++.h>
using namespace std;
 
// BIT (Fenwick tree) for point updates and prefix sums.
struct BIT {
    int n;
    vector<int> tree;
    BIT(int n): n(n), tree(n+1, 0) {}
    inline void update(int i, int delta) {
        for(; i <= n; i += i & -i)
            tree[i] += delta;
    }
    inline int query(int i) {
        int s = 0;
        for(; i > 0; i -= i & -i)
            s += tree[i];
        return s;
    }
};
 
// Structure for an offline query.
struct Query {
    int x;      // threshold on the x coordinate (prefix order index)
    int l, r;   // we want to count points with y in [l, r)
    int idx;    // query index (to add answer)
    int sign;   // +1 or -1 for inclusion/exclusion
};
 
// Main function.
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    vector<string> dict(N);
    for (int i = 0; i < N; i++){
        cin >> dict[i];
    }
 
    // Build an array for prefix ordering.
    vector<pair<string, int>> prefixArr(N);
    for (int i = 0; i < N; i++){
        prefixArr[i] = {dict[i], i};
    }
    sort(prefixArr.begin(), prefixArr.end());
    vector<int> rankPrefix(N);
    for (int i = 0; i < N; i++){
        rankPrefix[prefixArr[i].second] = i;
    }
 
    // Build an array for suffix ordering (sort by reversed string).
    vector<pair<string, int>> suffixArr(N);
    for (int i = 0; i < N; i++){
        string s = dict[i];
        reverse(s.begin(), s.end());
        suffixArr[i] = {s, i};
    }
    sort(suffixArr.begin(), suffixArr.end());
    vector<int> rankSuffix(N);
    for (int i = 0; i < N; i++){
        rankSuffix[suffixArr[i].second] = i;
    }
 
    // In the prefix ordering, each dictionary string (now a “point”) has:
    //   x = its index in prefixArr (0-indexed)
    //   y = its suffix rank (from rankSuffix)
    // We can simply build an array of y-coordinates in prefix order.
    vector<int> pointY(N);
    for (int i = 0; i < N; i++){
        int orig = prefixArr[i].second;
        pointY[i] = rankSuffix[orig];
    }
 
    // Prepare offline queries.
    // For a query (P, Q): we want to count dictionary strings S with:
    //    S has prefix P  --> S lies in prefixArr positions [L1, R1)
    //    S has suffix Q  --> S (when reversed) lies in suffixArr positions [L2, R2)
    // Hence the answer is the count of points (x,y) with x in [L1, R1) and y in [L2, R2).
    // We will answer such a 2D range count query by computing:
    //   f(R1, L2, R2) - f(L1, L2, R2)
    // where f(X, L2, R2) counts points with x < X and y in [L2, R2).
    vector<Query> queries;
    queries.reserve(2LL * M);
    vector<long long> answer(M, 0);
 
    // A small lambda for lexicographical compare (we compare only the string part)
    auto cmpPair = [](const pair<string,int>& a, const pair<string,int>& b) {
        return a.first < b.first;
    };
 
    for (int i = 0; i < M; i++){
        string P, Q;
        cin >> P >> Q;
        // Find the block in prefixArr for strings with prefix P.
        auto lowerIt = lower_bound(prefixArr.begin(), prefixArr.end(), make_pair(P, -1), cmpPair);
        string P_next = P; 
        P_next.push_back('{'); // '{' is greater than 'U'
        auto upperIt = lower_bound(prefixArr.begin(), prefixArr.end(), make_pair(P_next, -1), cmpPair);
        int L1 = lowerIt - prefixArr.begin();
        int R1 = upperIt - prefixArr.begin();
 
        // Similarly, for suffix: use reversed Q.
        string Q_rev = Q;
        reverse(Q_rev.begin(), Q_rev.end());
        auto lowerIt2 = lower_bound(suffixArr.begin(), suffixArr.end(), make_pair(Q_rev, -1), cmpPair);
        string Q_next = Q_rev;
        Q_next.push_back('{');
        auto upperIt2 = lower_bound(suffixArr.begin(), suffixArr.end(), make_pair(Q_next, -1), cmpPair);
        int L2 = lowerIt2 - suffixArr.begin();
        int R2 = upperIt2 - suffixArr.begin();
 
        // Create two queries for inclusion–exclusion.
        Query q1; q1.x = R1; q1.l = L2; q1.r = R2; q1.idx = i; q1.sign = 1;
        Query q2; q2.x = L1; q2.l = L2; q2.r = R2; q2.idx = i; q2.sign = -1;
        queries.push_back(q1);
        queries.push_back(q2);
    }
 
    // Sort all queries by the threshold x.
    sort(queries.begin(), queries.end(), [](const Query &a, const Query &b){
        return a.x < b.x;
    });
 
    // Process queries offline using BIT.
    BIT bit(N);
    int pointIdx = 0;  // will go over the prefix-sorted points (x=0...N-1)
    for (auto &q : queries) {
        // Add all points with x-coordinate < q.x into BIT.
        while (pointIdx < q.x) {
            // The y-coordinate for point pointIdx is pointY[pointIdx].
            // BIT is 1-indexed so update at (pointY[pointIdx] + 1).
            bit.update(pointY[pointIdx] + 1, 1);
            pointIdx++;
        }
        // Now count points with y in [q.l, q.r).
        int cnt = bit.query(q.r) - bit.query(q.l);
        answer[q.idx] += q.sign * cnt;
    }
 
    // Output answers.
    for (int i = 0; i < M; i++){
        cout << answer[i] << "\n";
    }
 
    return 0;
}
