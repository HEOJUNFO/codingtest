#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
const ll MINF = -LLONG_MAX/4;
 
// 회사 정보를 저장 (회사 함수: f(x)=A+B*x)
struct Company {
    bool active;
    ll A, B; // A = S - T*Z, B = Z
};
 
// 선형함수를 나타내는 구조체 (f(x)=m*x+c)와 해당 선이 유효해지는 시작 x값
struct Line {
    ll m, c;
    double start; // 이 선이 최적이 되는 구간 시작 x값
};
 
// 블록 구조체: [L, R) 범위의 인덱스를 커버하며, dirty이면 재구축 필요
struct Block {
    int L, R;
    bool dirty;
    vector<Line> hull; // 이 블록의 활성 선들의 볼록 껍질 (upper envelope)
};
 
int BLOCK_SIZE;
vector<Company> comp;
vector<Block> blocks;
 
// 두 선의 교차 x좌표를 구함
inline double intersectX(const Line &l1, const Line &l2) {
    return double(l1.c - l2.c) / double(l2.m - l1.m);
}
 
// 블록 bi의 hull 재구축 (활성 선들만 사용)
void rebuildBlock(int bi) {
    Block &B = blocks[bi];
    vector<Line> lines;
    for (int i = B.L; i < B.R; i++) {
        if(comp[i].active) {
            Line L;
            L.m = comp[i].B;
            L.c = comp[i].A;
            lines.push_back(L);
        }
    }
    if(lines.empty()){
        B.hull.clear();
        B.dirty = false;
        return;
    }
    // 기울기 오름차순 정렬 (동일 기울기라면 c가 큰 것만 남김)
    sort(lines.begin(), lines.end(), [](const Line &l1, const Line &l2){
        if(l1.m == l2.m) return l1.c > l2.c;
        return l1.m < l2.m;
    });
    vector<Line> uniqueLines;
    uniqueLines.push_back(lines[0]);
    for (size_t i = 1; i < lines.size(); i++){
        if(lines[i].m == uniqueLines.back().m)
            continue; // 기울기가 같으면 이미 c가 더 큰 값으로 넣었으므로 패스
        uniqueLines.push_back(lines[i]);
    }
    // 볼록 껍질 구성 (upper envelope)
    vector<Line> hull;
    for (auto &ln : uniqueLines) {
        if(hull.empty()){
            ln.start = -1e18;
            hull.push_back(ln);
        } else {
            // 새 선 ln와 기존 선들의 교차점을 구해, 최적 구간을 조정
            while(!hull.empty()){
                double xInt = intersectX(hull.back(), ln);
                if(xInt <= hull.back().start) {
                    hull.pop_back();
                    if(hull.empty()){
                        ln.start = -1e18;
                        break;
                    }
                } else {
                    ln.start = xInt;
                    break;
                }
            }
            hull.push_back(ln);
        }
    }
    B.hull = move(hull);
    B.dirty = false;
}
 
// 완전 포함된 블록의 hull에서, 주어진 T에 대해 최대 f(T)값 반환
inline ll queryBlockHull(int bi, ll T) {
    Block &B = blocks[bi];
    if(B.hull.empty()) return MINF;
    int lo = 0, hi = (int)B.hull.size()-1, ansIdx = 0;
    while(lo <= hi){
        int mid = (lo + hi) / 2;
        if(B.hull[mid].start <= T) {
            ansIdx = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return B.hull[ansIdx].m * T + B.hull[ansIdx].c;
}
 
// 블록의 일부 구간 [qL, qR)에서 직접 순회하며 최대값 계산
inline ll queryBlockPartial(int bi, int qL, int qR, ll T) {
    Block &B = blocks[bi];
    ll ans = MINF;
    int start = max(qL, B.L), end = min(qR, B.R);
    for (int i = start; i < end; i++){
        if(comp[i].active)
            ans = max(ans, comp[i].A + comp[i].B * T);
    }
    return ans;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    comp.resize(N);
    for (int i = 0; i < N; i++){
        comp[i].active = false;
        comp[i].A = MINF;
        comp[i].B = 0;
    }
 
    // 블록 크기를 sqrt(N) 정도로 결정 (최적의 상수 계수)
    BLOCK_SIZE = max(1, (int)floor(sqrt(N)));
    int numBlocks = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    blocks.resize(numBlocks);
    for (int b = 0; b < numBlocks; b++){
        blocks[b].L = b * BLOCK_SIZE;
        blocks[b].R = min(N, (b + 1) * BLOCK_SIZE);
        blocks[b].dirty = true;
        blocks[b].hull.clear();
    }
 
    for (int i = 0; i < M; i++){
        int type;
        cin >> type;
        if(type == 1){
            // 입주 이벤트: "1 T K Z S"
            ll T_day, Z, S;
            int K;
            cin >> T_day >> K >> Z >> S;
            int idx = K - 1;
            // 입주 당일엔 수익이 없으므로, f(x) = (S - T_day*Z) + Z*x
            comp[idx].active = true;
            comp[idx].A = S - T_day * Z;
            comp[idx].B = Z;
 
            int bIndex = idx / BLOCK_SIZE;
            blocks[bIndex].dirty = true;
 
        } else {
            // 조사 이벤트: "2 T A B"
            ll T_day;
            int A_off, B_off;
            cin >> T_day >> A_off >> B_off;
            int L = min(A_off, B_off) - 1;
            int R = max(A_off, B_off); // [L, R) 범위 (0-index)
 
            ll ans = MINF;
            int startBlock = L / BLOCK_SIZE, endBlock = (R - 1) / BLOCK_SIZE;
 
            if(startBlock == endBlock){
                ans = queryBlockPartial(startBlock, L, R, T_day);
            } else {
                ans = max(ans, queryBlockPartial(startBlock, L, R, T_day));
                ans = max(ans, queryBlockPartial(endBlock, L, R, T_day));
                for (int b = startBlock + 1; b < endBlock; b++){
                    if(blocks[b].dirty) rebuildBlock(b);
                    ans = max(ans, queryBlockHull(b, T_day));
                }
            }
 
            if(ans == MINF)
                cout << "nema" << "\n";
            else
                cout << ans << "\n";
        }
    }
    return 0;
}
