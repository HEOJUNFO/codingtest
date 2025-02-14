#include <bits/stdc++.h>
using namespace std;
 
// 최대 a[i]의 값 (문제에서 1<=a[i]<=1,000,000)
const int MAXV = 1000000;
 
// 후보 정보를 저장하는 구조체
struct Candidate {
    int dp;     // 해당 원소까지 선택했을 때의 길이
    int idx;    // 이 원소의 원래 인덱스 (1-indexed)
    int state;  // 이 후보가 끝난 상태 (1..k). 상태 r means "다음 비교는 p[r]를 써야함"
    int prev;   // 이전 후보의 인덱스 (없으면 -1)
};
 
// 기본(빈) 후보
Candidate defCand = {0, 0, 0, -1};
 
// FenwTree: prefix 최대값 (dp가 최대인 후보)를 관리 (1-indexed)
// 사용: 그룹이 '<'인 후보들을 관리 (a값 기준 오름차순 쿼리)
struct Fenw {
    int n;
    vector<Candidate> fenw;
    Fenw(int n_) : n(n_) {
        fenw.assign(n+1, defCand);
    }
    // pos: 1-indexed (a의 값)
    void update(int pos, const Candidate &cand) {
        for(; pos <= n; pos += pos & -pos) {
            if(cand.dp > fenw[pos].dp)
                fenw[pos] = cand;
        }
    }
    // query maximum candidate in [1, pos]
    Candidate query(int pos) {
        Candidate ret = defCand;
        for(; pos; pos -= pos & -pos) {
            if(fenw[pos].dp > ret.dp)
                ret = fenw[pos];
        }
        return ret;
    }
};
 
// FenwTree for suffix queries (for '>' group).
// 여기서는 a값을 반전시켜서 (rev = MAXV - a + 1) prefix query로 '>' 조건 (즉, a > x → rev < (MAXV - x +1))를 처리함.
struct FenwRev {
    int n;
    vector<Candidate> fenw;
    FenwRev(int n_) : n(n_) {
        fenw.assign(n+1, defCand);
    }
    void update(int pos, const Candidate &cand) {
        for(; pos <= n; pos += pos & -pos) {
            if(cand.dp > fenw[pos].dp)
                fenw[pos] = cand;
        }
    }
    Candidate query(int pos) {
        Candidate ret = defCand;
        for(; pos; pos -= pos & -pos) {
            if(fenw[pos].dp > ret.dp)
                ret = fenw[pos];
        }
        return ret;
    }
};
 
// global arrays: a[1..n]와 p[1..k] (p는 char: '<', '>', '=')
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, k;
    cin >> n >> k;
    vector<int> a(n+1);
    for (int i = 1; i <= n; i++){
        cin >> a[i];
    }
    // p[1..k] : 패턴. (문제에서는 기호들이 공백으로 구분되어 주어짐)
    vector<char> pat(k+1);
    for (int i = 1; i <= k; i++){
        cin >> pat[i];
    }
 
    // 세 그룹별 Fenw tree 및 '=' 그룹용 배열
    // 그룹 '<': 조건: 이전 값 < 현재 값 → FenwL: prefix query up to (x-1)
    Fenw fenwL(MAXV);
    // 그룹 '>': 조건: 이전 값 > 현재 값 → FenwG: 반전된 값에 대해 prefix query
    FenwRev fenwG(MAXV);
    // 그룹 '=': 조건: 이전 값 == 현재 값 → 배열 bestE[x]
    vector<Candidate> bestE(MAXV+1, defCand);
 
    // 결과 후보 (global best)
    Candidate bestGlobal = defCand;
    // 후보정보를 각 원소별로 저장 (재구현용)
    // candidate[i] : a[i]를 마지막 원소로 하는 최적 후보 (선택한 subsequence 정보)
    vector<Candidate> candInfo(n+1, defCand);
 
    // DP: 원소 i를 순서대로 처리
    for (int i = 1; i <= n; i++){
        int x = a[i];
        // 시작하는 경우: subsequence 길이 1, 상태 1 (다음 비교는 pat[1] 사용), prev = -1.
        Candidate cur = {1, i, 1, -1};
 
        // 확장 가능한 후보들: 그룹별로 쿼리 수행
        // 1. 그룹 '<': 이전 후보가 속한 그룹이 '<'일 경우, 조건: prev의 a < x.
        if(x > 1){
            Candidate candL = fenwL.query(x - 1);
            if(candL.dp > 0){
                // 새로운 후보: dp = candL.dp + 1, state 갱신: (candL.state==k ? 1 : candL.state+1)
                int ns = (candL.state == k ? 1 : candL.state + 1);
                Candidate temp = {candL.dp + 1, i, ns, candL.idx};
                if(temp.dp > cur.dp)
                    cur = temp;
            }
        }
        // 2. 그룹 '>': 조건: 이전 후보의 a > x → 반전 값에서 query.
        if(x < MAXV){
            int rx = MAXV - x; // query 범위:  rx >= 1
            Candidate candG = fenwG.query(rx);
            if(candG.dp > 0){
                int ns = (candG.state == k ? 1 : candG.state + 1);
                Candidate temp = {candG.dp + 1, i, ns, candG.idx};
                if(temp.dp > cur.dp)
                    cur = temp;
            }
        }
        // 3. 그룹 '=': 조건: 이전 후보의 a == x.
        {
            Candidate candE = bestE[x];
            if(candE.dp > 0){
                int ns = (candE.state == k ? 1 : candE.state + 1);
                Candidate temp = {candE.dp + 1, i, ns, candE.idx};
                if(temp.dp > cur.dp)
                    cur = temp;
            }
        }
 
        candInfo[i] = cur; // 현재 원소를 마지막으로 하는 후보 정보 저장
 
        // 이 cur 후보가 의미하는 건, a[i]를 마지막으로 하는 subsequence의 정보.
        // 이제, a[i]의 값과, 새로 갱신된 상태 cur.state에 따라 어느 그룹의 자료구조에 넣을지 결정:
        // 후보의 "다음 비교 기호"는 pat[cur.state] (즉, cur 상태가 의미하는 비교)
        char grp = pat[cur.state];
        if(grp == '<'){
            fenwL.update(x, cur);
        } else if(grp == '>'){
            // 반전 키: r = MAXV - x + 1
            fenwG.update(MAXV - x + 1, cur);
        } else { // '='
            if(cur.dp > bestE[x].dp)
                bestE[x] = cur;
        }
 
        // global best 갱신
        if(cur.dp > bestGlobal.dp)
            bestGlobal = cur;
    }
 
    // 재구성을 위해, 각 원소에 대해 predecessor 정보를 candInfo[i].prev를 따라가면 된다.
    int len = bestGlobal.dp;
    vector<int> seq;
    int curIdx = bestGlobal.idx;
    while(curIdx != -1) {
        seq.push_back(a[curIdx]);
        curIdx = candInfo[curIdx].prev;
    }
    reverse(seq.begin(), seq.end());
 
    cout << len << "\n";
    for (int i = 0; i < len; i++){
        cout << seq[i] << (i+1==len ? "\n" : " ");
    }
 
    return 0;
}
