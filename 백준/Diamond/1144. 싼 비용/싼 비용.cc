#include <bits/stdc++.h>
using namespace std;

constexpr int INF = 1000000007; 
int n, m;
int v[9][9];
map<string, int> cache[9][9];

// 상태 문자열을 정규화하는 함수
// 같은 형태의 연결 상태를 동일한 문자열로 표현하기 위함
string Norm(const string& s) {
    char check[10]{};
    int t = 1;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '0' || check[s[i] & 15]) continue;
        check[s[i] & 15] = (char)(t++ + '0');
    }
    string ret(s.size(), '0');
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '0') continue;
        ret[i] = check[s[i] & 15];
    }
    return ret;
}

// 현재 칸을 선택하는 경우 연결 상태를 갱신하는 함수
// y가 0이 아닌 경우(즉 같은 행에서 왼쪽 칸이 존재하는 경우)
string Merge(const string& s) {
    string ret = s;
    ret.erase(ret.begin()); // 맨 앞의 연결 상태 제거(윗줄 상태 반영)
    // 뒤에 새로운 연결 상태 추가
    if (s[0] == '0' && s.back() == '0') ret.push_back('9');
    else if (s[0] == '0') ret.push_back(s.back());
    else if (s.back() == '0' || s[0] == s.back()) ret.push_back(s[0]);
    else {
        ret.push_back(s[0]);
        for (auto &c : ret) if (c == s.back()) c = s[0];
    }
    return Norm(ret);
}

// 현재 칸을 선택하는 경우 연결 상태를 갱신하는 함수
// y가 0인 경우(행의 시작 칸)
string Merge2(const string& s) {
    string ret = s;
    ret.erase(ret.begin());
    if (s[0] == '0') ret.push_back('9');
    else ret.push_back(s[0]);
    return Norm(ret);
}

// 위쪽에 남는 연결 요소가 없는지 체크하는 함수
// 즉, 현재 상태에서 다음 상태로 넘어갈 때 위에 걸쳐진 연결 요소가 없도록 해야 함
bool CheckPass(const string& s) {
    if (s[0] == '0') return true;
    for (int i = 1; i < (int)s.size(); i++)
        if (s[i] == s[0]) return true;
    return false;
}

// 현재 상태가 유효한지 체크(연결 요소가 1개 이하인지)
bool CheckValid(const string& s) {
    char check[10]{};
    for (auto c : s) check[c & 15] = 1;
    int cnt = 0;
    for (int i = 1; i < 10; i++) if (check[i]) cnt++;
    return cnt <= 1;
}

// DFS를 이용한 DP
// x, y: 현재 확인중인 칸의 좌표
// cur: 현재 상태를 나타내는 문자열
int DFS(int x, int y, string cur) {
    // 모든 행을 다 확인한 경우
    if (x == n) {
        // 모든 칸을 봤다면, 남아있는 연결 요소가 1개 이하인지 체크
        return CheckValid(cur) ? 0 : INF;
    }
    cur = Norm(cur);
    if (cache[x][y].count(cur)) return cache[x][y][cur];

    int &ret = cache[x][y][cur] = INF;
    int nx = x, ny = y + 1;
    if (ny >= m) {
        nx++;
        ny = 0;
    }

    // 현재 칸을 선택하지 않는 경우
    // 위쪽에 남아있는 연결 요소가 없어야 함 (CheckPass)
    if (CheckPass(cur)) {
        string nxt = cur;
        nxt.erase(nxt.begin());
        nxt.push_back('0');
        ret = min(ret, DFS(nx, ny, nxt));
    }

    // 현재 칸을 선택하는 경우
    if (y > 0) {
        // Merge 함수로 연결 상태 갱신
        ret = min(ret, DFS(nx, ny, Merge(cur)) + v[x][y]);
    } else {
        // 행 시작 칸일 경우 Merge2 사용
        ret = min(ret, DFS(nx, ny, Merge2(cur)) + v[x][y]);
    }

    // 아무것도 선택하지 않는 경우 최적일 수도 있음
    // 남은 상태가 유효하다면 0으로 갱신 시도
    if (CheckValid(cur)) ret = min(ret, 0);

    return ret;
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    cin >> n >> m;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> v[i][j];

    cout << DFS(0, 0, string(m, '0')) << "\n";

    return 0;
}
