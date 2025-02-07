#include <bits/stdc++.h>
using namespace std;
 
// 최대 직원 수 : N 최대 300,000 → 총 노드 수 = N+1
const int NMAX = 300001;
int N; // 입력으로 주어지는 새 직원 수 (창업자 제외)
 
// 각 노드(직원)의 월급과 상사 정보
int sal[NMAX];
int par[NMAX]; // 0번(창업자)는 par[0] = -1 로 설정
vector<int> children[NMAX];
 
// Heavy-Light Decomposition 관련 전역변수
int heavy[NMAX], depth[NMAX], sizeArr[NMAX];
int head[NMAX], pos[NMAX];
int curPos = 0;
 
// DFS: 각 노드의 부분트리 크기와 heavy child 계산
int dfs(int v) {
    sizeArr[v] = 1;
    heavy[v] = -1;
    int maxSize = 0;
    for (int c : children[v]) {
        depth[c] = depth[v] + 1;
        int subSize = dfs(c);
        if(subSize > maxSize){
            maxSize = subSize;
            heavy[v] = c;
        }
        sizeArr[v] += subSize;
    }
    return sizeArr[v];
}
 
// HLD: 체인 head와 base array 상의 위치를 결정
void decompose(int v, int h) {
    head[v] = h;
    pos[v] = curPos++;
    if(heavy[v] != -1)
        decompose(heavy[v], h);
    for (int c : children[v]) {
        if(c == heavy[v]) continue;
        decompose(c, c);
    }
}
 
// --- 세그먼트 트리 ---
// base 배열 : HLD 순서대로 각 노드의 월급을 저장 (size = M = N+1)
int M;
vector<int> base;
 
struct SegNode {
    int leftVal;  // 구간의 왼쪽(체인 상위쪽) 값
    int rightVal; // 구간의 오른쪽(체인 하위쪽) 값
    bool allSame; // 구간의 모든 값이 같은가?
};
 
vector<SegNode> seg;
vector<int> lazy; // lazy 값: -1이면 업데이트 없음, 아니면 해당 값으로 구간이 대입되어 있음.
 
// 세그먼트 트리 빌드
void segBuild(int idx, int l, int r) {
    if(l == r) {
        seg[idx].leftVal = base[l];
        seg[idx].rightVal = base[l];
        seg[idx].allSame = true;
        return;
    }
    int mid = (l + r) / 2;
    segBuild(idx*2, l, mid);
    segBuild(idx*2+1, mid+1, r);
    seg[idx].leftVal = seg[idx*2].leftVal;
    seg[idx].rightVal = seg[idx*2+1].rightVal;
    seg[idx].allSame = (seg[idx*2].allSame && seg[idx*2+1].allSame && (seg[idx*2].leftVal == seg[idx*2+1].leftVal));
}
 
// lazy 적용: 구간 [l, r]를 val로 대입
void segApply(int idx, int l, int r, int val) {
    seg[idx].leftVal = val;
    seg[idx].rightVal = val;
    seg[idx].allSame = true;
    lazy[idx] = val;
}
 
// lazy 전파
void segPushDown(int idx, int l, int r) {
    if(lazy[idx] != -1) {
        int mid = (l + r) / 2;
        segApply(idx*2, l, mid, lazy[idx]);
        segApply(idx*2+1, mid+1, r, lazy[idx]);
        lazy[idx] = -1;
    }
}
 
// 구간 업데이트: [ql, qr]를 val로 대입
void segUpdate(int idx, int l, int r, int ql, int qr, int val) {
    if(ql <= l && r <= qr) {
        segApply(idx, l, r, val);
        return;
    }
    segPushDown(idx, l, r);
    int mid = (l + r) / 2;
    if(ql <= mid) segUpdate(idx*2, l, mid, ql, qr, val);
    if(qr > mid) segUpdate(idx*2+1, mid+1, r, ql, qr, val);
    seg[idx].leftVal = seg[idx*2].leftVal;
    seg[idx].rightVal = seg[idx*2+1].rightVal;
    seg[idx].allSame = (seg[idx*2].allSame && seg[idx*2+1].allSame &&
                        seg[idx*2].leftVal == seg[idx*2+1].leftVal);
}
 
