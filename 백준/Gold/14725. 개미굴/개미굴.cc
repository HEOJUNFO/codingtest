#include <bits/stdc++.h>
using namespace std;

struct Trie {
    map<string, Trie*> children;
};

void insertTrie(Trie* root, const vector<string>& v) {
    Trie* cur = root;
    for (auto &food : v) {
        if (cur->children.find(food) == cur->children.end()) {
            cur->children[food] = new Trie();
        }
        cur = cur->children[food];
    }
}

void printTrie(Trie* root, int depth) {
    for (auto &child : root->children) {
        for (int i = 0; i < depth; i++) cout << "--";
        cout << child.first << "\n";
        printTrie(child.second, depth + 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N; cin >> N;
    Trie *root = new Trie();

    for (int i = 0; i < N; i++) {
        int K; cin >> K;
        vector<string> info(K);
        for (int j = 0; j < K; j++) cin >> info[j];
        insertTrie(root, info);
    }

    printTrie(root, 0);
    return 0;
}
