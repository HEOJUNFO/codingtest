#include <bits/stdc++.h>
using namespace std;
 
// Implicit Treap node definition.
struct Node {
    int val;      // sample height
    int id;       // original index (for tie-break in stable order)
    int priority; // random priority for treap balancing
    int size;     // size of the subtree
    bool rev;     // lazy reversal flag
    Node *left, *right, *parent;
    Node(int v, int id) : val(v), id(id), priority(rand()), size(1), rev(false),
                           left(nullptr), right(nullptr), parent(nullptr) {}
};
 
// Returns the size of the subtree (or 0 if null).
int getSize(Node* t) {
    return t ? t->size : 0;
}
 
// Updates the size field and fixes parent pointers.
void update(Node* t) {
    if(t) {
        t->size = getSize(t->left) + getSize(t->right) + 1;
        if(t->left)  t->left->parent  = t;
        if(t->right) t->right->parent = t;
    }
}
 
// Propagates the lazy reversal flag downwards.
void push_down(Node* t) {
    if(t && t->rev) {
        swap(t->left, t->right);
        if(t->left)  t->left->rev ^= true;
        if(t->right) t->right->rev ^= true;
        t->rev = false;
    }
}
 
// Splits the treap 't' into 'left' and 'right' parts,
// where 'left' contains the first 'key' nodes.
void split(Node* t, int key, Node*& left, Node*& right) {
    if(!t) { 
        left = right = nullptr; 
        return; 
    }
    push_down(t);
    int leftSize = getSize(t->left);
    if(key <= leftSize) {
        split(t->left, key, left, t->left);
        right = t;
        update(right);
    } else {
        split(t->right, key - leftSize - 1, t->right, right);
        left = t;
        update(left);
    }
}
 
// Merges two treaps 'left' and 'right' and returns the new root.
Node* merge(Node* left, Node* right) {
    if(!left || !right) 
        return left ? left : right;
    push_down(left);
    push_down(right);
    if(left->priority > right->priority) {
        left->right = merge(left->right, right);
        update(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        update(right);
        return right;
    }
}
 
// Computes the 1-indexed position of a given node in the treap.
// To be sure of the correct answer, we push down lazy flags along the node’s path.
int get_index(Node* node) {
    vector<Node*> path;
    for(Node* cur = node; cur; cur = cur->parent)
        path.push_back(cur);
    reverse(path.begin(), path.end());
    for(Node* n : path)
        push_down(n);
    int pos = getSize(node->left) + 1;
    while(node->parent) {
        if(node == node->parent->right)
            pos += getSize(node->parent->left) + 1;
        node = node->parent;
    }
    return pos;
}
 
// Recursively free the treap memory.
void freeTreap(Node* t) {
    if(!t) return;
    freeTreap(t->left);
    freeTreap(t->right);
    delete t;
}
 
// Main solution function.
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    while(true) {
        int N;
        cin >> N;
        if(N == 0) break;
 
        vector<int> samples(N);
        for (int i = 0; i < N; i++){
            cin >> samples[i];
        }
 
        // Build the initial treap from the input order.
        vector<Node*> nodes(N);
        Node* root = nullptr;
        for (int i = 0; i < N; i++){
            nodes[i] = new Node(samples[i], i); // id is the original index
            root = merge(root, nodes[i]);
        }
 
        // Create a sorted order (stable sort by height, then by original index)
        vector<Node*> sortedNodes = nodes;
        sort(sortedNodes.begin(), sortedNodes.end(), [](Node* a, Node* b){
            if(a->val == b->val)
                return a->id < b->id;
            return a->val < b->val;
        });
 
        vector<int> ans(N, 0);
 
        // For each i from 1 to N, determine the current position of the i-th smallest sample
        // and then reverse the segment from position i to that position.
        for (int i = 1; i <= N; i++){
            Node* target = sortedNodes[i-1];
            int pos = get_index(target);
            ans[i-1] = pos;
 
            // Reverse the subarray [i, pos]:
            // 1. Split off the first (i-1) fixed nodes.
            // 2. Then split off the segment [i, pos].
            // 3. Toggle the reversal flag for that segment.
            // 4. Merge everything back.
            Node *left = nullptr, *mid = nullptr, *right = nullptr;
            split(root, i-1, left, mid);
            split(mid, pos - (i - 1), mid, right);
            if(mid)
                mid->rev ^= true;
            root = merge(left, merge(mid, right));
        }
 
        // Output the reversal positions.
        for (int i = 0; i < N; i++){
            cout << ans[i] << (i == N - 1 ? "\n" : " ");
        }
 
        freeTreap(root);
    }
    return 0;
}
