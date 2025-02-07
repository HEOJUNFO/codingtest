#include <bits/stdc++.h>
using namespace std;
 
// 최대치: 서버 N 최대 8000, 회사 C 최대 100 (여유있게 105로 잡음)
const int MAXC = 105;
const int MAXN = 8500; // 약간 여유 있게
 
// 각 회사별 Link-Cut Tree (LCT) 자료구조를 2차원 배열로 관리합니다.
// 각 comp (1<=comp<=C)와 정점 (1<=x<=N)에 대해
// Lch[comp][x]: x의 왼쪽 자식, Rch[comp][x]: 오른쪽 자식, Parent[comp][x]: 부모, revFlag[comp][x]: 반전 플래그
static int Lch[MAXC][MAXN], Rch[MAXC][MAXN], Parent[MAXC][MAXN];
static bool revFlag[MAXC][MAXN];
// 각 회사별로 각 정점에 소속된 케이블 수(incident count)를 저장
static int deg[MAXC][MAXN];
 
// -- Link-Cut Tree 함수들 (회사 comp, 노드 x에 대해) --
 
// x가 splay 트리에서 루트인지 판단
inline bool isRoot(int comp, int x) {
    int p = Parent[comp][x];
    return (p == 0) || (Lch[comp][p] != x && Rch[comp][p] != x);
}
 
// revFlag가 켜져있으면 자식에 전파하고 자식들을 swap
inline void pushDown(int comp, int x) {
    if(revFlag[comp][x]){
        int l = Lch[comp][x], r = Rch[comp][x];
        if(l) revFlag[comp][l] = !revFlag[comp][l];
        if(r) revFlag[comp][r] = !revFlag[comp][r];
        swap(Lch[comp][x], Rch[comp][x]);
        revFlag[comp][x] = false;
    }
}
 
// x까지의 경로 상의 모든 노드에 대해 플래그 전파
void pushAll(int comp, int x) {
    if(!isRoot(comp, x))
        pushAll(comp, Parent[comp][x]);
    pushDown(comp, x);
}
 
// splay 트리 회전 (노드 x를 한 단계 위로 올림)
void rotate(int comp, int x) {
    int p = Parent[comp][x], g = Parent[comp][p];
    if(!isRoot(comp, p)) {
        if(Lch[comp][g] == p)
            Lch[comp][g] = x;
        else
            Rch[comp][g] = x;
    }
    Parent[comp][x] = g;
    if(Lch[comp][p] == x) {
        Lch[comp][p] = Rch[comp][x];
        if(Rch[comp][x])
            Parent[comp][ Rch[comp][x] ] = p;
        Rch[comp][x] = p;
        Parent[comp][p] = x;
    } else {
        Rch[comp][p] = Lch[comp][x];
        if(Lch[comp][x])
            Parent[comp][ Lch[comp][x] ] = p;
        Lch[comp][x] = p;
        Parent[comp][p] = x;
    }
}
 
// splay: x를 해당 보조트리의 루트로 끌어올림
void splay(int comp, int x) {
    pushAll(comp, x);
    while(!isRoot(comp, x)) {
        int p = Parent[comp][x], g = Parent[comp][p];
        if(!isRoot(comp, p)) {
            if((Lch[comp][g] == p) == (Lch[comp][p] == x))
                rotate(comp, p);
            else
                rotate(comp, x);
        }
        rotate(comp, x);
    }
}
 
// access: x를 기준으로 선호경로를 재구성 (x가 우측 체인 최상단이 되도록)
void access(int comp, int x) {
    for (int y = 0; x; x = Parent[comp][x]) {
        splay(comp, x);
        Rch[comp][x] = y;
        y = x;
    }
}
 
// makeRoot: x를 루트로 만듦 (트리에서의 루트, 반전 플래그 사용)
void makeRoot(int comp, int x) {
    access(comp, x);
    splay(comp, x);
    revFlag[comp][x] = !revFlag[comp][x];
}
 
// findRoot: x가 속한 트리의 루트를 반환
int findRoot(int comp, int x) {
    access(comp, x);
    splay(comp, x);
    while(Lch[comp][x]) {
        pushDown(comp, x);
        x = Lch[comp][x];
    }
    splay(comp, x);
    return x;
}
 
// 두 노드 u, v가 같은 트리에 속해있는지 반환
bool connected(int comp, int u, int v) {
    if(u==0 || v==0) return false;
    return findRoot(comp, u) == findRoot(comp, v);
}
 
