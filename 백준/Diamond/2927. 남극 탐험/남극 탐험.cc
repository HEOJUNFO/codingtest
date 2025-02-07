#include <iostream>
#include <string>
using namespace std;

const int MAXN = 30000 + 5;

struct Node {
    int ch[2]; // 좌측, 우측 자식
    int parent;
    bool rev;  // lazy reversal flag
    int val;   // 해당 섬의 펭귄 수
    int sum;   // splay 트리 상의 구간 합 (자신과 자식들의 펭귄 수)
};

Node tree[MAXN];

// splay 트리의 루트 판별 (현재 LCT의 가상 트리 내에서의 루트 여부)
bool isSplayRoot(int x) {
    int p = tree[x].parent;
    return (p == 0 || (tree[p].ch[0] != x && tree[p].ch[1] != x));
}

// 자식의 정보를 이용해 현재 노드의 구간 합을 갱신
void pushUp(int x) {
    tree[x].sum = tree[x].val;
    if(tree[x].ch[0])
        tree[x].sum += tree[tree[x].ch[0]].sum;
    if(tree[x].ch[1])
        tree[x].sum += tree[tree[x].ch[1]].sum;
}

// lazy reversal 플래그가 있다면 자식에게 전파
void pushDown(int x) {
    if(tree[x].rev) {
        int l = tree[x].ch[0], r = tree[x].ch[1];
        if(l) tree[l].rev ^= true;
        if(r) tree[r].rev ^= true;
        // 좌우 자식을 swap
        swap(tree[x].ch[0], tree[x].ch[1]);
        tree[x].rev = false;
    }
}

// x의 모든 조상에 대해 lazy 값을 전파
void pushAll(int x) {
    if(!isSplayRoot(x))
        pushAll(tree[x].parent);
    pushDown(x);
}

// splay 회전: x를 부모와 교환
void rotate(int x) {
    int p = tree[x].parent;
    int gp = tree[p].parent;
    int dir = (tree[p].ch[1] == x); // x가 p의 오른쪽 자식이면 1, 아니면 0
    int other = tree[x].ch[dir ^ 1];
    if(!isSplayRoot(p)) {
        if(tree[gp].ch[0] == p)
            tree[gp].ch[0] = x;
        else
            tree[gp].ch[1] = x;
    }
    tree[x].parent = gp;
    tree[p].ch[dir] = other;
    if(other)
        tree[other].parent = p;
    tree[x].ch[dir ^ 1] = p;
    tree[p].parent = x;
    pushUp(p);
    pushUp(x);
}
 
// splay: x를 splay 트리의 루트로 끌어올림
void splay(int x) {
    pushAll(x);
    while(!isSplayRoot(x)){
        int p = tree[x].parent;
        int gp = tree[p].parent;
        if(!isSplayRoot(p)){
            if((tree[gp].ch[0] == p) ^ (tree[p].ch[0] == x))
                rotate(x);
            else
                rotate(p);
        }
        rotate(x);
    }
}
 
// access: x를 포함하는 preferred path를 재구성하고, x를 최상위로 끌어올림
void access(int x) {
    int last = 0;
    for (int y = x; y; y = tree[y].parent) {
        splay(y);
        tree[y].ch[1] = last;
        pushUp(y);
        last = y;
    }
    splay(x);
}
 
// makeRoot: x를 루트로 만드는 연산 (경로 상 순서를 반전시킴)
void makeRoot(int x) {
    access(x);
    tree[x].rev ^= true;
}
 
// findRoot: x가 속한 트리의 루트를 찾음
int findRoot(int x) {
    access(x);
    while(tree[x].ch[0]) {
        pushDown(x);
        x = tree[x].ch[0];
    }
    splay(x);
    return x;
}
 
// link: 두 노드를 연결 (두 트리가 분리되어 있을 때)
void link(int x, int y) {
    makeRoot(x);
    if(findRoot(y) != x) { // 분리되어 있으면 연결
        tree[x].parent = y;
    }
}
 
// queryPath: x에서 y까지의 경로에 있는 펭귄 수의 합을 구함
int queryPath(int x, int y) {
    makeRoot(x);
    access(y);
    return tree[y].sum;
}
 
// updateVal: 섬 x의 펭귄 수를 newVal로 갱신
void updateVal(int x, int newVal) {
    access(x);
    tree[x].val = newVal;
    pushUp(x);
}
 
// DSU (Union-Find) 자료구조: bridge 명령에서 두 섬의 연결 여부를 판별
int dsu_parent[MAXN];
 
int dsu_find(int a) {
    return dsu_parent[a] == a ? a : dsu_parent[a] = dsu_find(dsu_parent[a]);
}
 
void dsu_union(int a, int b) {
    a = dsu_find(a);
    b = dsu_find(b);
    if(a != b)
        dsu_parent[a] = b;
}
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    for(int i = 1; i <= N; i++){
        dsu_parent[i] = i;
        tree[i].parent = 0;
        tree[i].ch[0] = tree[i].ch[1] = 0;
        tree[i].rev = false;
        cin >> tree[i].val;
        tree[i].sum = tree[i].val;
    }
 
    int Q;
    cin >> Q;
    while(Q--){
        string cmd;
        cin >> cmd;
        if(cmd == "bridge"){
            int u, v;
            cin >> u >> v;
            // DSU로 두 섬의 연결 여부 확인
            if(dsu_find(u) == dsu_find(v)){
                cout << "no" << "\n";
            } else {
                cout << "yes" << "\n";
                dsu_union(u, v);
                link(u, v);
            }
        } else if(cmd == "penguins"){
            int u, x;
            cin >> u >> x;
            updateVal(u, x);
        } else if(cmd == "excursion"){
            int u, v;
            cin >> u >> v;
            if(dsu_find(u) != dsu_find(v)){
                cout << "impossible" << "\n";
            } else {
                int ans = queryPath(u, v);
                cout << ans << "\n";
            }
        }
    }
    return 0;
}
