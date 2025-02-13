#include <bits/stdc++.h>
using namespace std;
 
struct Node {
    int nxt[26];   // transitions for each letter 'a'..'z'
    int fail;      // failure link in the Aho–Corasick automaton
    int cnt;       // number of menus that have this prefix (popularity)
    int depth;     // length of the prefix represented by this node
    long long maxA; // maximum A-value among occurrences of this prefix as a substring
    Node() {
        memset(nxt, -1, sizeof(nxt));
        fail = 0;
        cnt = 0;
        depth = 0;
        maxA = 0;
    }
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
    while(T--){
        int N;
        cin >> N;
        vector<string> menus(N);
        for (int i = 0; i < N; i++){
            cin >> menus[i];
        }
        // Read joy arrays corresponding to each menu.
        vector<vector<long long>> joys(N);
        for (int i = 0; i < N; i++){
            int len = menus[i].size();
            joys[i].resize(len);
            for (int j = 0; j < len; j++){
                cin >> joys[i][j];
            }
        }
 
        // --- Build the trie over all menus ---
        // (each menu insertion automatically inserts all its prefixes)
        vector<Node> trie;
        trie.push_back(Node()); // root at index 0
        for (int i = 0; i < N; i++){
            int cur = 0;
            for (char ch : menus[i]) {
                int c = ch - 'a';
                if(trie[cur].nxt[c] == -1){
                    trie[cur].nxt[c] = trie.size();
                    Node newNode;
                    newNode.depth = trie[cur].depth + 1;
                    trie.push_back(newNode);
                }
                cur = trie[cur].nxt[c];
                // Each menu contributes once for every prefix it passes.
                trie[cur].cnt++; 
            }
        }
 
        // --- Build failure links (Aho–Corasick) ---
        queue<int> q;
        trie[0].fail = 0;
        for (int c = 0; c < 26; c++){
            int nxt = trie[0].nxt[c];
            if(nxt != -1){
                trie[nxt].fail = 0;
                q.push(nxt);
            } else {
                trie[0].nxt[c] = 0; // missing transitions point back to root
            }
        }
        while(!q.empty()){
            int cur = q.front();
            q.pop();
            int f = trie[cur].fail;
            // (Do NOT propagate cnt here – popularity is the direct count.)
            for (int c = 0; c < 26; c++){
                int nxt = trie[cur].nxt[c];
                if(nxt != -1){
                    trie[nxt].fail = trie[f].nxt[c];
                    q.push(nxt);
                } else {
                    trie[cur].nxt[c] = trie[f].nxt[c];
                }
            }
        }
 
        // --- Process each menu as text ---
        // For each menu we run the automaton and update the current state's maxA
        for (int i = 0; i < (int)trie.size(); i++){
            trie[i].maxA = 0;
        }
        for (int i = 0; i < N; i++){
            int state = 0;
            for (int pos = 0; pos < (int)menus[i].size(); pos++){
                int c = menus[i][pos] - 'a';
                state = trie[state].nxt[c]; // due to failure links, this always succeeds
                // Record the joy value for an occurrence ending at position pos
                trie[state].maxA = max(trie[state].maxA, joys[i][pos]);
            }
        }
 
        // --- Propagate the maxA values along the failure links ---
        // Process nodes in descending order of depth so that a node’s maxA is pushed to its suffixes.
        vector<int> order(trie.size());
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b){
            return trie[a].depth > trie[b].depth;
        });
        for (int idx : order){
            int f = trie[idx].fail;
            trie[f].maxA = max(trie[f].maxA, trie[idx].maxA);
        }
 
        // --- Compute the best quality ---
        // For every candidate prefix (each node), quality = (popularity) * (length) * (maxA)
        long long ans = 0;
        for (int i = 0; i < (int)trie.size(); i++){
            long long candidate = (long long) trie[i].cnt * trie[i].depth * trie[i].maxA;
            ans = max(ans, candidate);
        }
 
        cout << ans << "\n";
    }
    return 0;
}
