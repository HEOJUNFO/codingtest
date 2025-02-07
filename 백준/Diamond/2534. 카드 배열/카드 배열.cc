#include <iostream>
#include <vector>
#include <queue>
using namespace std;
 
// 모듈러 상수
const int MOD = 1000000007;
 
// 빠른 거듭제곱 (base^power mod m)
long long fast_pow(int base, int power, int m) {
    long long res = 1, t = base % m;
    while(power > 0) {
        if(power & 1)
            res = (res * t) % m;
        power >>= 1;
        t = (t * t) % m;
    }
    return res;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    // 전체 카드 수 N, 선택할 카드 수 k, 제약조건의 개수 P
    int N, k, P;
    cin >> N >> k >> P;
 
    // 최대값을 구할 때 사용할 그래프 (역방향: 제약조건 a > b → b->a)
    // 최소값을 구할 때 사용할 그래프 (원래 방향: a->b)
    vector<vector<int>> gph_max(k), gph_min(k);
    // indegree 배열
    vector<int> indeg_max(k, 0), indeg_min(k, 0);
    for (int i = 0; i < P; i++){
        int a, b;
        cin >> a >> b;
        // a > b 라는 제약조건.
        // 최대값: 역방향으로 b에서 a로 연결
        gph_max[b].push_back(a);
        indeg_max[a]++;
 
        // 최소값: 원래 방향으로 a->b
        gph_min[a].push_back(b);
        indeg_min[b]++;
    }
 
    // val[i]는 최종적으로 (최대 배정에서 할당된 카드 – 최소 배정에서 할당된 카드)를 저장
    // (즉, i번 자리에 할당된 값의 차이)
    vector<int> val(k, 0);
 
    // --- 최대 배정 (사용할 값은 {N-k, N-k+1, …, N-1})
    // 위상정렬 (우선순위 큐: 오름차순으로 처리)
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < k; i++){
        if(indeg_max[i] == 0)
            pq.push(i);
    }
    // cur는 할당할 카드 값 (N-k부터 시작)
    int cur = N - k - 1;
    while(!pq.empty()){
        int t = pq.top();
        pq.pop();
        val[t] = ++cur;  // t번 자리(즉, 카드 C_t에 할당된 최대값)
        for (int nxt : gph_max[t]){
            if(--indeg_max[nxt] == 0)
                pq.push(nxt);
        }
    }
 
    // --- 최소 배정 (사용할 값은 {0, 1, …, k-1})
    // 재사용을 위해 pq를 비워두고 다시 채웁니다.
    while(!pq.empty()) pq.pop();
    for (int i = 0; i < k; i++){
        if(indeg_min[i] == 0)
            pq.push(i);
    }
    cur = k; // 할당할 값은 k-1부터 내려갈 것이므로 cur를 k로 둡니다.
    while(!pq.empty()){
        int t = pq.top();
        pq.pop();
        // 최소 배정에서 t번 자리에 할당되는 값는 (--cur)
        // 두 배정의 차이를 val[t]에 반영: (최대 - 최소)
        val[t] -= --cur;
        for (int nxt : gph_min[t]){
            if(--indeg_min[nxt] == 0)
                pq.push(nxt);
        }
    }
 
    // 최종 차이값 = Σ (val[i] * N^(i))  (i=0: C₀ (일의 자리), ..., i=k-1: Cₖ₋₁ (가장 높은 자리))
    // 문제에서는 카드배열의 수의 값이 Cₖ₋₁×N^(k-1) + ... + C₀×N⁰ 이므로,
    // 각 자릿수의 차이를 해당 자리 가중치와 곱해 합산합니다.
    long long ans = 0;
    for (int i = 0; i < k; i++){
        ans = (ans + 1LL * val[i] * fast_pow(N, i, MOD)) % MOD;
    }
 
    cout << ans % MOD << "\n";
    return 0;
}
