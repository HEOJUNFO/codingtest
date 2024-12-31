#include <bits/stdc++.h>
using namespace std;

/**
 * KMP Prefix function을 계산하는 함수
 * S: 분석할 문자열
 * pi[i]: 문자열 S[0..i]에서의 접두사이면서 접미사인 최대 길이를 저장
 */
vector<int> computePrefixFunction(const string &S) {
    int n = (int)S.size();
    vector<int> pi(n, 0);
    int j = 0;  // 가장 긴 접두사-접미사의 길이 후보

    // i: 현재 위치(1부터 시작), j: pi[i-1] 값 이용
    for(int i = 1; i < n; i++){
        // 현재 문자가 맞지 않을 경우, 이전 가능한 위치로 j를 이동
        while(j > 0 && S[i] != S[j]) {
            j = pi[j - 1];
        }
        // 맞으면 j를 1 증가
        if(S[i] == S[j]) {
            j++;
            pi[i] = j;
        }
    }
    return pi;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    cin >> S;
    int n = (int)S.size();

    // 1. KMP의 prefix function(= pi 배열) 계산
    vector<int> pi = computePrefixFunction(S);

    // 2. 보더(= Prefix이자 Suffix)의 길이들을 찾기
    //    - pi[n-1]에서 시작해 거슬러 올라가며, 가능한 모든 보더 길이를 저장
    vector<int> borders;
    int k = pi[n - 1];  // 문자열 S의 마지막 위치에서의 pi 값
    while(k > 0) {
        borders.push_back(k);
        k = pi[k - 1];  // 연쇄적으로 거슬러 올라가기
    }
    // 문자열 전체 길이 n도 보더로 포함(문제 요구 사항)
    borders.push_back(n);

    // 보더 길이를 오름차순으로 정렬
    sort(borders.begin(), borders.end());

    // 3. 각 보더(또는 접두사) 길이가 S 내에서 몇 번 등장하는지 계산
    //    (길이 i인 접두사가 문자열 내에서 몇 번 등장하는지 세기)
    vector<int> count(n + 1, 0);
    for(int i = 0; i < n; i++){
        count[ pi[i] ]++;
    }
    // 누적(큰 i부터 작은 i 방향) -> 자신의 count를 pi[i-1]에 누적
    for(int i = n; i > 0; i--){
        count[ pi[i - 1] ] += count[i];
    }
    // 각 길이 i에 대해, 실제 등장 횟수는 count[i] + 1 (자기 자신 등장 포함)
    //  -> 문제에서 "그 문자열 그 자체도 세야 하므로" +1

    // 출력 단계
    cout << borders.size() << "\n";
    for(auto &len : borders){
        cout << len << " " << count[len] + 1 << "\n";
    }

    return 0;
}
