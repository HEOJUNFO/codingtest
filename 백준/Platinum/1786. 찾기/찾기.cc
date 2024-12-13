#include <bits/stdc++.h>
using namespace std;

// KMP 알고리즘을 이용한 문자열 매칭
// T에서 P를 찾아서 P가 나타나는 모든 시작 위치를 반환한다.

// 접두사이면서 접미사인 최대 길이를 담는 파이 배열(pi)을 계산하는 함수
vector<int> getPi(const string& P) {
    int m = (int)P.size();
    vector<int> pi(m, 0);
    int j = 0;
    for(int i = 1; i < m; i++) {
        while(j > 0 && P[i] != P[j]) {
            j = pi[j-1];
        }
        if(P[i] == P[j]) {
            pi[i] = ++j;
        }
    }
    return pi;
}

// KMP 매칭 함수
// 문자열 T에서 문자열 P가 나타나는 (1-based) 시작 위치를 반환한다.
vector<int> kmpSearch(const string& T, const string& P) {
    vector<int> result;
    auto pi = getPi(P);
    int n = (int)T.size();
    int m = (int)P.size();
    int j = 0;
    for(int i = 0; i < n; i++) {
        while(j > 0 && T[i] != P[j]) {
            j = pi[j-1];
        }
        if(T[i] == P[j]) {
            if(j == m-1) {
                // 매칭 발생 (i - (m-1))가 시작 인덱스 (0-based)
                result.push_back(i - m + 2); 
                // 다음 매칭을 위해 j 이동
                j = pi[j];
            } else {
                j++;
            }
        }
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string T, P;
    // 문제에서 공백도 포함한다고 했으므로, getline으로 입력받는다.
    {
        string temp;
        // 첫째 줄: T
        // 문자열 앞뒤 공백 없다고 가정, 문제 정의상 T, P는 별도의 줄
        // 가능한 모든 문자열을 그대로 받아야 하므로 getline 사용
        getline(cin, T);
        // 둘째 줄: P
        getline(cin, P);
    }

    auto matches = kmpSearch(T, P);
    cout << matches.size() << "\n";
    for (int pos : matches) {
        cout << pos << " ";
    }
    cout << "\n";

    return 0;
}