// 질의: 구간 [ql,qr]에서 왼쪽부터 스캔하며 “값 < s”인 첫 인덱스를 찾는다.
// 찾지 못하면 -1를 리턴한다.
int segQueryFirst(int idx, int l, int r, int ql, int qr, int s) {
    if(r < ql || l > qr) return -1;
    if(ql <= l && r <= qr) {
        if(seg[idx].allSame) {
            if(seg[idx].leftVal < s) return l;
            else return -1;
        }
    }
    segPushDown(idx, l, r);
    int mid = (l + r) / 2;
    int leftAns = segQueryFirst(idx*2, l, mid, ql, qr, s);
    if(leftAns != -1) return leftAns;
    return segQueryFirst(idx*2+1, mid+1, r, ql, qr, s);
}
 
// --- 메인 ---
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> N;
    M = N + 1;
    // 창업자(0번) 월급 읽기; par[0] = -1
    cin >> sal[0];
    par[0] = -1;
    for (int i = 1; i <= N; i++){
        int s, b;
        cin >> s >> b;
        sal[i] = s;
        par[i] = b;
        children[b].push_back(i);
    }
 
    // 트리 구성 및 HLD 준비
    depth[0] = 0;
    dfs(0);
    decompose(0, 0);
 
    // base 배열 구성: base[pos[v]] = sal[v]
    base.resize(M);
    vector<int> nodeAt(M);
    for (int v = 0; v < M; v++){
        nodeAt[ pos[v] ] = v;
    }
    for (int i = 0; i < M; i++){
        int v = nodeAt[i];
        base[i] = sal[v];
    }
 
    // 세그먼트 트리 초기화
    seg.resize(4 * M);
    lazy.assign(4 * M, -1);
    segBuild(1, 0, M-1);
 
    /* 
       이제 “새 직원 채용” 이벤트를 순서대로 처리한다.
       새로운 직원 i (1<=i<=N)가 채용될 때, 
         - 새 월급 newSal = sal[i],
         - 상사 = par[i] (이미 채용된 노드)
       에 대해, 상사 체인을 따라 “월급이 newSal보다 작은” 직원들을
       한 번에 갱신하고, 갱신된 직원 수를 센다.
 
       Heavy–Light Decomposition를 이용하면,
         while(u != -1) {
           let chainHead = head[u];
           구간 [ pos[chainHead], pos[u] ]는 한 체인 구간 (위쪽은 체인 head)
           이 구간 내에서 “왼쪽부터 스캔하여 값 < newSal 인 첫 인덱스 posFirst를 찾는다.”
           – 만약 posFirst == -1 → 이 체인 구간에는 모두 ≥ newSal이므로 전파 중단.
           – 만약 posFirst == pos[chainHead] : 체인 전체가 newSal 미만 → 전체 갱신 후 u = par[chainHead]로 전파.
           – 그렇지 않으면, 구간 [posFirst, pos[u]]만 갱신하고 전파 중단.
         }
    */
    for (int i = 1; i <= N; i++){
        int newSal = sal[i]; // 새 직원의 월급
        int boss = par[i];    // 새 직원의 상사
        int countUpdated = 0;
        int u = boss;
        while(u != -1) {
            int chainHead = head[u];
            int L = pos[chainHead], R = pos[u];
            // [L,R]는 한 체인 구간 (체인 head가 L, u가 R)
            // 이 구간은 부모의 월급이 자식의 월급보다 크거나 같으므로 내림차순.
            // u(구간의 오른쪽)는 반드시 현재 newSal보다 작으므로, 구간 내 어딘가에는 값 < newSal인 곳이 있다.
            int posFirst = segQueryFirst(1, 0, M-1, L, R, newSal);
            if(posFirst == -1) break; // 이 구간에는 갱신할 노드가 없으므로 전파 중단.
            int blockCount = R - posFirst + 1;
            countUpdated += blockCount;
            // 구간 [posFirst, R]를 newSal로 대입
            segUpdate(1, 0, M-1, posFirst, R, newSal);
            // 만약 체인 head의 값이 newSal 이상(즉, posFirst > L)라면 그 위(상사)는 업데이트하지 않으므로 전파 중단.
            if(posFirst > L) break;
            u = par[ chainHead ]; // 다음 체인: 체인 head의 상사로 전파
        }
        cout << countUpdated << "\n";
    }
    return 0;
}
