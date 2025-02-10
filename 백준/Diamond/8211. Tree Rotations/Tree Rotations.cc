#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

// Ordered set that supports order_of_key (which returns the number of items strictly less than a given key)
template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// Structure to hold the DP result for a subtree.
struct DPNode {
    long long dp;              // minimal inversion count in this subtree
    ordered_set<int>* os;      // container holding all leaf labels in sorted order
};

// Recursively read the tree and compute the DP result.
// The tree description: if the next number is nonzero, it is a leaf with that label.
// If it is 0, then it represents an internal node with a bifurcation; read left subtree then right subtree.
DPNode solve() {
    int x;
    cin >> x;
    DPNode node;
    if(x != 0) { // leaf node
        node.dp = 0;
        node.os = new ordered_set<int>();
        node.os->insert(x);
        return node;
    } else {
        // Internal node: first process left and right subtrees.
        DPNode leftNode = solve();
        DPNode rightNode = solve();
        
        int sizeL = leftNode.os->size();
        int sizeR = rightNode.os->size();
        long long cross = 0;
        // Compute f(S_L, S_R): for each element in the left subtree, count how many elements in the right subtree are less.
        // To be efficient, iterate over the smaller container.
        if(sizeL <= sizeR) {
            for(auto a : *(leftNode.os)) {
                cross += rightNode.os->order_of_key(a);
            }
        } else {
            // Alternatively, for each b in the right subtree, count how many in the left subtree are greater than b.
            for(auto b : *(rightNode.os)) {
                cross += sizeL - leftNode.os->order_of_key(b+1);
            }
        }
        long long totalPairs = (long long) sizeL * sizeR;
        long long bestCross = min(cross, totalPairs - cross);
        long long dp_here = leftNode.dp + rightNode.dp + bestCross;
 
        // Merge the two containers S_L and S_R into one.
        // Use small-to-large merging: merge the smaller set into the larger one.
        ordered_set<int>* merged;
        ordered_set<int>* small;
        ordered_set<int>* large;
        if(sizeL < sizeR) {
            merged = rightNode.os;
            small = leftNode.os;
            large = rightNode.os;
        } else {
            merged = leftNode.os;
            small = rightNode.os;
            large = leftNode.os;
        }
        for(auto val : *small) {
            large->insert(val);
        }
        delete small;  // free memory for the smaller container
 
        node.dp = dp_here;
        node.os = merged;
        return node;
    }
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n; // number of leaves
    DPNode root = solve();
    cout << root.dp << "\n";
    delete root.os; // free the memory for the final container
    return 0;
}
