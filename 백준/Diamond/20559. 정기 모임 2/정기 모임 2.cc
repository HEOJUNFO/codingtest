#include <bits/stdc++.h>
using namespace std;
 
// DSU (Union‐Find) – 각 정점의 최소 번호를 함께 관리
struct DSU {
    vector<int> parent;
    vector<int> mn; // 해당 집합의 최소 정점 번호
    DSU(int n) : parent(n+1), mn(n+1) {
        for(int i=1; i<=n; i++){
            parent[i]=i; 
            mn[i] = i;
        }
    }
    int findp(int a) {
        return parent[a] = (parent[a]==a ? a : findp(parent[a]));
    }
    // union 연산; 두 집합을 합칠 때, 합쳐지기 전 두 집합의 최소 정점 번호의 최대값을 activatedThreshold로 반환
    bool uni(int a, int b, int &activatedThreshold) {
        a = findp(a), b = findp(b);
        if(a==b)return false;
        activatedThreshold = max(mn[a], mn[b]);
        if(mn[a] > mn[b]) swap(a,b);
        parent[b]=a;
        mn[a] = min(mn[a], mn[b]);
        return true;
    }
};
 
// DSU 이벤트 – (T, w) : T는 이 merge가 “활성화”되기 위한 임계값, w는 해당 간선의 가중치
struct Event {
    int T;
    int w;
};
 
// Fenwick Tree (Binary Indexed Tree) 를 이용해 order-statistic 쿼리 지원
// 여기서는 두 Fenw: 하나는 빈도, 하나는 합을 저장 (가중치 값은 1~N 범위)
struct FenwPair {
    int n;
    vector<int> fenwCount;
    vector<long long> fenwSum;
    FenwPair(int n): n(n), fenwCount(n+1,0), fenwSum(n+1,0) {}
    void update(int i, int cntDelta, int valDelta) {
        for(; i<=n; i+= i & -i){
            fenwCount[i] += cntDelta;
            fenwSum[i] += valDelta;
        }
    }
    int queryCount(int i) {
        int s=0;
        for(; i; i-= i & -i) s += fenwCount[i];
        return s;
    }
    long long querySum(int i) {
        long long s=0;
        for(; i; i-= i & -i) s += fenwSum[i];
        return s;
    }
    // getPrefixSum(k) : multiset (오름차순 정렬 시)에서 앞쪽 k개 원소들의 총합을 반환
    long long getPrefixSum(int k) {
        long long res = 0;
        int idx = 0;
        int currCount = 0;
        for (int bit = 1 << 20; bit; bit >>= 1) {
            int next = idx + bit;
            if(next <= n && currCount + fenwCount[next] < k){
                currCount += fenwCount[next];
                res += fenwSum[next];
                idx = next;
            }
        }
        int remain = k - currCount;
        res += (long long) remain * (idx+1);
        return res;
    }
};
 
// 메인 – DSU로 이벤트들을 기록한 후, 모임 i (S = {1,...,i})마다 활성 이벤트(즉, T <= i인 이벤트들)를 
// Fenw 를 통해 관리하며, 현재 활성 이벤트들(총 i-1개)을 오름차순 정렬했을 때, 
// 만약 i <= K이면 모든 간선의 합, i > K이면 (i-1)개 중 뒤쪽 K개 (즉, 순서상 인덱스 [i-K, i-1])의 합을 출력합니다.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, K;
    cin >> N >> K;
    vector<array<int,3>> edges;
    edges.reserve(N-1);
    for (int i=1; i<=N-1; i++){
        int A, B, C;
        cin >> A >> B >> C;
        edges.push_back({A,B,C});
    }
    // DSU 처리를 위해 간선을 가중치 오름차순 정렬
    sort(edges.begin(), edges.end(), [](auto &a, auto &b){
        return a[2] < b[2];
    });
    DSU dsu(N);
    vector<Event> events;
    events.reserve(N-1);
    for(auto &e: edges){
        int a = e[0], b = e[1], w = e[2];
        int actTh = 0;
        if(dsu.uni(a, b, actTh)){
            events.push_back({actTh, w});
        }
    }
    // events의 개수는 정확히 N-1개여야 함.
    // 이제 activation threshold(T) 기준으로 오름차순 정렬
    sort(events.begin(), events.end(), [](const Event &e1, const Event &e2){
        return e1.T < e2.T;
    });
 
    // FenwPair – w의 범위는 최대 N (문제 조건)
    int maxW = N;
    FenwPair fenw(maxW);
 
    int evIndex = 0; // events 배열 인덱스
    // 모임 i = 1..N에 대해 답을 구함
    // DSU의 성질에 의해, meeting i에서는 S={1,...,i}를 연결하는 데 필요한 이벤트가 정확히 i-1개가 됩니다.
    // 이 i-1개의 간선들을 오름차순 정렬했을 때, 
    // 만약 i <= K라면 (모임 정점 수 = i)이므로 f(i,1)+...+f(i,i)= (오직 i-1개의 간선의 합)
    // 만약 i > K라면 f(i,1)+...+f(i,K)= (오름차순 정렬했을 때, 순서 인덱스 [i-K, i-1]에 해당하는 원소들의 합)
    for (int i = 1; i <= N; i++){
        // T == i 인 이벤트들을 모두 Fenw에 삽입
        while(evIndex < (int)events.size() && events[evIndex].T == i){
            int w = events[evIndex].w;
            fenw.update(w, 1, w);
            evIndex++;
        }
        int tot = i - 1; // 현재 활성 이벤트의 개수
        long long ans = 0;
        if(tot <= 0){
            ans = 0;
        } else {
            if(tot < K) {
                ans = fenw.getPrefixSum(tot);
            } else {
                // tot >= K -> 오름차순 정렬했을 때, 위치 L = tot - (K - 1)부터 tot까지의 합
                int L = tot - (K - 1);
                long long sumAll = fenw.getPrefixSum(tot);
                long long sumBefore = fenw.getPrefixSum(L - 1);
                ans = sumAll - sumBefore;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}
