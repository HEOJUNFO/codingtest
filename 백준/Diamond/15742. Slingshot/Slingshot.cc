#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
const ll INF = 1e18;
 
struct Slingshot {
    ll x, y, t;
};
 
struct Query {
    ll a, b;
    int id;
};
 
// Segment Tree for range minimum queries.
struct SegmentTree {
    int n;
    vector<ll> tree;
    SegmentTree(int n) {
        this->n = 1;
        while(this->n < n) this->n *= 2;
        tree.assign(2 * this->n, INF);
    }
    // Point update: 최소값 갱신
    void update(int i, ll val) {
        i += n;
        tree[i] = min(tree[i], val);
        for(i /= 2; i >= 1; i /= 2) {
            tree[i] = min(tree[2*i], tree[2*i+1]);
        }
    }
    // 구간 [l, r] 내 최소값 질의
    ll query(int l, int r) {
        ll res = INF;
        l += n; r += n;
        while(l <= r) {
            if(l % 2 == 1) {
                res = min(res, tree[l]);
                l++;
            }
            if(r % 2 == 0) {
                res = min(res, tree[r]);
                r--;
            }
            l /= 2; r /= 2;
        }
        return res;
    }
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    vector<Slingshot> shots(N);
    for (int i = 0; i < N; i++){
        cin >> shots[i].x >> shots[i].y >> shots[i].t;
    }
    vector<Query> queries(M);
    vector<ll> ans(M, 0);
    for (int j = 0; j < M; j++){
        cin >> queries[j].a >> queries[j].b;
        queries[j].id = j;
        // baseline : 직접 옮기는 시간 = |a - b|
        ans[j] = llabs(queries[j].a - queries[j].b);
    }
 
    // 좌표 압축: 슬링샷의 y좌표와 쿼리의 b를 함께 고려
    vector<ll> comp;
    for (int i = 0; i < N; i++){
        comp.push_back(shots[i].y);
    }
    for (int j = 0; j < M; j++){
        comp.push_back(queries[j].b);
    }
    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());
    auto compress = [&](ll y) -> int {
        return int(lower_bound(comp.begin(), comp.end(), y) - comp.begin());
    };
    int compSize = comp.size();
 
    // Sweep 1: x_i <= a 인 슬링샷에 대해 처리
    {
        vector<Query> qSorted = queries;
        sort(qSorted.begin(), qSorted.end(), [](const Query &q1, const Query &q2){
            return q1.a < q2.a;
        });
        vector<Slingshot> sSorted = shots;
        sort(sSorted.begin(), sSorted.end(), [](const Slingshot &s1, const Slingshot &s2){
            return s1.x < s2.x;
        });
 
        // 두 segment tree: 
        // segLow : y <= b 인 경우 (t - x - y)
        // segHigh: y >= b 인 경우 (t - x + y)
        SegmentTree segLow(compSize), segHigh(compSize);
        int i = 0; // sSorted의 인덱스
        for (auto &q : qSorted) {
            // 슬링샷의 x좌표가 q.a 이하인 경우 모두 추가
            while(i < sSorted.size() && sSorted[i].x <= q.a) {
                int pos = compress(sSorted[i].y);
                segLow.update(pos, sSorted[i].t - sSorted[i].x - sSorted[i].y);
                segHigh.update(pos, sSorted[i].t - sSorted[i].x + sSorted[i].y);
                i++;
            }
            int posB = compress(q.b);
            ll cand = INF;
            // y <= b 인 경우: 비용 = a + b + (t - x - y)
            ll valLow = segLow.query(0, posB);
            if(valLow < INF)
                cand = min(cand, q.a + q.b + valLow);
            // y >= b 인 경우: 비용 = a - b + (t - x + y)
            ll valHigh = segHigh.query(posB, compSize - 1);
            if(valHigh < INF)
                cand = min(cand, q.a - q.b + valHigh);
 
            ans[q.id] = min(ans[q.id], cand);
        }
    }
 
    // Sweep 2: x_i >= a 인 슬링샷에 대해 처리
    {
        vector<Query> qSorted = queries;
        sort(qSorted.begin(), qSorted.end(), [](const Query &q1, const Query &q2){
            return q1.a > q2.a;
        });
        vector<Slingshot> sSorted = shots;
        sort(sSorted.begin(), sSorted.end(), [](const Slingshot &s1, const Slingshot &s2){
            return s1.x > s2.x;
        });
 
        // 두 segment tree: 
        // segLow : y <= b 인 경우 (t + x - y)
        // segHigh: y >= b 인 경우 (t + x + y)
        SegmentTree segLow(compSize), segHigh(compSize);
        int i = 0;
        for (auto &q : qSorted) {
            // 슬링샷의 x좌표가 q.a 이상인 경우 모두 추가
            while(i < sSorted.size() && sSorted[i].x >= q.a) {
                int pos = compress(sSorted[i].y);
                segLow.update(pos, sSorted[i].t + sSorted[i].x - sSorted[i].y);
                segHigh.update(pos, sSorted[i].t + sSorted[i].x + sSorted[i].y);
                i++;
            }
            int posB = compress(q.b);
            ll cand = INF;
            // y <= b 인 경우: 비용 = -a + b + (t + x - y)
            ll valLow = segLow.query(0, posB);
            if(valLow < INF)
                cand = min(cand, -q.a + q.b + valLow);
            // y >= b 인 경우: 비용 = -a - b + (t + x + y)
            ll valHigh = segHigh.query(posB, compSize - 1);
            if(valHigh < INF)
                cand = min(cand, -q.a - q.b + valHigh);
 
            ans[q.id] = min(ans[q.id], cand);
        }
    }
 
    // 최종 결과 출력 (원래 입력 순서대로)
    for (int j = 0; j < M; j++){
        cout << ans[j] << "\n";
    }
 
    return 0;
}
