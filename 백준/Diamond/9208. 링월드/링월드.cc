#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

struct Interval {
    int64 l, r;
    bool operator<(const Interval &other) const {
        if (r != other.r) return r < other.r;
        return l < other.l;
    }
};

// successor‑DSU with hash map
class DSU {
    unordered_map<int64,int64> parent;
public:
    // find first free position >= x
    int64 find(int64 x) {
        auto it = parent.find(x);
        if (it==parent.end()) return x;
        return it->second = find(it->second);
    }
    // mark x as used
    void occupy(int64 x) {
        parent[x] = find(x+1);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; if(!(cin>>T)) return 0;
    while(T--){
        int64 m; int n; cin>>m>>n;
        vector<Interval> vec; vec.reserve(2*n);
        for(int i=0;i<n;++i){
            int64 x,y; cin>>x>>y;
            if(x<=y){
                vec.push_back({x,y});
                vec.push_back({x+m,y+m});
            }else{
                vec.push_back({x,y+m});
            }
        }
        if(n>m){
            cout<<"NO\n";
            continue;
        }
        sort(vec.begin(), vec.end());
        DSU dsu; bool ok=true; int processed=0;
        for(auto &iv: vec){
            int64 pos = dsu.find(iv.l);
            if(pos>iv.r){ ok=false; break; }
            dsu.occupy(pos);
            ++processed;
            if(processed==n){
                // We only need to assign n intervals; once we succeed for the first n processed
                // original intervals might be duplicated, but the theorem guarantees overall success.
            }
        }
        cout<<(ok?"YES":"NO")<<"\n";
    }
    return 0;
}