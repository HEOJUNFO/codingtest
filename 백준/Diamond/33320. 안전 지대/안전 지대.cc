#include <bits/stdc++.h>
using namespace std;
 
//---------------- DSU ----------------
struct DSU {
    vector<int> parent;
    DSU(int n) : parent(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int a) {
        if (parent[a] == a) return a;
        return parent[a] = find(parent[a]);
    }
    void unionn(int a, int b) {
        a = find(a), b = find(b);
        if(a != b)
            parent[b] = a;
    }
};
 
//---------------- Rectangle structure ----------------
// Each military base i manages a rectangle with 
// x in [A[i], C[i]] and y in [B[i], D[i]].
struct Rect {
    int idx;
    long long A, B, C, D;
};
 
//---------------- Active group structure ----------------
// While sweeping the x–axis we maintain groups (merged active intervals)
// Each group stores:
//   min_y, max_y : union (merged) y–interval
//   expire : the maximum C value among rectangles in the group (i.e. until which x some rectangle remains active)
//   rep : a representative index (used for DSU union)
struct Group {
    long long min_y, max_y, expire;
    int rep;
};
 
// Order groups by min_y; tie break by rep.
struct GroupCmp {
    bool operator()(const Group &a, const Group &b) const {
        if (a.min_y == b.min_y)
            return a.rep < b.rep;
        return a.min_y < b.min_y;
    }
};
 
//---------------- Main function ----------------
/*
  The function find_union takes N and the four arrays (A, B, C, D).
  It returns a vector<int> U of length N such that:
    - U[i] is the union (component) number (in 0..M–1)
    - Rectangles with the same union number are connected (their safe–zones intersect)
*/
vector<int> find_union(int N, vector<int> A, vector<int> B, vector<int> C, vector<int> D) {
    vector<Rect> rects(N);
    for (int i = 0; i < N; i++) {
        rects[i] = { i, (long long)A[i], (long long)B[i], (long long)C[i], (long long)D[i] };
    }
    // Sort rectangles by left x (A) ascending.
    sort(rects.begin(), rects.end(), [](const Rect &r1, const Rect &r2) {
        return r1.A < r2.A;
    });
 
    DSU dsu(N);
    // active groups maintained in a balanced BST (ordered by min_y)
    set<Group, GroupCmp> active;
 
    // Sweep over rectangles in order of increasing A.
    for (auto &r : rects) {
        long long curX = r.A;
        // Remove any active group whose expire (max C in group) is less than curX.
        while(!active.empty()){
            auto it = active.begin();
            if(it->expire < curX)
                active.erase(it);
            else break;
        }
 
        // Start a new group corresponding to rectangle r.
        Group newGroup;
        newGroup.min_y = r.B;
        newGroup.max_y = r.D;
        newGroup.expire = r.C;
        newGroup.rep = r.idx;
 
        // Collect all groups in 'active' that intersect [r.B, r.D].
        // (Two intervals [p,q] and [x,y] intersect if q>=x and y>=p.)
        vector<set<Group,GroupCmp>::iterator> toMerge;
 
        // In the set ordered by min_y, start at lower_bound(newGroup with min_y = r.B)
        auto it = active.lower_bound(Group{r.B,0,0,0});
        // Check the group immediately before it (if any)
        if(it != active.begin()){
            auto it2 = prev(it);
            if(it2->max_y >= newGroup.min_y)
                toMerge.push_back(it2);
        }
        // Now check forward from it while group.min_y <= newGroup.max_y.
        for(; it != active.end(); it++){
            if(it->min_y > newGroup.max_y) break;
            if(it->max_y >= newGroup.min_y)
                toMerge.push_back(it);
        }
 
        // Merge all intersecting groups into newGroup.
        for(auto &itr : toMerge) {
            const Group &g = *itr;
            newGroup.min_y = min(newGroup.min_y, g.min_y);
            newGroup.max_y = max(newGroup.max_y, g.max_y);
            newGroup.expire = max(newGroup.expire, g.expire);
            dsu.unionn(newGroup.rep, g.rep);
        }
        // Erase the merged groups from the active set.
        for(auto &itr : toMerge) {
            active.erase(itr);
        }
        // Union the new rectangle r with the new group’s rep.
        dsu.unionn(newGroup.rep, r.idx);
 
        // Insert the new merged group.
        active.insert(newGroup);
    }
 
    // Now, the DSU structure has partitioned the rectangles into connected components.
    // Assign new labels (0...M-1) to each unique DSU representative.
    unordered_map<int,int> compLabel;
    compLabel.reserve(N * 2);
    vector<int> U(N, 0);
    int label = 0;
    for (int i = 0; i < N; i++){
        int root = dsu.find(i);
        if(compLabel.find(root) == compLabel.end()){
            compLabel[root] = label++;
        }
        U[i] = compLabel[root];
    }
    return U;
}
 
/*
// For local testing (remove main() when using as a library)
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N; cin >> N;
    vector<int> A(N), B(N), C(N), D(N);
    for(int i=0;i<N;i++){
        cin >> A[i] >> B[i] >> C[i] >> D[i];
    }
    vector<int> ans = find_union(N, A, B, C, D);
    for(auto x : ans) cout << x << " ";
    cout << "\n";
    return 0;
}
*/
 
// This is the complete solution.
