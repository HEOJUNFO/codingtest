#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<int> fenw;
    Fenwick(int n): n(n), fenw(n+1,0){}
    void update(int i, int v){
        for (; i<=n; i+=i&(-i)) fenw[i]+=v;
    }
    int query(int i){
        int s=0; for (; i>0; i-=i&(-i)) s+=fenw[i];
        return s;
    }
    int range_query(int l, int r){
        if (r<l) return 0;
        return query(r)-query(l-1);
    }
};

// 문자열 접두사 범위 [L,R] 찾기
// strings는 정렬된 사전 배열
pair<int,int> prefix_range(const vector<string> &arr, const string &P){
    // P로 시작하는 범위: 
    // lower_bound로 P 이상인 시작점, upper_bound로 P보다 사전순으로 앞서는 P의 접두사 한정 upper_bound를 찾는다.
    // 정확히는 P를 prefix로 가지는 문자열 범위: 
    // lower = lower_bound(arr.begin(), arr.end(), P)
    // upper = upper_bound(arr.begin(), arr.end(), P + '{')  // '{'는 'Z'보다 뒤, 'U'뒤의 다음 가능한 문자를 크게 잡기 위해
    // 여기서는 가능한 범위 내에서 P와 길이가 달라도 prefix만 확인하면 안되고,
    // 문자열 정렬상에서 P의 prefix를 가지는 모든 문자열은 P 이상이면서 P+"~"(사전상 P 다음 가능한 문자열)보다 작거나 같음
    // 여기선 알파벳이 {A,C,G,U} 이므로 U보다 큰 다음 문자를 만들기 위해 'Z' 또는 'V'같은 큰 문자를 붙일 수 있다.
    // 가장 큰 문자는 'Z'로 가정하자.
    string Pnext = P;
    if (!Pnext.empty()) Pnext.back()++;
    else Pnext.push_back('A'); 
    // 더 안전하게 P + 'Z' 와 같이 해도 됨. 여기서는 Pnext를 P에서 마지막 문자 증가 시켜 prefix 구간 확보.
    // 그러나 'U' 다음 문자는 없으므로 'Z'를 붙이는 것이 안전.
    // 단순하게 Pnext = P + 'Z'; 로 하자.
    Pnext = P + 'Z';

    int L = (int)(lower_bound(arr.begin(), arr.end(), P) - arr.begin()) + 1;
    int R = (int)(lower_bound(arr.begin(), arr.end(), Pnext) - arr.begin());
    if (L > R) return {-1,-1};
    return {L,R};
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N,M; cin>>N>>M;
    vector<string> dict(N);
    for (int i=0; i<N; i++) cin>>dict[i];

    // S 정렬(원문)
    vector<string> S = dict;
    sort(S.begin(), S.end());

    // R 정렬(뒤집어서 정렬)
    vector<string> R = dict;
    for (auto &x: R) reverse(x.begin(), x.end());
    sort(R.begin(), R.end());

    // 각 원문 문자열에 대해 pos, rpos 구하기
    // pos: S에서의 위치
    // rpos: R에서의 위치
    // dict[i]의 pos, rpos를 찾으려면 dict[i]가 S에서 몇 번째인지, R에서 몇 번째인지 알아야 한다.
    // 문자열이 많으므로 map<string,int> 사용 시 메모리/시간 부담. 대신 이중 이분 탐색으로 찾기.
    // N 최대 2,000,000 이므로 이분탐색은 O(logN)으로 가능.

    // pos[i], rpos[i]를 담기 위해 배열 준비
    // dict[i]에 대해 pos[i], rpos[i]
    vector<int> pos(N), rpos(N);
    for (int i=0; i<N; i++){
        // dict[i]의 pos
        int p = (int)(lower_bound(S.begin(), S.end(), dict[i]) - S.begin());
        pos[i] = p+1; // 1-based
        string revd = dict[i]; reverse(revd.begin(), revd.end());
        int rp = (int)(lower_bound(R.begin(), R.end(), revd) - R.begin());
        rpos[i] = rp+1; // 1-based
    }

    // 쿼리 처리
    // 각 쿼리 (P,Q)에 대해:
    // [sp,ep] = prefix_range(S,P)
    // [sq,eq] = prefix_range(R,reverse(Q))
    // 없으면 0 출력
    // 있으면 fenwicks 이용 2D 쿼리:
    // ans = rect_sum(ep,eq) - rect_sum(ep,sq-1) - rect_sum(sp-1,eq) + rect_sum(sp-1,sq-1)

    struct Query {
        int sp,ep,sq,eq;
        bool valid;
    };
    vector<Query> queries(M);
    vector<string> P_arr(M), Q_arr(M);
    for (int i=0; i<M; i++){
        cin >> P_arr[i] >> Q_arr[i];
    }

    // 미리 Q를 뒤집은 문자열 준비
    vector<string> Q_rev(M);
    for (int i=0; i<M; i++){
        Q_rev[i] = Q_arr[i];
        reverse(Q_rev[i].begin(), Q_rev[i].end());
    }

    // 쿼리 범위 구하기
    for (int i=0; i<M; i++){
        auto [sp,ep] = prefix_range(S, P_arr[i]);
        if (sp == -1) {
            queries[i].valid = false;
            continue;
        }
        auto [sq,eq] = prefix_range(R, Q_rev[i]);
        if (sq == -1) {
            queries[i].valid = false;
            continue;
        }
        queries[i].sp = sp;
        queries[i].ep = ep;
        queries[i].sq = sq;
        queries[i].eq = eq;
        queries[i].valid = true;
    }

    // Fenwick 쿼리 준비
    // rect_sum(x,y)를 fenwicks로 처리:
    // rect_sum(x,y): pos≤x,rpos≤y 개수
    // 쿼리당 4개 fenwicks query 필요:
    // ans = f(ep,eq)-f(ep,sq-1)-f(sp-1,eq)+f(sp-1,sq-1)
    // 여기서 f(x,y)= rect_sum(x,y)
    // x: pos dimension
    // pos 기준으로 정렬하여 fenwicks 업데이트
    // 쿼리를 (x,y) 형태로 저장: x는 ep 또는 sp-1, y는 eq 또는 sq-1 형태.

    struct FenwQuery {
        int x,y; 
        int idx;
        int sign; // +1 or -1
    };

    // 최대 4*M FenwQuery
    vector<FenwQuery> fenwQ;
    fenwQ.reserve(4*M);

    // 정리: sq-1 또는 eq, sp-1 또는 ep에서 -1, +1 설정 주의
    // ans = f(ep,eq) - f(ep,sq-1) - f(sp-1,eq) + f(sp-1,sq-1)
    // f(x,y)는 x≥0,y≥0일 때만 의미. x나 y가 0이면 쿼리는 0 처리가능. (fenw index 1-based)
    // y나 x가 0일 때 처리 주의. 여기서 1-based Fenw이므로 y=0이면 쿼리 0.
    // 즉, sq-1 또는 sp-1이 0일 수 있으니 범위 체크 후 쿼리 추가 시 조건문 처리.

    vector<long long> ans(M,0);
    // fenwQ에 쿼리 추가하는 함수
    auto addFenwQuery = [&](int X,int Y,int i,int s) {
        if (X<0 || Y<0) return; // x나 y가 0 미만이면 유효한 쿼리가 아님
        if (X==0 || Y==0) {
            // fenw.query(0) = 0이므로 무시
            return;
        }
        // X,Y는 1-based fenw에서 유효: 이미 pos,rpos를 1-based로 했으니 괜찮음
        if (X>0 && Y>0) fenwQ.push_back({X,Y,i,s});
    };

    for (int i=0; i<M; i++){
        if (!queries[i].valid) {
            ans[i]=0;
            continue;
        }
        int sp=queries[i].sp, ep=queries[i].ep, sq=queries[i].sq, eq=queries[i].eq;
        addFenwQuery(ep,eq,i,+1);
        addFenwQuery(ep,sq-1,i,-1);
        addFenwQuery(sp-1,eq,i,-1);
        addFenwQuery(sp-1,sq-1,i,+1);
    }

    // fenwQ를 x 기준 정렬
    sort(fenwQ.begin(), fenwQ.end(), [](auto &a, auto &b){
        return a.x < b.x;
    });

    // pos기준으로 점들 (pos[i], rpos[i])을 pos순 정렬하기 위해 pos,i 쌍으로 정렬
    // 사실 pos[i]는 i번 문자열에 대한 것이 아니라 i번째로 입력된 문자열에 대한 pos
    // pos[i]는 1..N 범위의 순서를 이미 가지고 있다.
    // i는 그냥 인덱스: 점들을 pos기준 정렬은 간단히 (pos[i], rpos[i])를 pos[i] 오름차순으로 정렬
    vector<pair<int,int>> points(N);
    for (int i=0; i<N; i++){
        points[i] = {pos[i], rpos[i]};
    }
    sort(points.begin(), points.end());

    // Fenwicks 준비
    Fenwick fenw(N);

    int ptr=0; // points를 순차적으로 fenw.update
    for (auto &q : fenwQ) {
        int X = q.x, Y=q.y;
        // pos ≤ X 인 점들 fenw에 업데이트
        while (ptr<N && points[ptr].first <= X) {
            fenw.update(points[ptr].second,1);
            ptr++;
        }
        // 쿼리 처리
        int partial = fenw.query(Y);
        ans[q.idx] += (long long)partial * q.sign;
    }

    // 출력
    for (int i=0; i<M; i++){
        if (!queries[i].valid) cout<<0<<"\n";
        else cout<<ans[i]<<"\n";
    }

    return 0;
}
