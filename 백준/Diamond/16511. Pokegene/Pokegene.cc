#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;
 
const int ALPH = 26;
 
struct TrieNode {
    int nxt[ALPH];
    int parent;
    char c;
    vector<int> children;
    TrieNode(){ memset(nxt, -1, sizeof(nxt)); parent = -1; c = 0; }
};
 
vector<TrieNode> trie;
 
int insertTrie(const string &s) {
    int cur = 0;
    for(char ch : s){
        int c = ch - 'a';
        if(trie[cur].nxt[c] == -1){
            trie[cur].nxt[c] = trie.size();
            TrieNode node;
            node.parent = cur;
            node.c = ch;
            trie.push_back(node);
            trie[cur].children.push_back(trie.size()-1);
        }
        cur = trie[cur].nxt[c];
    }
    return cur;
}
 
const int NMAX = 500000;
int sz[NMAX], par[NMAX], dep[NMAX], heavy[NMAX];
int head[NMAX], pos[NMAX];
int curPos = 0;
 
void dfs(int v) {
    sz[v] = 1; heavy[v] = -1;
    for (int u : trie[v].children) {
        dep[u] = dep[v] + 1; par[u] = v;
        dfs(u);
        if(heavy[v] == -1 || sz[u] > sz[heavy[v]])
            heavy[v] = u;
        sz[v] += sz[u];
    }
}
 
void decompose(int v, int h) {
    head[v] = h; pos[v] = curPos++;
    if(heavy[v] != -1) decompose(heavy[v], h);
    for (int u : trie[v].children) {
        if(u == heavy[v]) continue;
        decompose(u, u);
    }
}
 
struct SegNode {
    int l, r, lazy, val, cnt;
    bool uniform;
};
 
vector<SegNode> seg;
int segSize;
 
void seg_init(int n) {
    segSize = 1; while(segSize < n) segSize *= 2;
    seg.assign(segSize * 2, {0,0,0,true,0,0});
    for (int i = 0; i < segSize; i++) {
        int idx = segSize + i;
        seg[idx].l = seg[idx].r = i;
        seg[idx].lazy = 0; seg[idx].uniform = true; seg[idx].val = 0;
        seg[idx].cnt = (i < n ? 1 : 0);
    }
    for (int i = segSize - 1; i >= 1; i--) {
        seg[i].l = seg[i*2].l;
        seg[i].r = seg[i*2+1].r;
        seg[i].lazy = 0;
        if(seg[i*2].uniform && seg[i*2+1].uniform && seg[i*2].val == seg[i*2+1].val) {
            seg[i].uniform = true; seg[i].val = seg[i*2].val;
        } else {
            seg[i].uniform = false;
        }
        seg[i].cnt = seg[i*2].cnt + seg[i*2+1].cnt;
    }
}
 
inline void pushDown(int idx) {
    if(seg[idx].lazy) {
        int addVal = seg[idx].lazy;
        for (int child = idx * 2; child < idx * 2 + 2; child++) {
            if(child >= (int)seg.size()) break;
            seg[child].lazy += addVal;
            if(seg[child].uniform)
                seg[child].val += addVal;
        }
        seg[idx].lazy = 0;
    }
}
 
inline void seg_update(int idx, int l, int r, int delta) {
    if(seg[idx].r < l || seg[idx].l > r) return;
    if(l <= seg[idx].l && seg[idx].r <= r) {
        seg[idx].lazy += delta;
        if(seg[idx].uniform)
            seg[idx].val += delta;
        return;
    }
    pushDown(idx);
    seg_update(idx * 2, l, r, delta);
    seg_update(idx * 2 + 1, l, r, delta);
    if(seg[idx * 2].uniform && seg[idx * 2 + 1].uniform && seg[idx * 2].val == seg[idx * 2 + 1].val) {
        seg[idx].uniform = true; seg[idx].val = seg[idx * 2].val;
    } else {
        seg[idx].uniform = false;
    }
}
 
inline int seg_query(int idx, int l, int r, int X) {
    if(seg[idx].r < l || seg[idx].l > r) return 0;
    if(l <= seg[idx].l && seg[idx].r <= r && seg[idx].uniform)
        return (seg[idx].val == X ? seg[idx].cnt : 0);
    pushDown(idx);
    return seg_query(idx * 2, l, r, X) + seg_query(idx * 2 + 1, l, r, X);
}
 
void path_update(int u, int delta) {
    while(true) {
        int h = head[u];
        seg_update(1, pos[h], pos[u], delta);
        if(h == 0) break;
        u = par[h];
    }
}
 
int full_query(int X, int totalNodes) {
    return seg_query(1, 1, totalNodes - 1, X);
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, Q; 
    cin >> N >> Q;
    trie.reserve(250000);
    trie.push_back(TrieNode());
    vector<int> endpoint(N);
    vector<string> genomes(N);
    for (int i = 0; i < N; i++){
        cin >> genomes[i];
        endpoint[i] = insertTrie(genomes[i]);
    }
    dep[0] = 0; par[0] = -1;
    dfs(0);
    curPos = 0; decompose(0, 0);
    int totalNodes = curPos;
    seg_init(totalNodes);
 
    for (int qi = 0; qi < Q; qi++){
        int K, Lval; 
        cin >> K >> Lval;
        vector<int> idxs(K);
        for (int i = 0; i < K; i++){
            cin >> idxs[i]; 
            idxs[i]--;
        }
        for (int i = 0; i < K; i++){
            int u = endpoint[idxs[i]];
            path_update(u, 1);
        }
        int ans = full_query(Lval, totalNodes);
        cout << ans << "\n";
        for (int i = 0; i < K; i++){
            int u = endpoint[idxs[i]];
            path_update(u, -1);
        }
    }
    return 0;
}
