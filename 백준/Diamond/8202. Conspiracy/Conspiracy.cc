#include <bits/stdc++.h>
using namespace std;
 
// Maximum number of vertices
const int MAXN = 5000;
 
// We'll store the neighbor–set for each vertex in a bitset.
vector<bitset<MAXN>> neigh;
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n;
    cin >> n;
    neigh.resize(n);
    for (int i = 0; i < n; i++){
        neigh[i].reset();
    }
 
    // Read input.
    // Each of the next n lines starts with an integer k (the number of acquaintances),
    // then k numbers (the acquaintances, 1-indexed).
    long long totalEdges = 0;
    for (int i = 0; i < n; i++){
        int k;
        cin >> k;
        for (int j = 0; j < k; j++){
            int nb;
            cin >> nb;
            nb--; // convert to 0-indexed
            // Set both directions (the graph is undirected).
            neigh[i].set(nb, true);
            neigh[nb].set(i, true);
            totalEdges++;
        }
    }
    // Each edge is read twice; so the actual number of distinct edges is:
    totalEdges /= 2;
 
    // If the graph is complete then each vertex has (n-1) neighbors,
    // and the number of distinct edges is n*(n-1)/2.
    if(totalEdges == (long long)n*(n-1)/2){
        cout << n << "\n";
        return 0;
    }
    // If the graph is edgeless, answer is also n.
    if(totalEdges == 0){
        cout << n << "\n";
        return 0;
    }
 
    // Compute degree of each vertex.
    vector<int> deg(n, 0);
    for (int i = 0; i < n; i++){
        deg[i] = (int)neigh[i].count();
    }
 
    // --- Compute a split partition.
    // We want to partition V = A ∪ B such that A is a clique and B is independent.
    // We use a greedy algorithm:
    // Process the vertices in non-increasing order of degree.
    vector<int> order(n);
    for (int i = 0; i < n; i++){
        order[i] = i;
    }
    sort(order.begin(), order.end(), [&](int a, int b){
        if(deg[a] == deg[b]) return a < b;
        return deg[a] > deg[b];
    });
 
    vector<int> A, B;
    for (int v : order) {
        bool canJoinA = true;
        for (int u : A) {
            if(!neigh[v].test(u)) { // if v is not adjacent to some u already in A
                canJoinA = false;
                break;
            }
        }
        if(canJoinA) {
            A.push_back(v);
        } else {
            B.push_back(v);
        }
    }
    // We require both groups to be nonempty.
    // If B ended up empty (this happens if the graph is complete, but then we already handled that case),
    // force one vertex from A into B.
    if(B.empty()){
        int v = A.back();
        A.pop_back();
        B.push_back(v);
    }
 
    // Now check that A is really a clique and B is independent.
    bool validPartition = true;
    for (size_t i = 0; i < A.size() && validPartition; i++){
        for (size_t j = i+1; j < A.size() && validPartition; j++){
            if(!neigh[A[i]].test(A[j])){
                validPartition = false;
                break;
            }
        }
    }
    for (size_t i = 0; i < B.size() && validPartition; i++){
        for (size_t j = i+1; j < B.size() && validPartition; j++){
            if(neigh[B[i]].test(B[j])){
                validPartition = false;
                break;
            }
        }
    }
    if(!validPartition){
        cout << 0 << "\n";
        return 0;
    }
 
    // --- Now we have a split partition P0 = (A, B).
    // (In non‐extreme cases this partition is unique.)
    //
    // We now compute the numbers defined above.
    //
    // We first create bit–masks for A and B.
    bitset<MAXN> A_mask, B_mask;
    A_mask.reset(); B_mask.reset();
    for (int v : A) A_mask.set(v, true);
    for (int v : B) B_mask.set(v, true);
 
    // 1. Count the canonical partition P0 (count = 1)
    int countP0 = 1;
 
    // 2. Compute W = { a in A : for all b in B, (a,b) is false }.
    vector<int> W;
    for (int v : A){
        bitset<MAXN> inter = neigh[v] & B_mask;
        if(inter.none()){
            W.push_back(v);
        }
    }
    int countAtoB = (int) W.size(); // each gives a new partition by moving a from A to B.
 
    // 3. Compute U = { b in B : for all a in A, (b,a) is true }.
    int countBtoA = 0;
    if(B.size() >= 2){
        for (int v : B){
            bitset<MAXN> temp = neigh[v] & A_mask;
            if(temp == A_mask)
                countBtoA++;
        }
    }
 
    // 4. For each a in W, we want to count
    //    U'_a = { b in B : for every x in A with x != a, (b,x) is true }.
    // For each such pair (a, b) we get a new partition by doing a two–swap.
    int countTwoSwap = 0;
    for (int a : W){
        bitset<MAXN> A_except = A_mask;
        A_except.reset(a);
        for (int b : B){
            bitset<MAXN> testBS = neigh[b] & A_except;
            if(testBS == A_except)
                countTwoSwap++;
        }
    }
 
    int totalCount = countP0 + countAtoB + countBtoA + countTwoSwap;
 
    // Output the answer.
    cout << totalCount << "\n";
 
    return 0;
}
