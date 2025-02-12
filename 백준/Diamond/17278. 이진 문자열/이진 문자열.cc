#include <bits/stdc++.h>
using namespace std;
 
// 각 구간(segment): 정의역 [L, R]에서 f(p) = p + delta, 
// flip=0이면 그대로, flip=1이면 S₀의 문자를 반전시킴.
struct Segment {
    long long L, R;
    long long delta;
    int flip; // 0 또는 1
};
 
// 인접한 구간 중 (delta, flip)이 동일하면 병합
void mergeSegments(vector<Segment> &segs) {
    if(segs.empty()) return;
    vector<Segment> merged;
    merged.push_back(segs[0]);
    for (size_t i = 1; i < segs.size(); i++) {
        Segment &last = merged.back();
        if(segs[i].L == last.R + 1 && segs[i].delta == last.delta && segs[i].flip == last.flip)
            last.R = segs[i].R;
        else
            merged.push_back(segs[i]);
    }
    segs.swap(merged);
}
 
// globalK: 최종 문자열의 앞 k글자를 다루므로, 함수의 정의역는 [1, k]
long long globalK;
 
// op의 역함수 f_op를 현재 함수 F와 합성하는 함수  
// op의 파라미터: x, y (d = y - x + 1)
// f_op : 입력 q (1≤q≤globalK)에 대해
//  - q in [1, y]           → q, flip 0
//  - q in [y+1, min(K, y+d)] → q + (x - y - 1), flip 1
//  - q in [min(K, y+d)+1, K] → q - d, flip 0
// 현재 F는 p in [L,R]에서 F(p)= p + C, flip = b.
// 합성: G(p) = f_op( F(p) )
vector<Segment> composeWithOp(const vector<Segment> &F, long long x, long long y) {
    long long d = y - x + 1;
    vector<Segment> G;
    long long K = globalK;
    // f_op의 세 구간을 아래와 같이 둡니다.
    // A: q in [1, y] → delta 0, flip 0.
    // B: q in [y+1, min(K, y+d)] → delta = (x - y - 1), flip 1.
    // C: q in [min(K, y+d)+1, K] (존재하면) → delta = -d, flip 0.
    for (auto &seg : F) {
        long long Lp = seg.L, Rp = seg.R;
        long long C = seg.delta;
        int b = seg.flip;
        // p에 대해 q = p + C; q는 [Lp+C, Rp+C]
        // --- Piece A: q in [1, y] ---
        long long A1 = 1, A2 = y;
        long long pA_L = max(Lp, A1 - C);
        long long pA_R = min(Rp, A2 - C);
        if(pA_L <= pA_R) {
            Segment newSeg;
            newSeg.L = pA_L;
            newSeg.R = pA_R;
            newSeg.delta = C; // f_op(q)=q
            newSeg.flip = b;  // b XOR 0
            G.push_back(newSeg);
        }
        // --- Piece B: q in [y+1, min(K, y+d)] ---
        long long B1 = y + 1;
        long long B2 = min(K, y + d);
        if(B1 <= B2) {
            long long pB_L = max(Lp, B1 - C);
            long long pB_R = min(Rp, B2 - C);
            if(pB_L <= pB_R) {
                Segment newSeg;
                newSeg.L = pB_L;
                newSeg.R = pB_R;
                newSeg.delta = C + (x - y - 1); // f_op(q)=q + (x-y-1)
                newSeg.flip = b ^ 1; // 반전
                G.push_back(newSeg);
            }
        }
        // --- Piece C: q in [min(K, y+d)+1, K] ---
        long long r = min(K, y + d);
        if(r < K) {
            long long C1_val = r + 1;
            long long pC_L = max(Lp, C1_val - C);
            long long pC_R = min(Rp, K - C);
            if(pC_L <= pC_R) {
                Segment newSeg;
                newSeg.L = pC_L;
                newSeg.R = pC_R;
                newSeg.delta = C - d; // f_op(q)=q - d
                newSeg.flip = b; // 그대로
                G.push_back(newSeg);
            }
        }
    }
    sort(G.begin(), G.end(), [](const Segment &a, const Segment &b) {
        return a.L < b.L;
    });
    mergeSegments(G);
    return G;
}
 
// 메인 – 각 테스트 케이스마다
// 1. 초기 문자열 S (S₀)와 m개의 op (x,y)를 입력받음.
// 2. 항등함수 F (정의역 [1,k]에서 f(p)=p)를 초기값으로 설정.
// 3. op들을 마지막 연산부터 첫 연산 순으로 순회하면서, op의 y가 k 미만이면 F를 composeWithOp 로 업데이트.
// 4. F의 각 구간 [L,R]에 대해, p의 값은 S₀의 (p+delta)번째 문자를 사용하며, flip flag에 따라 문자를 반전시킴.
// 5. 최종적으로 앞 k글자에 해당하는 문자열을 출력.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
    while(T--){
        long long n, m, k;
        cin >> n >> m >> k;
        globalK = k;
 
        string S;
        cin >> S;
        // S: 초기 이진 문자열 (길이 n)
 
        // m개의 op 입력 (각 op: x, y)
        struct Op { long long x, y; };
        vector<Op> ops(m);
        for (int i = 0; i < m; i++){
            cin >> ops[i].x >> ops[i].y;
        }
 
        // F: 최종 Sₘ의 앞 k글자 p (1≤p≤k)가 S₀에서 어느 위치의 문자로 대응되는지 나타내는 piecewise 함수.
        vector<Segment> F;
        F.push_back({1, k, 0, 0}); // 항등함수: f(p)=p, flip 0.
 
        // 연산은 S에 순서대로 적용되었으므로, 역추적은 마지막 op부터 첫 op까지 진행.
        // op의 y가 k 이상이면 Sₘ의 앞 k에는 영향을 주지 않으므로 건너뜁니다.
        for (int i = m - 1; i >= 0; i--){
            if(ops[i].y < k){
                F = composeWithOp(F, ops[i].x, ops[i].y);
                if(F.empty()) break; // 이론상 발생하지 않음.
            }
        }
 
        // F를 이용하여 답을 구성합니다.
        // F의 각 구간 [L,R]에서, 최종 Sₘ의 p번째 문자는 S₀의 (p + delta)번째 (1-indexed)이며,
        // flip flag가 1이면 문자를 반전합니다.
        string ans;
        ans.resize(k);
        for(auto &seg : F){
            long long startP = seg.L, endP = seg.R;
            long long offset = seg.delta;
            int flipFlag = seg.flip;
            // S₀에서의 해당 구간: p + offset, p = startP ... endP.
            // 즉, S₀의 인덱스는 (startP+offset)부터 (endP+offset) (1-indexed).
            long long pos0_start = startP + offset; // 1-indexed
            long long len = endP - startP + 1;
            for (int j = 0; j < len; j++){
                char c = S[(pos0_start - 1) + j];
                if(flipFlag == 1)
                    c = (c=='0' ? '1' : '0');
                ans[startP - 1 + j] = c;
            }
        }
 
        cout << ans << "\n";
    }
    return 0;
}
