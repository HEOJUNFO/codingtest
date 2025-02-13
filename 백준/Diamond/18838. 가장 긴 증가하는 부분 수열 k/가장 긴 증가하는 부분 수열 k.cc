#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1000000000000000000LL; // 1e18 이상

struct Node {
    int len;
    ll cnt;
};

int N;
ll K;
vector<int> A;
vector<int> dp;       // 각 위치에서의 최장 길이
vector<ll> ways;      // 각 위치에서의 경우의 수

// 세그먼트 트리 (값의 범위: 1 ~ N)
// 각 리프는 A의 값에 대응하며, (최장 길이, 경우의 수) 를 저장합니다.
int segSize;
vector<Node> segTree;

Node combine(Node a, Node b) {
    if(a.len > b.len) return a;
    else if(a.len < b.len) return b;
    else {
        Node ret;
        ret.len = a.len;
        ret.cnt = a.cnt + b.cnt;
        if(ret.cnt > K) ret.cnt = K; // K 이상이면 K로 고정
        return ret;
    }
}

void initSegTree(int n) {
    segSize = 1;
    while(segSize < n) segSize *= 2;
    segTree.assign(segSize*2, {0,0});
}

void update(int pos, Node val) {
    pos += segSize - 1;
    // pos는 유일하므로 그냥 대입 (단, 같은 값이면 cnt를 더함)
    if(val.len > segTree[pos].len) segTree[pos] = val;
    else if(val.len == segTree[pos].len)
    {
        segTree[pos].cnt += val.cnt;
        if(segTree[pos].cnt > K) segTree[pos].cnt = K;
    }
    for(pos /= 2; pos >= 1; pos /= 2) {
        segTree[pos] = combine(segTree[pos*2], segTree[pos*2+1]);
    }
}

Node query(int l, int r, int idx, int nl, int nr) {
    if(r < nl || nr < l) return {0,0};
    if(l <= nl && nr <= r) return segTree[idx];
    int mid = (nl + nr) / 2;
    Node left = query(l, r, idx*2, nl, mid);
    Node right = query(l, r, idx*2+1, mid+1, nr);
    return combine(left, right);
}

Node query(int l, int r) {
    return query(l, r, 1, 1, segSize);
}

/*
  기존 reconstruct()는 재귀마다 [start, N) 전체를 순회했으므로
  최악의 경우 O(N^2) 시간이 걸릴 수 있습니다.
  대신, dp 값 별 후보를 미리 모아두면 전체 후보들의 합은 O(N)입니다.
  각 dp 길이 r에 대해, cand[r]에는 dp[i]==r 인 인덱스 i에 대해 (A[i], i, ways[i])
  를 저장하며, A[i] 오름차순으로 정렬합니다.
  reconstruct2()는 dp 길이 remain에 해당하는 후보 벡터만 순회하여
  index 조건(i >= start)과 A값 조건(A[i] > prev)을 만족하는 후보들만 고려합니다.
*/

vector<vector<tuple<int,int,ll>>> cand; // cand[r]: dp 값 r인 후보 (값, 원래 인덱스, ways)

void reconstruct2(int prev, int start, int remain, ll &k, vector<int> &res) {
    if(remain == 0) return;
    // dp 값 remain인 후보 벡터는 A값 오름차순으로 정렬되어 있으므로 사전순 탐색 가능
    for (auto &t : cand[remain]) {
        int val, idx;
        ll cnt;
        tie(val, idx, cnt) = t;
        if(idx < start || val <= prev) continue;
        if(cnt < k) {
            k -= cnt;
        } else {
            res.push_back(val);
            reconstruct2(val, idx+1, remain-1, k, res);
            return;
        }
    }
}
  
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> K;
    A.resize(N);
    for (int i = 0; i < N; i++){
        cin >> A[i];
    }
    
    dp.resize(N);
    ways.resize(N);
    
    // 세그먼트 트리 초기화 (값 범위는 1 ~ N)
    initSegTree(N);
    
    // 오른쪽부터 dp, ways 계산
    // 각 위치 i에 대해, query( A[i]+1, N )을 통해 A[i]보다 큰 값들에서 얻을 수 있는 최장 길이와 경우의 수를 구함.
    for (int i = N-1; i >= 0; i--){
        int pos = A[i]; // A[i] ∈ [1, N]
        Node resNode = {0, 0};
        if(pos < N) {
            resNode = query(pos+1, N);
        }
        dp[i] = resNode.len + 1;
        ways[i] = (resNode.len == 0 ? 1LL : resNode.cnt);
        if(ways[i] > K) ways[i] = K; // K 이상이면 K로 고정
        update(pos, {dp[i], ways[i]});
    }
    
    // 전체 LIS의 길이 L와 총 경우의 수 계산
    int L = 0;
    ll total = 0;
    for (int i = 0; i < N; i++){
        if(dp[i] > L) {
            L = dp[i];
            total = ways[i];
        } else if(dp[i] == L) {
            total += ways[i];
            if(total > K) total = K;
        }
    }
    
    if(total < K) {
        cout << -1 << "\n";
        return 0;
    }
    
    // 각 dp 길이별 후보 모으기
    // cand[r]에는 dp[i]==r인 인덱스 i에 대해 (A[i], i, ways[i]) 저장
    cand.assign(L+1, vector<tuple<int,int,ll>>());
    for (int i = 0; i < N; i++){
        int d = dp[i];
        cand[d].push_back({A[i], i, ways[i]});
    }
    // 사전순(즉, A값 오름차순)으로 정렬
    for (int d = 1; d <= L; d++){
        sort(cand[d].begin(), cand[d].end(), [](const tuple<int,int,ll> &a, const tuple<int,int,ll> &b){
            return get<0>(a) < get<0>(b);
        });
    }
    
    vector<int> ans;
    ll k = K;
    reconstruct2(-1, 0, L, k, ans);
    
    for (int i = 0; i < ans.size(); i++){
        cout << ans[i] << (i+1==ans.size() ? "\n" : " ");
    }
    return 0;
}
