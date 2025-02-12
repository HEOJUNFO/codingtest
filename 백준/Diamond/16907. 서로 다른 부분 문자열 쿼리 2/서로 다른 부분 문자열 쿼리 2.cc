#include <iostream>
#include <cstring>
#include <string>
using namespace std;

const int MAXLEN = 400005; // 최대 문자 수의 2배 정도

struct State {
    int len, link;
    int next[26];
};

State st[MAXLEN];
int sz, last;
long long distinctCount = 0;  // 현재까지의 서로 다른 부분 문자열의 개수

// suffix automaton 초기화
void sa_init() {
    st[0].len = 0;
    st[0].link = -1;
    memset(st[0].next, -1, sizeof(st[0].next));
    sz = 1;
    last = 0;
}

// 문자 c를 추가하면서 suffix automaton 업데이트
void sa_extend(char c) {
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    memset(st[cur].next, -1, sizeof(st[cur].next));
    
    int p = last;
    int letter = c - 'a';
    for(; p != -1 && st[p].next[letter] == -1; p = st[p].link) {
        st[p].next[letter] = cur;
    }
    
    if(p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[letter];
        if(st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            memcpy(st[clone].next, st[q].next, sizeof(st[q].next));
            st[clone].link = st[q].link;
            for(; p != -1 && st[p].next[letter] == q; p = st[p].link) {
                st[p].next[letter] = clone;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
    // 새로 추가된 상태가 만들어내는 새로운 부분 문자열의 개수는
    // st[cur].len - st[st[cur].link].len 이다.
    distinctCount += st[cur].len - st[st[cur].link].len;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string Q;
    cin >> Q;
    
    sa_init();
    
    for(char c : Q) {
        if(c == '?') {
            cout << distinctCount << "\n";
        } else {
            sa_extend(c);
        }
    }
    
    return 0;
}
