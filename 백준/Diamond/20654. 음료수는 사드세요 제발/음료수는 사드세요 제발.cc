#include <bits/stdc++.h>
using namespace std;
 
// 최대 가격 (p_i 최대값)
const int MAXP = 100000;
 
// BIT (Fenwick Tree) : 1-indexed, 두 BIT를 사용함 (용량과 비용)
struct BIT {
    int n;
    vector<long long> vol;  // 누적 용량
    vector<long long> cost; // 누적 비용 (각 원소는 price * volume)
    BIT(int n): n(n) {
        vol.assign(n+1, 0);
        cost.assign(n+1, 0);
    }
    void init(int n_) {
        n = n_;
        vol.assign(n+1, 0);
        cost.assign(n+1, 0);
    }
    void update(int idx, long long deltaVol, long long deltaCost) {
        for(; idx <= n; idx += idx & -idx){
            vol[idx] += deltaVol;
            cost[idx] += deltaCost;
        }
    }
    long long queryVol(int idx) {
        long long res = 0;
        for(; idx > 0; idx -= idx & -idx)
            res += vol[idx];
        return res;
    }
    long long queryCost(int idx) {
        long long res = 0;
        for(; idx > 0; idx -= idx & -idx)
            res += cost[idx];
        return res;
    }
    // 누적 용량이 target 이상이 되는 최소 가격 인덱스를 찾는다.
    int lower_bound(long long target) {
        int idx = 0;
        int bit = 1 << 17; // MAXP <= 1e5이므로 2^17=131072면 충분함.
        for(; bit; bit >>= 1) {
            int next = idx + bit;
            if(next <= n && vol[next] < target) {
                target -= vol[next];
                idx = next;
            }
        }
        return idx + 1;
    }
};
 
// 액체 정보
struct Liquid {
    int d, p;     // 맛, 가격 (1리터당)
    long long l;  // 최대 사용량
};
 
// 사람(쿼리) 정보
struct Query {
    long long g; // 총 가격 제한
    long long L; // 목표 용량
    int id;
};
 
// 메인 아이디어: 
// 음료수의 맛은 사용된 액체들의 맛 중 최솟값이므로, 
// 어떤 맛 x를 “최소 맛 기준”으로 한다면 d_i >= x 인 액체들만 사용할 수 있다.
// 이때, “최소 비용”으로 L리터를 채울 수 있는지 BIT를 이용하여 구한다.
// 액체의 가격별로 용량(l_i)와 비용(p_i * l_i)를 BIT에 업데이트한 후, 
// 낮은 가격부터 L리터를 채워나가는 그리디한 방식으로 비용을 구한다.
// 조건을 만족하는지 여부는 (총 비용 <= g)로 판별하며, 조건이 단조적이므로 병렬 이진 탐색으로 최대 맛을 찾는다.
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m;
    cin >> n >> m;
    vector<Liquid> liquids(n);
    int maxTaste = 0;
    for (int i = 0; i < n; i++){
        cin >> liquids[i].d >> liquids[i].p >> liquids[i].l;
        maxTaste = max(maxTaste, liquids[i].d);
    }
 
    vector<Query> queries(m);
    for (int i = 0; i < m; i++){
        cin >> queries[i].g >> queries[i].L;
        queries[i].id = i;
    }
 
    // 각 쿼리에 대해 이분탐색 구간 [lo, hi] (답이 0이면 불가능)
    vector<int> lo(m, 0), hi(m, maxTaste);
 
    // 병렬 이진 탐색: 약 log2(maxTaste) (최대 17~20회 반복)
    for (int iter = 0; iter < 20; iter++){
        // 현재 범위에서 결정되지 않은 쿼리들의 후보 taste(mid)와 id를 모은다.
        vector<pair<int,int>> bucket; // {candidate taste, query id}
        for (int i = 0; i < m; i++){
            if(lo[i] < hi[i]){
                int mid = (lo[i] + hi[i] + 1) / 2;
                bucket.push_back({mid, i});
            }
        }
        if(bucket.empty()) break;
 
        // candidate taste 내림차순으로 정렬 (taste가 높을수록 액체의 제약이 크므로 BIT 업데이트 순서를 맞추기 위함)
        sort(bucket.begin(), bucket.end(), [](auto &a, auto &b){
            return a.first > b.first;
        });
 
        // 액체들을 맛 내림차순으로 정렬
        sort(liquids.begin(), liquids.end(), [](const Liquid &a, const Liquid &b){
            return a.d > b.d;
        });
 
        // BIT 초기화 (가격 1 ~ MAXP)
        BIT bit(MAXP);
 
        int liqPtr = 0;
        // bucket에 있는 각 쿼리에 대해, candidate taste 보다 맛이 높은 액체들을 BIT에 추가한 후 
        // L리터를 채울 때의 최소 비용을 구하여 g와 비교한다.
        for(auto &entry : bucket){
            int cand = entry.first;
            int qid = entry.second;
            // 맛이 cand 이상인 액체들을 BIT에 추가
            while(liqPtr < n && liquids[liqPtr].d >= cand){
                int price = liquids[liqPtr].p;
                long long volume = liquids[liqPtr].l;
                // BIT update : 해당 가격에서 volume과 (price*volume)를 추가
                bit.update(price, volume, (long long)price * volume);
                liqPtr++;
            }
            long long required = queries[qid].L;
            long long totVol = bit.queryVol(MAXP);
            if(totVol < required){
                // L리터를 채울 수 없으므로 candidate taste 불가능
                hi[qid] = cand - 1;
            } else {
                // BIT를 이용하여 낮은 가격부터 L리터 채우는 최소 비용 계산
                int pos = bit.lower_bound(required);
                long long volBefore = bit.queryVol(pos - 1);
                long long costBefore = bit.queryCost(pos - 1);
                long long remain = required - volBefore;
                long long curCost = costBefore + (long long) pos * remain;
                if(curCost <= queries[qid].g) {
                    lo[qid] = cand; // 가능하므로 taste 후보 갱신
                } else {
                    hi[qid] = cand - 1;
                }
            }
        }
    }
 
    // 최종적으로 lo[i]가 0이면 음료수를 대접할 수 없으므로 -1, 아니면 lo[i]가 답.
    for (int i = 0; i < m; i++){
        if(lo[i] == 0)
            cout << -1 << "\n";
        else
            cout << lo[i] << "\n";
    }
 
    return 0;
}
