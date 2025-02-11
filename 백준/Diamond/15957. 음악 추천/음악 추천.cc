#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// 전역 상수 및 전역 변수
const int NMAX = 100001;
vector<int> children[NMAX];
int inTime[NMAX], outTime[NMAX], subSize[NMAX];
int eulerArr[NMAX];
int timer = 0;
 
// DFS를 통해 Euler Tour 인덱스와 서브트리 크기를 계산한다.
void dfs(int node) {
    inTime[node] = timer;
    eulerArr[timer] = node;
    timer++;
    subSize[node] = 1;
    for (int nxt : children[node]) {
        dfs(nxt);
        subSize[node] += subSize[nxt];
    }
    outTime[node] = inTime[node] + subSize[node] - 1;
}
 
// Fenwick Tree (Binary Indexed Tree)
// – BIT는 1-indexed로 구현하며, 범위 업데이트 & 점 쿼리를 지원한다.
struct Fenw {
    int n;
    vector<ll> fenw;
    Fenw(int n) : n(n) {
        fenw.assign(n+1, 0);
    }
    inline void update(int i, ll delta) {
        for (; i <= n; i += i & -i)
            fenw[i] += delta;
    }
    // [l, r] 구간에 delta만큼 추가
    inline void range_update(int l, int r, ll delta) {
        update(l, delta);
        update(r+1, -delta);
    }
    inline ll query(int i) {
        ll sum = 0;
        for (; i > 0; i -= i & -i)
            sum += fenw[i];
        return sum;
    }
};
 
// 각 이벤트에 대한 구조체: 시간 T, 업데이트할 서브트리의 루트 P, 가중치 S
// 그리고 각 노드에 실제로 추가되는 값 add = S / (서브트리의 노드 개수)
struct Event {
    ll T;
    int P;
    ll S;
    ll add;
};
 
// parallel binary search 에 사용할 쿼리 구조체: 후보 이벤트 인덱스(mid)와 대상 가수(singer)
struct Query {
    int mid;
    int singer;
};
 
// 메인 함수
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, K;
    ll J;
    cin >> N >> K >> J;
    // 1번부터 N번까지의 곡(노드). 
    // 2번 노드부터 부모 노드 번호가 주어진다.
    for (int i = 2; i <= N; i++){
        int par;
        cin >> par;
        children[par].push_back(i);
    }
    // 각 곡의 가수 번호 입력 (1번부터 N번까지)
    vector<int> songSinger(N+1);
    // 가수별로 해당하는 곡(노드)의 Euler 인덱스를 저장하기 위한 벡터 (가수 번호는 최대 N)
    vector<vector<int>> singerNodes(N+1);
    for (int i = 1; i <= N; i++){
        int s;
        cin >> s;
        songSinger[i] = s;
    }
    // Euler Tour 수행: 루트는 1번 노드
    dfs(1);
 
    // 각 곡의 Euler 인덱스를 해당 가수 그룹에 저장
    for (int i = 1; i <= N; i++){
        int singer = songSinger[i];
        singerNodes[singer].push_back(inTime[i]);
    }
    // 각 가수별 곡의 개수
    vector<int> singerCount(N+1, 0);
    for (int i = 1; i <= N; i++){
        singerCount[i] = singerNodes[i].size();
    }
 
    // 이벤트 데이터 입력
    vector<Event> events(K);
    for (int i = 0; i < K; i++){
        ll T;
        int P;
        ll S;
        cin >> T >> P >> S;
        events[i].T = T;
        events[i].P = P;
        events[i].S = S;
        // 서브트리의 크기로 나눈 몫이 각 노드에 부여되는 점수
        events[i].add = S / subSize[P];
    }
    // 이벤트를 시간 T 기준 오름차순 정렬 (시뮬레이션 순서)
    sort(events.begin(), events.end(), [](const Event &a, const Event &b){
        return a.T < b.T;
    });
 
    // 각 가수에 대해 "평균 점수 > J" 가 되는 최초 이벤트를 찾는다.
    // 조건: (누적 점수) > (J * (곡의 개수))
    // singer s의 최종 조건은 total > J * singerCount[s].
    // binary search 범위: 이벤트 인덱스 [0, K-1] 만약 조건을 달성하지 못하면 -1을 출력.
    const int SINGER_MAX = N;
    // lo[s]와 hi[s] : 가수 s에 대해 조건이 성립하지 않는 최대 이벤트 인덱스와 성립하는 최소 인덱스
    vector<int> lo(SINGER_MAX+1, -1), hi(SINGER_MAX+1, K);
 
    // parallel binary search – 최대 약 log(K)번 반복
    for (int iter = 0; iter < 20; iter++){
        vector<Query> queries;
        for (int s = 1; s <= SINGER_MAX; s++){
            if(singerCount[s] == 0) continue;
            if(lo[s] + 1 < hi[s]) {
                int mid = (lo[s] + hi[s]) / 2;
                queries.push_back({mid, s});
            }
        }
        if(queries.empty()) break;
 
        sort(queries.begin(), queries.end(), [](const Query &a, const Query &b){
            return a.mid < b.mid;
        });
 
        Fenw fenw(N); // BIT 초기화 (Euler Tour 크기 N)
        int currentEvent = -1;
        // queries를 mid값 오름차순으로 처리 – BIT에 이벤트를 순차적으로 적용
        for (auto &q: queries) {
            while(currentEvent < q.mid) {
                currentEvent++;
                int node = events[currentEvent].P;
                // Euler 인덱스는 0-indexed이므로 BIT에서는 +1해준다.
                int l = inTime[node] + 1;
                int r = outTime[node] + 1;
                ll addVal = events[currentEvent].add;
                fenw.range_update(l, r, addVal);
            }
            // 가수 q.singer의 모든 곡에 대해 BIT 쿼리 수행
            ll total = 0;
            for (int pos : singerNodes[q.singer]) {
                total += fenw.query(pos + 1);
            }
            // 조건 확인: 누적 점수가 (J * 곡 수) 보다 큰지
            if(total > (ll)J * singerCount[q.singer])
                hi[q.singer] = q.mid;
            else
                lo[q.singer] = q.mid;
        }
    }
 
    // 각 가수에 대해 hi[s]가 K이면 조건을 달성하지 못한 것이며, 그렇지 않으면 해당 이벤트의 시간 T가 답.
    vector<ll> singerAns(SINGER_MAX+1, -1);
    for (int s = 1; s <= SINGER_MAX; s++){
        if(singerCount[s] == 0) continue;
        if(hi[s] == K)
            singerAns[s] = -1;
        else
            singerAns[s] = events[hi[s]].T;
    }
 
    // 1번 곡부터 N번 곡까지, 해당 곡을 부른 가수의 답을 출력 (같은 가수면 같은 값)
    for (int i = 1; i <= N; i++){
        cout << singerAns[songSinger[i]] << "\n";
    }
 
    return 0;
}
