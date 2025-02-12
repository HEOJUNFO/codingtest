#include <bits/stdc++.h>
using namespace std;
 
// Fenwick tree for point updates and prefix sums.
struct Fenw {
    int n;
    vector<int> fenw;
    Fenw(int n) : n(n), fenw(n+1, 0) {}
    void update(int i, int delta) {
        for(i++; i <= n; i += i & -i)
            fenw[i] += delta;
    }
    int query(int i) {
        if(i < 0) return 0;
        int s = 0;
        for(i++; i > 0; i -= i & -i)
            s += fenw[i];
        return s;
    }
    int rangeQuery(int l, int r) {
        if(l > r) return 0;
        return query(r) - query(l-1);
    }
};
 
// Comparator that compares only the string component.
struct PairStrCmp {
    bool operator()(const pair<string,int> &a, const pair<string,int> &b) const {
        return a.first < b.first;
    }
};
 
// Struct to hold an offline BIT query.
struct Query {
    int boundary; // prefix-sorted index boundary at which we want the BIT sum
    int L2, R2;   // we want to count how many have suffix-index in [L2, R2)
    int qid;      // query id
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    vector<string> chains(N);
    for (int i = 0; i < N; i++){
        cin >> chains[i];
    }
 
    // Build an array for prefix-sorted order.
    vector<pair<string,int>> prefixArr;
    prefixArr.reserve(N);
    for (int i = 0; i < N; i++){
        prefixArr.push_back({chains[i], i});
    }
    sort(prefixArr.begin(), prefixArr.end(), PairStrCmp());
 
    // Build an array for suffix-sorted order (store reversed chain).
    vector<pair<string,int>> suffixArr;
    suffixArr.reserve(N);
    for (int i = 0; i < N; i++){
        string rev = chains[i];
        reverse(rev.begin(), rev.end());
        suffixArr.push_back({rev, i});
    }
    sort(suffixArr.begin(), suffixArr.end(), PairStrCmp());
 
    // For each chain, record its position in suffixArr.
    vector<int> posSuffix(N);
    for (int i = 0; i < N; i++){
        int id = suffixArr[i].second;
        posSuffix[id] = i;
    }
 
    // Build array A: for each chain in prefix order, store its suffix position.
    vector<int> A(N);
    for (int i = 0; i < N; i++){
        int origId = prefixArr[i].second;
        A[i] = posSuffix[origId];
    }
 
    // For each query (P, Q), we want to count chains that satisfy:
    //   - Their string starts with P (prefix condition)
    //   - Their string ends with Q (suffix condition)
    // We note that “ends with Q” is equivalent to:
    //   the reversed chain starts with reverse(Q).
    // Hence, if we let:
    //    [L1, R1) be the indices in prefixArr with S in [P, P + '{')
    //    [L2, R2) be the indices in suffixArr with reversed(S) in [rev(Q), rev(Q) + '{')
    // then the answer for the query is:
    //    count_{i in [L1, R1)} [A[i] in [L2, R2) ].
    // We will answer these 2D queries by an offline BIT.
 
    // Helper lambdas for binary search.
    auto getPrefixRange = [&](const string &P) -> pair<int,int> {
        string lowKey = P;
        string highKey = P + '{'; // '{' is greater than any of A, C, G, U.
        pair<string,int> lowPair = {lowKey, -1};
        pair<string,int> highPair = {highKey, -1};
        int L = int(lower_bound(prefixArr.begin(), prefixArr.end(), lowPair, PairStrCmp()) - prefixArr.begin());
        int R = int(lower_bound(prefixArr.begin(), prefixArr.end(), highPair, PairStrCmp()) - prefixArr.begin());
        return {L, R};
    };
    auto getSuffixRange = [&](const string &Q) -> pair<int,int> {
        string Qrev = Q;
        reverse(Qrev.begin(), Qrev.end());
        string low = Qrev;
        string high = Qrev + '{';
        pair<string,int> lowPair = {low, -1};
        pair<string,int> highPair = {high, -1};
        int L = int(lower_bound(suffixArr.begin(), suffixArr.end(), lowPair, PairStrCmp()) - suffixArr.begin());
        int R = int(lower_bound(suffixArr.begin(), suffixArr.end(), highPair, PairStrCmp()) - suffixArr.begin());
        return {L, R};
    };
 
    // Read queries and compute the ranges.
    vector<int> qL1(M), qR1(M), qL2(M), qR2(M);
    for (int i = 0; i < M; i++){
        string P, Q;
        cin >> P >> Q;
        auto pr = getPrefixRange(P);
        auto sr = getSuffixRange(Q);
        qL1[i] = pr.first;  qR1[i] = pr.second;
        qL2[i] = sr.first;  qR2[i] = sr.second;
    }
 
    // We answer count_{i in [qL1, qR1)} of [A[i] in [qL2, qR2)).
    // To do so, we use offline BIT queries.
    // For each query we will eventually compute:
    //    result = F(qR1) - F(qL1)
    // where F(X) = count of indices in [0, X) with A[i] in [qL2, qR2).
    vector<Query> queriesR, queriesL;
    for (int i = 0; i < M; i++){
        if(qL1[i] >= qR1[i]) continue; // no chain has the given prefix.
        queriesR.push_back({qR1[i], qL2[i], qR2[i], i});
        queriesL.push_back({qL1[i], qL2[i], qR2[i], i});
    }
    auto cmpQuery = [](const Query &a, const Query &b) { return a.boundary < b.boundary; };
    sort(queriesR.begin(), queriesR.end(), cmpQuery);
    sort(queriesL.begin(), queriesL.end(), cmpQuery);
 
    // Process BIT queries on array A.
    Fenw fenw(N);
    vector<long long> resR(M, 0), resL(M, 0);
    int ptr = 0;
    // For each query with boundary q.boundary (which means we consider prefix indices [0, q.boundary))
    for (auto &q : queriesR) {
        while (ptr < q.boundary) {
            fenw.update(A[ptr], 1);
            ptr++;
        }
        // Count how many of the processed elements have suffix index in [q.L2, q.R2).
        int cnt = fenw.rangeQuery(q.L2, q.R2 - 1);
        resR[q.qid] = cnt;
    }
 
    // Reset BIT and process queries for the left bound.
    fenw = Fenw(N);
    ptr = 0;
    for (auto &q : queriesL) {
        while (ptr < q.boundary) {
            fenw.update(A[ptr], 1);
            ptr++;
        }
        int cnt = fenw.rangeQuery(q.L2, q.R2 - 1);
        resL[q.qid] = cnt;
    }
 
    // Final answer for each query = resR - resL.
    for (int i = 0; i < M; i++){
        long long answer = resR[i] - resL[i];
        cout << answer << "\n";
    }
 
    return 0;
}