// link: 서로 다른 트리에 있는 u와 v를 연결 (u의 루트를 v에 연결)
void lct_link(int comp, int u, int v) {
    makeRoot(comp, u);
    Parent[comp][u] = v;
}
 
// cut: u와 v 사이의 간선을 제거 (존재하면)
void lct_cut_edge(int comp, int u, int v) {
    makeRoot(comp, u);
    access(comp, v);
    splay(comp, v);
    if(Lch[comp][v] == u){
        Lch[comp][v] = 0;
        Parent[comp][u] = 0;
    } else {
        // 반대 순서로 시도
        makeRoot(comp, v);
        access(comp, u);
        splay(comp, u);
        if(Lch[comp][u] == v){
            Lch[comp][u] = 0;
            Parent[comp][v] = 0;
        }
    }
}
 
// -- End of LCT --
 
// 케이블 정보를 저장하는 구조체
struct Cable {
    int u, v, owner;
};
 
// (u,v) 쌍을 key로 하는 해시맵을 위해 해시 함수 정의 (u < v임을 가정)
struct PairHash {
    size_t operator()(const pair<int,int> &p) const {
        return ((size_t)p.first)*10007ULL + (size_t)p.second;
    }
};
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    while(true){
        int N, M, C, T;
        if(!(cin >> N >> M >> C >> T)) break;
        if(N==0 && M==0 && C==0 && T==0) break;
 
        // 각 회사별, 각 정점별 LCT 데이터와 incident count 초기화
        for (int comp = 1; comp <= C; comp++){
            for (int i = 1; i <= N; i++){
                Lch[comp][i] = 0;
                Rch[comp][i] = 0;
                Parent[comp][i] = 0;
                revFlag[comp][i] = false;
                deg[comp][i] = 0;
            }
        }
 
        // 케이블 정보와 (u,v) -> 케이블 index를 저장할 해시맵
        vector<Cable> cables(M);
        unordered_map<pair<int,int>, int, PairHash> cableMap;
        cableMap.reserve(M * 2);
        for (int i = 0; i < M; i++){
            int a, b, comp;
            cin >> a >> b >> comp;
            cables[i] = {a, b, comp};
            cableMap[{a, b}] = i;
            // 해당 회사의 incident count 업데이트
            deg[comp][a]++;
            deg[comp][b]++;
            // 초기 상태이므로 규정(acyclic, 각 정점 최대 2) 만족; LCT에 간선 추가
            lct_link(comp, a, b);
        }
 
        // 거래 처리
        for (int i = 0; i < T; i++){
            int a, b, comp;
            cin >> a >> b >> comp;
            // 케이블 정의에서는 a < b였으므로, 거래에서는 정점 순서를 정규화
            int u = a, v = b;
            if(u > v) swap(u, v);
 
            auto it = cableMap.find({u, v});
            if(it == cableMap.end()){
                cout << "No such cable." << "\n";
                continue;
            }
            int idx = it->second;
            int curOwner = cables[idx].owner;
 
            if(curOwner == comp){
                cout << "Already owned." << "\n";
                continue;
            }
 
            // monopoly 판정: 이미 해당 회사가 a 또는 b에서 2개 이상의 케이블을 소유중이면 금지
            if(deg[comp][a] >= 2 || deg[comp][b] >= 2){
                cout << "Forbidden: monopoly." << "\n";
                continue;
            }
 
            // redundant 판정: company comp의 LCT 상에서 a와 b가 이미 연결되어 있다면(즉, 이 케이블을 추가하면 사이클 발생)
            if( connected(comp, a, b) ){
                cout << "Forbidden: redundant." << "\n";
                continue;
            }
 
            // 위의 규칙에 어긋나지 않으므로 판매 진행
            cout << "Sold." << "\n";
 
            // 이전 소유회사로부터 케이블 제거 (incident count 감소 및 LCT에서 간선 제거)
            if(curOwner != 0){
                deg[curOwner][a]--;
                deg[curOwner][b]--;
                lct_cut_edge(curOwner, a, b);
            }
 
            // 새 소유회사(comp)에 간선 추가
            cables[idx].owner = comp;
            deg[comp][a]++;
            deg[comp][b]++;
            lct_link(comp, a, b);
        }
 
        cout << "\n";
    }
    return 0;
}
