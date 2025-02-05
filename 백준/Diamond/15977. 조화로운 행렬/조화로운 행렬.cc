#include <bits/stdc++.h>
using namespace std;
 
// -------------------
// M = 2 인 경우
// -------------------
// 2차원 점 (a,b) 에 대해, a로 정렬한 후 b에 대해 
// Longest Increasing Subsequence (LIS)를 구하는 함수.
 
int solve2D_increasing(const vector<pair<long long, long long>> &points) {
    vector<pair<long long,long long>> pts = points;
    sort(pts.begin(), pts.end(), [](const pair<long long,long long> &p1, const pair<long long,long long> &p2){
        if(p1.first == p2.first)
            return p1.second < p2.second;
        return p1.first < p2.first;
    });
    vector<long long> lis;
    for(auto &p : pts) {
        long long val = p.second;
        // strictly increasing : lower_bound suffices (모두 서로 다름)
        auto it = std::lower_bound(lis.begin(), lis.end(), val);
        if(it == lis.end()){
            lis.push_back(val);
        } else {
            *it = val;
        }
    }
    return lis.size();
}
 
// -------------------
// M = 3 인 경우
// -------------------
// 3차원 점을 위한 구조체
struct Point3 {
    long long a, b, c;
    int pb, pc; // b, c 좌표의 좌표압축 후 값
};
 
// 3차원 점들에 대해, a에 대해 정렬한 후 (b, c)가 모두 strictly 증가하는 체인을 
// 2차원 BIT (Fenwick Tree) 를 사용하여 구하는 함수.
int solve3D_increasing(vector<Point3> pts) {
    // a 오름차순, b, c 오름차순으로 정렬
    sort(pts.begin(), pts.end(), [](const Point3 &p1, const Point3 &p2) {
        if(p1.a != p2.a) return p1.a < p2.a;
        if(p1.b != p2.b) return p1.b < p2.b;
        return p1.c < p2.c;
    });
    int n = pts.size();
    // b, c 좌표 압축
    vector<long long> allB, allC;
    allB.reserve(n);
    allC.reserve(n);
    for(auto &p : pts) {
        allB.push_back(p.b);
        allC.push_back(p.c);
    }
    sort(allB.begin(), allB.end());
    allB.erase(unique(allB.begin(), allB.end()), allB.end());
    sort(allC.begin(), allC.end());
    allC.erase(unique(allC.begin(), allC.end()), allC.end());
    for(auto &p : pts) {
        p.pb = int(lower_bound(allB.begin(), allB.end(), p.b) - allB.begin()) + 1;
        p.pc = int(lower_bound(allC.begin(), allC.end(), p.c) - allC.begin()) + 1;
    }
    int sizeB = allB.size();
    // 2차원 BIT: 외부 인덱스는 b(1~sizeB), 내부는 c에 대한 BIT.
    // BITtree[i]에는 BIT의 노드 i가 관리할 c 좌표들이 들어있다.
    vector<vector<int>> BITtree(sizeB+1);
    vector<vector<int>> BITval(sizeB+1);  // 각 노드별 BIT (최댓값 관리)
 
    // 각 점의 pb에 대하여, 해당 점의 pc 를 BIT의 여러 노드에 추가.
    for(auto &p : pts) {
        int x = p.pb;
        for (int i = x; i <= sizeB; i += i & -i) {
            BITtree[i].push_back(p.pc);
        }
    }
    // 각 BIT 노드별로 정렬 및 중복 제거, 그리고 BITval 배열 초기화 (1-indexed)
    for (int i = 1; i <= sizeB; i++) {
        sort(BITtree[i].begin(), BITtree[i].end());
        BITtree[i].erase(unique(BITtree[i].begin(), BITtree[i].end()), BITtree[i].end());
        BITval[i].assign(BITtree[i].size()+1, 0);
    }
 
    // BIT query : bIndex 이하의 BIT 노드들에서, c 값이 cVal 이하인 값들의 최댓값을 구한다.
    auto queryBIT = [&](int bIndex, int cVal) -> int {
        int res = 0;
        for (int i = bIndex; i > 0; i -= i & -i) {
            int pos = int(upper_bound(BITtree[i].begin(), BITtree[i].end(), cVal) - BITtree[i].begin());
            int cur = 0;
            for (int j = pos; j > 0; j -= j & -j) {
                cur = max(cur, BITval[i][j]);
            }
            res = max(res, cur);
        }
        return res;
    };
 
    // BIT update : bIndex 이상의 BIT 노드들에서, cVal 위치에 value값을 갱신.
    auto updateBIT = [&](int bIndex, int cVal, int value) {
        for (int i = bIndex; i <= sizeB; i += i & -i) {
            int pos = int(lower_bound(BITtree[i].begin(), BITtree[i].end(), cVal) - BITtree[i].begin()) + 1;
            for (int j = pos; j < (int)BITval[i].size(); j += j & -j) {
                BITval[i][j] = max(BITval[i][j], value);
            }
        }
    };
 
    int best = 0;
    // DP 진행 : 각 점에 대해, 이전 점들 중 (b, c) 모두 작아야 하므로 query(pb-1, pc-1)를 하여 dp = prev_max + 1.
    for(auto &p : pts) {
        int curVal = queryBIT(p.pb - 1, p.pc - 1) + 1;
        updateBIT(p.pb, p.pc, curVal);
        best = max(best, curVal);
    }
    return best;
}
 
// -------------------
// main()
// -------------------
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int M, N;
    cin >> M >> N;
    vector<vector<long long>> Q(M, vector<long long>(N));
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            cin >> Q[i][j];
        }
    }
 
    int ans = 0;
    // 아이디어: 열들이 서로 “comparable” 하다는 것은
    // 열들을 M차원 점으로 보았을 때, 전좌표에 대해 모두 strictly increasing 하거나 strictly decreasing한 체인임.
    // 감소하는 체인은 각 좌표에 음수를 취하면 increasing chain이 된다.
    if(M == 2){
        vector<pair<long long, long long>> pts, ptsNeg;
        pts.resize(N);
        ptsNeg.resize(N);
        for(int j = 0; j < N; j++){
            pts[j] = { Q[0][j], Q[1][j] };
            ptsNeg[j] = { -Q[0][j], -Q[1][j] };
        }
        int lisInc = solve2D_increasing(pts);
        int lisDec = solve2D_increasing(ptsNeg);
        ans = max(lisInc, lisDec);
    } else if(M == 3){
        vector<Point3> pts, ptsNeg;
        pts.resize(N);
        ptsNeg.resize(N);
        for (int j = 0; j < N; j++){
            pts[j] = { Q[0][j], Q[1][j], Q[2][j], 0, 0 };
            ptsNeg[j] = { -Q[0][j], -Q[1][j], -Q[2][j], 0, 0 };
        }
        int lisInc = solve3D_increasing(pts);
        int lisDec = solve3D_increasing(ptsNeg);
        ans = max(lisInc, lisDec);
    }
 
    cout << ans << "\n";
    return 0;
}
