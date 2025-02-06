#include <bits/stdc++.h>
using namespace std;
 
// a_i, m 최대 1e9 이므로 INF는 그보다 큰 값 (1e9+1)
const int INF = 1000000001; 
// 질의에서 k의 최대값이 100000이므로 DP 배열의 크기를 100000으로 설정
const int MAX_K = 100000;
 
struct Item {
    int c, a, b;
};
 
struct Query {
    int m, k, s; 
    int T; // T = m + s + 1 : 안전 조건(b_i > m+s)와 동치
    int idx; // 원래 질의 순서 저장
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n; 
    cin >> n;
    vector<Item> items(n);
    for (int i = 0; i < n; i++){
        cin >> items[i].c >> items[i].a >> items[i].b;
    }
    // 아이템을 회수시간 b 내림차순으로 정렬 
    sort(items.begin(), items.end(), [](const Item &i1, const Item &i2){
        return i1.b > i2.b;
    });
 
    int p; 
    cin >> p;
    vector<Query> queries(p);
    for (int i = 0; i < p; i++){
        int m, k, s;
        cin >> m >> k >> s;
        queries[i].m = m;
        queries[i].k = k;
        queries[i].s = s;
        // 강도 계획의 종료 시간 m+s 까지 아무도 나타나면 안되므로, 안전 조건은 b >= m+s+1
        queries[i].T = m + s + 1;
        queries[i].idx = i;
    }
    // 질의를 T 내림차순으로 정렬 (즉, 더 까다로운 조건부터 처리)
    sort(queries.begin(), queries.end(), [](const Query &q1, const Query &q2){
        return q1.T > q2.T;
    });
 
    // dp[x] : 현재까지 추가된 “안전한” 아이템들(모두 b_i >= 현재 임계값)로 합 x를 만들 때 사용한 아이템들 중 최대 drop‐off 시간의 최솟값
    // dp[0] = 0 (아무것도 선택하지 않으면 최대 drop-off 시간은 0), 그 외는 INF.
    vector<int> dp(MAX_K+1, INF);
    dp[0] = 0;
 
    int itemPtr = 0;
    int totalItems = n;
    // 각 질의의 결과를 원래 순서대로 저장
    vector<string> answer(p);
 
    // 질의를 내림차순 순으로 처리
    for (int qi = 0; qi < p; qi++){
        // 현재 질의의 T (m+s+1) 보다 큰 (또는 같은) b_i를 가진 아이템들을 모두 dp에 추가한다.
        while(itemPtr < totalItems && items[itemPtr].b >= queries[qi].T){
            int c = items[itemPtr].c;
            int a = items[itemPtr].a;
            // 0/1 knapSack 업데이트 : dp[x] = min( dp[x] , max(dp[x-c], a) )
            for (int x = MAX_K; x >= c; x--){
                // 만약 dp[x-c]가 만들어질 수 있다면
                int prev = dp[x - c];
                if(prev < INF){
                    int candidate = max(prev, a);
                    if(candidate < dp[x])
                        dp[x] = candidate;
                }
            }
            itemPtr++;
        }
 
        // 현재 dp 배열은 안전 집합 S(T) (즉, b_i >= queries[qi].T 인 아이템들)를 반영함.
        // 질의 조건은: m 시점에 도난 진행하며, 선택한 아이템들이 모두 m 이하에 drop-off 되어야 하므로,
        // dp[k] (선택한 아이템들의 최대 drop-off 시각)가 m 이하인지 확인.
        if(dp[queries[qi].k] <= queries[qi].m)
            answer[queries[qi].idx] = "TAK";
        else
            answer[queries[qi].idx] = "NIE";
    }
 
    // 원래 질의 순서대로 출력
    for (int i = 0; i < p; i++){
        cout << answer[i] << "\n";
    }
 
    return 0;
}
