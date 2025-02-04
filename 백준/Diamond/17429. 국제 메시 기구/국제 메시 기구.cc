#include <bits/stdc++.h>
using namespace std;
 
// 32비트 unsigned 정수를 사용 (unsigned int 연산은 2^32 모듈로 이루어짐)
using uint32 = uint32_t;
 
// 최대 노드 수 (문제 조건)
const int NMAX = 500000;
 
// 트리의 인접 리스트 (1-indexed)
vector<int> adj[NMAX+1];
 
// heavy-light decomposition에 사용할 배열들
vector<int> par, depth, siz, heavy, head, pos, tin, tout;
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, Q;
    cin >> N >> Q;
    for (int i = 1; i <= N; i++){
        adj[i].clear();
    }
    for (int i = 1; i <= N-1; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    // 필요한 배열들을 크기 N+1로 재할당
    par.resize(N+1, 0);
    depth.resize(N+1, 0);
    siz.resize(N+1, 0);
    heavy.resize(N+1, -1);
    head.resize(N+1, 0);
    pos.resize(N+1, 0);
    tin.resize(N+1, 0);
    tout.resize(N+1, 0);
 
    // 1. par, depth, 그리고 순서를 구하기 위한 DFS (반복문 사용)
    vector<int> order;
    order.reserve(N);
    {
        vector<int> st;
        st.push_back(1);
        par[1] = 0;  // 1번 노드는 루트 (부모 없음: 0으로 표시)
        depth[1] = 0;
        while(!st.empty()){
            int u = st.back();
            st.pop_back();
            order.push_back(u);
            for(auto v : adj[u]){
                if(v == par[u]) continue;
                par[v] = u;
                depth[v] = depth[u] + 1;
                st.push_back(v);
            }
        }
    }
 
    // 2. 역순으로 subtree 크기(siz)와 heavy 자식 계산
    for (int i = (int)order.size()-1; i >= 0; i--){
        int u = order[i];
        siz[u] = 1;
        int maxSize = 0;
        for(auto v : adj[u]){
            if(v == par[u]) continue;
            siz[u] += siz[v];
            if(siz[v] > maxSize){
                maxSize = siz[v];
                heavy[u] = v;
            }
        }
    }
 
    // 3. heavy-light decomposition (보통의 재귀형 decompose(u, h)를 반복문으로 구현)
    //    pos[u]는 base array에서 u의 위치, tin[u]는 서브트리 구간의 시작,
    //    tout[u]는 tin[u] + siz[u] - 1 로 설정합니다.
    int curPos = 0;
    {
        vector<int> st; // light child들을 저장할 스택
        int u = 1, chainHead = 1;
        while(true){
            head[u] = chainHead;
            pos[u] = ++curPos;
            tin[u] = pos[u];  // tin[u] = pos[u] (서브트리 구간의 시작)
            // heavy 자식이 아닌 light child들을 스택에 넣습니다.
            for(auto v : adj[u]){
                if(v == par[u] || v == heavy[u]) continue;
                st.push_back(v);
            }
            if(heavy[u] != -1){
                // heavy chain 계속
                u = heavy[u];
                // chainHead는 그대로
                continue;
            } else {
                if(!st.empty()){
                    u = st.back();
                    st.pop_back();
                    chainHead = u;  // 새로운 체인의 헤드
                    continue;
                } else {
                    break;  // 모든 노드 방문 완료
                }
            }
        }
    }
    // tout[u] = pos[u] + siz[u] - 1 (서브트리 내 노드들이 연속임)
    for (int u = 1; u <= N; u++){
        tout[u] = pos[u] + siz[u] - 1;
    }
 
    // 4. 세그먼트 트리 구축
    // base array의 인덱스는 1 ~ N (curPos == N)
    // 초기 금액은 모두 0입니다.
    int segSize = N;
    // seg[] : 구간 합, lazy_mul[]와 lazy_add[] : lazy 전파용 (초기값 identity: 곱셈 1, 덧셈 0)
    vector<uint32> seg(4 * segSize + 5, 0);
    vector<uint32> lazy_mul(4 * segSize + 5, 1);
    vector<uint32> lazy_add(4 * segSize + 5, 0);
 
    // 세그먼트 트리 함수들을 lambda로 정의합니다.
    // push_down : lazy 값을 자식에게 전파.
    function<void(int, int, int)> push_down = [&](int idx, int l, int r) {
        if(lazy_mul[idx] != 1 || lazy_add[idx] != 0){
            int mid = (l + r) >> 1;
            int left = idx << 1, right = left | 1;
            seg[left] = seg[left] * lazy_mul[idx] + lazy_add[idx] * (uint32)(mid - l + 1);
            lazy_mul[left] = lazy_mul[left] * lazy_mul[idx];
            lazy_add[left] = lazy_add[left] * lazy_mul[idx] + lazy_add[idx];
 
            seg[right] = seg[right] * lazy_mul[idx] + lazy_add[idx] * (uint32)(r - mid);
            lazy_mul[right] = lazy_mul[right] * lazy_mul[idx];
            lazy_add[right] = lazy_add[right] * lazy_mul[idx] + lazy_add[idx];
 
            lazy_mul[idx] = 1;
            lazy_add[idx] = 0;
        }
    };
 
    // seg_update(idx, l, r, ql, qr, mul, add)
    // [ql,qr] 구간에 대해 각 원소를 (mul * x + add)로 변환합니다.
    function<void(int, int, int, int, int, uint32, uint32)> seg_update = 
    [&](int idx, int l, int r, int ql, int qr, uint32 mul, uint32 add) -> void {
        if(ql <= l && r <= qr){
            seg[idx] = seg[idx] * mul + add * (uint32)(r - l + 1);
            lazy_mul[idx] = lazy_mul[idx] * mul;
            lazy_add[idx] = lazy_add[idx] * mul + add;
            return;
        }
        push_down(idx, l, r);
        int mid = (l + r) >> 1;
        if(ql <= mid)
            seg_update(idx<<1, l, mid, ql, qr, mul, add);
        if(qr > mid)
            seg_update(idx<<1|1, mid+1, r, ql, qr, mul, add);
        seg[idx] = seg[idx<<1] + seg[idx<<1|1];
    };
 
    // seg_query(idx, l, r, ql, qr) : [ql,qr] 구간 합을 구합니다.
    function<uint32(int, int, int, int, int)> seg_query = 
    [&](int idx, int l, int r, int ql, int qr) -> uint32 {
        if(ql <= l && r <= qr)
            return seg[idx];
        push_down(idx, l, r);
        int mid = (l + r) >> 1;
        uint32 res = 0;
        if(ql <= mid)
            res = res + seg_query(idx<<1, l, mid, ql, qr);
        if(qr > mid)
            res = res + seg_query(idx<<1|1, mid+1, r, ql, qr);
        return res;
    };
 
    // 5. heavy-light decomposition 을 이용한 경로 업데이트/쿼리 함수.
    // update_path(u, v, mul, add) : u에서 v까지의 경로에 대해 (mul*x + add) 연산을 적용.
    auto update_path = [&](int u, int v, uint32 mul, uint32 add) {
        while(head[u] != head[v]){
            if(depth[ head[u] ] < depth[ head[v] ])
                swap(u, v);
            seg_update(1, 1, segSize, pos[ head[u] ], pos[u], mul, add);
            u = par[ head[u] ];
        }
        if(depth[u] > depth[v])
            swap(u, v);
        seg_update(1, 1, segSize, pos[u], pos[v], mul, add);
    };
 
    // query_path(u, v) : u에서 v까지의 경로의 합을 구함.
    auto query_path = [&](int u, int v) -> uint32 {
        uint32 res = 0;
        while(head[u] != head[v]){
            if(depth[ head[u] ] < depth[ head[v] ])
                swap(u, v);
            res = res + seg_query(1, 1, segSize, pos[ head[u] ], pos[u]);
            u = par[ head[u] ];
        }
        if(depth[u] > depth[v])
            swap(u, v);
        res = res + seg_query(1, 1, segSize, pos[u], pos[v]);
        return res;
    };
 
    // 6. 쿼리 처리
    // 명령어 설명:
    // 1 X V : 금고 X의 서브트리에 있는 모든 금고에 V원 추가  (업데이트: (mul=1, add=V))
    // 2 X Y V : 금고 X부터 금고 Y까지의 경로에 V원 추가         (업데이트: (mul=1, add=V))
    // 3 X V : 금고 X의 서브트리의 금액을 V배                    (업데이트: (mul=V, add=0))
    // 4 X Y V : 금고 X부터 금고 Y까지의 경로의 금액을 V배         (업데이트: (mul=V, add=0))
    // 5 X : 금고 X의 서브트리의 금액 합 출력
    // 6 X Y : 금고 X부터 금고 Y까지의 경로의 금액 합 출력
    //
    // 서브트리 구간은 [ tin[X], tout[X] ] 로 처리합니다.
 
    for (int i = 0; i < Q; i++){
        int t;
        cin >> t;
        if(t == 1){
            int X; uint32 V;
            cin >> X >> V;
            seg_update(1, 1, segSize, tin[X], tout[X], 1, V);
        }
        else if(t == 2){
            int X, Y; uint32 V;
            cin >> X >> Y >> V;
            update_path(X, Y, 1, V);
        }
        else if(t == 3){
            int X; uint32 V;
            cin >> X >> V;
            seg_update(1, 1, segSize, tin[X], tout[X], V, 0);
        }
        else if(t == 4){
            int X, Y; uint32 V;
            cin >> X >> Y >> V;
            update_path(X, Y, V, 0);
        }
        else if(t == 5){
            int X;
            cin >> X;
            uint32 ans = seg_query(1, 1, segSize, tin[X], tout[X]);
            cout << ans << "\n";
        }
        else if(t == 6){
            int X, Y;
            cin >> X >> Y;
            uint32 ans = query_path(X, Y);
            cout << ans << "\n";
        }
    }
 
    return 0;
}
