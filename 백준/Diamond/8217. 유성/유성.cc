#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// BIT: 1-indexed. 여기서는 점 쿼리(각 구역의 총합) 지원.
struct BIT {
    int n;
    vector<ll> tree;
    BIT(int n) : n(n) {
        tree.assign(n+1, 0);
    }
    inline void update(int i, ll delta) {
        for(; i <= n; i += i & -i)
            tree[i] += delta;
    }
    inline ll query(int i) {
        ll s = 0;
        for(; i > 0; i -= i & -i)
            s += tree[i];
        return s;
    }
    void reset() {
        fill(tree.begin(), tree.end(), 0);
    }
};
 
struct Update {
    int l, r;
    ll a;
};
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    // 1번부터 M번까지 구역의 소유국 번호 (구역 번호 1~M)
    vector<int> owner(M+1);
    for (int i = 1; i <= M; i++){
        cin >> owner[i];
    }
    // 각 국가의 목표 운석 샘플 수량 (국가 번호 1~N)
    vector<ll> target(N+1);
    for (int j = 1; j <= N; j++){
        cin >> target[j];
    }
    int Q;
    cin >> Q;
    // Q일간의 유성우 예보 (날짜 1~Q)
    vector<Update> updates(Q+1);
    for (int i = 1; i <= Q; i++){
        int l, r;
        ll a;
        cin >> l >> r >> a;
        updates[i] = {l, r, a};
    }
 
    // 각 국가가 소유한 구역의 위치 저장
    vector<vector<int>> positions(N+1);
    for (int i = 1; i <= M; i++){
        int country = owner[i];
        positions[country].push_back(i);
    }
 
    // 각 국가별 이분탐색 경계: low[j] ~ high[j]
    // 만약 목표 달성이 불가능하면 최종 low[j]는 Q+1이 됨.
    vector<int> low(N+1, 1), high(N+1, Q+1);
 
    // 병렬 이분탐색을 위한 bucket:
    // bucket[mid]에는 mid일을 검사해야 하는 국가의 번호들을 저장.
    vector<vector<int>> bucket(Q+2);
 
    // BIT의 크기는 구역 번호 최대 M에 대해 업데이트가 M+1번 인덱스까지 필요하므로.
    BIT bit(M+1);
 
    // 모든 국가에 대해 low < high 인 동안 계속 이분탐색 수행
    bool still = true;
    while(still) {
        still = false;
        // bucket 초기화 (1일 ~ Q일)
        for (int i = 1; i <= Q; i++){
            bucket[i].clear();
        }
        // 아직 답이 결정되지 않은 국가에 대해 현재 mid값 산출
        for (int j = 1; j <= N; j++){
            if(low[j] < high[j]){
                still = true;
                int mid = (low[j] + high[j]) / 2;
                bucket[mid].push_back(j);
            }
        }
        // BIT를 초기화 후, 날짜별로 업데이트 적용하며 bucket에 있는 국가의 상태 검사
        bit.reset();
        // day: 1일부터 Q일까지 순서대로 진행
        for (int day = 1; day <= Q; day++){
            // 해당 일의 유성우 업데이트 적용
            int l = updates[day].l, r = updates[day].r;
            ll a = updates[day].a;
            if(l <= r) {
                // 구간 [l, r]에 a 추가
                bit.update(l, a);
                if(r+1 <= M+1) bit.update(r+1, -a);
            } else {
                // 원형 구간: [l, M]과 [1, r]
                bit.update(l, a);
                bit.update(M+1, -a);
                bit.update(1, a);
                if(r+1 <= M+1) bit.update(r+1, -a);
            }
 
            // day일이 mid값인 국가들에 대해 현재까지의 누적 샘플 수 계산
            for (int country : bucket[day]) {
                ll sum = 0;
                // 해당 국가가 소유한 각 구역에 대해 BIT 쿼리 수행
                for (int pos : positions[country]) {
                    sum += bit.query(pos);
                    if(sum >= target[country]) break; // 목표 달성 여부를 미리 판단
                }
                if(sum >= target[country])
                    high[country] = day;
                else
                    low[country] = day + 1;
            }
        }
    }
 
    // 결과 출력: low[j]가 Q+1이면 목표 달성이 불가능하므로 "NIE" 출력.
    for (int j = 1; j <= N; j++){
        if(low[j] == Q+1)
            cout << "NIE" << "\n";
        else
            cout << low[j] << "\n";
    }
 
    return 0;
}
