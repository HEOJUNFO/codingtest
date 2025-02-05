#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
// 전역 배열 A: 각 정점에 적힌 값.
vector<int> A;
 
// find_min: 두 그룹 L과 R에서 하위 bit들만 고려하여 
// A[x]와 A[y] (x in L, y in R) 간의 최소 XOR 값을 구한다.
ll find_min(const vector<int>& L, const vector<int>& R, int bit) {
    if(bit < 0) return 0;
    vector<int> L0, L1, R0, R1;
    for (int idx : L) {
        if( (A[idx] >> bit) & 1 )
            L1.push_back(idx);
        else
            L0.push_back(idx);
    }
    for (int idx : R) {
        if( (A[idx] >> bit) & 1 )
            R1.push_back(idx);
        else
            R0.push_back(idx);
    }
    ll res = LLONG_MAX;
    bool candidateFound = false;
    // 만약 같은 분류(0-0)에서 pairing이 가능하면 재귀적으로 하위비트를 처리
    if(!L0.empty() && !R0.empty()){
        res = min(res, find_min(L0, R0, bit-1));
        candidateFound = true;
    }
    // 또는 1-1인 경우
    if(!L1.empty() && !R1.empty()){
        res = min(res, find_min(L1, R1, bit-1));
        candidateFound = true;
    }
    if(candidateFound) return res;
    // 같은 분류로 매칭이 불가능하면, 반드시 현재 비트에서 차이가 발생하므로 (1<<bit) 비용 추가
    if(!L0.empty() && !R1.empty()){
        res = min(res, (1LL << bit) + find_min(L0, R1, bit-1));
    }
    if(!L1.empty() && !R0.empty()){
        res = min(res, (1LL << bit) + find_min(L1, R0, bit-1));
    }
    return res;
}
 
// solveMST: 인덱스 리스트 indices에 대해, 
// A[i] xor A[j] 가 가중치인 완전 그래프의 MST 비용을 현재 bit부터 계산한다.
ll solveMST(const vector<int>& indices, int bit) {
    if(indices.size() <= 1 || bit < 0)
        return 0;
    vector<int> group0, group1;
    // 현재 bit를 기준으로 그룹 분할: bit가 0이면 group0, 1이면 group1.
    for (int idx : indices) {
        if( (A[idx] >> bit) & 1 )
            group1.push_back(idx);
        else
            group0.push_back(idx);
    }
    ll cost = 0;
    cost += solveMST(group0, bit-1);
    cost += solveMST(group1, bit-1);
    // 두 그룹 모두 정점이 존재하면, 이 둘을 잇는 최소 간선의 비용 추가
    if(group0.empty() || group1.empty())
        return cost;
    ll cross = (1LL << bit) + find_min(group0, group1, bit-1);
    return cost + cross;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n;
    cin >> n;
    A.resize(n);
    for (int i = 0; i < n; i++){
        cin >> A[i];
    }
    // 전체 정점을 인덱스 리스트에 담는다.
    vector<int> indices(n);
    for (int i = 0; i < n; i++){
        indices[i] = i;
    }
    // A[i] < 2^30 이므로 최상위 비트 30부터 시작
    ll ans = solveMST(indices, 30);
    cout << ans << "\n";
    return 0;
}
