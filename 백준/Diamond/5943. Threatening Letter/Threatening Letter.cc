#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int ALPHABET = 26;

struct State {
    int len, link;
    int next[ALPHABET];
    State() : len(0), link(-1) {
        for (int i = 0; i < ALPHABET; i++) next[i] = -1;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    
    // 신문 텍스트와 메시지를 여러 줄에 걸쳐 입력받기
    string newspaper, message, temp;
    while (newspaper.size() < (size_t)N) {
        cin >> temp;
        newspaper += temp;
    }
    while (message.size() < (size_t)M) {
        cin >> temp;
        message += temp;
    }
    
    // 신문 텍스트로 suffix automaton 구축
    vector<State> sa;
    sa.reserve(2 * N);
    sa.push_back(State());
    int last = 0; // 현재 마지막 상태 인덱스
    for (char c : newspaper) {
        int cur = sa.size();
        sa.push_back(State());
        sa[cur].len = sa[last].len + 1;
        int p = last;
        int letter = c - 'A';
        // 아직 해당 문자를 읽는 전이가 없으면 새 상태로 연결
        while (p != -1 && sa[p].next[letter] == -1) {
            sa[p].next[letter] = cur;
            p = sa[p].link;
        }
        if (p == -1) {
            // p가 -1이면 처음부터 전이가 없는 경우: 링크는 0
            sa[cur].link = 0;
        } else {
            int q = sa[p].next[letter];
            if (sa[p].len + 1 == sa[q].len) {
                sa[cur].link = q;
            } else {
                // q 상태를 복사한 clone 상태 생성
                int clone = sa.size();
                sa.push_back(State());
                sa[clone].len = sa[p].len + 1;
                sa[clone].link = sa[q].link;
                for (int i = 0; i < ALPHABET; i++) {
                    sa[clone].next[i] = sa[q].next[i];
                }
                // p로부터 q로 가던 전이를 clone으로 대체
                while (p != -1 && sa[p].next[letter] == q) {
                    sa[p].next[letter] = clone;
                    p = sa[p].link;
                }
                sa[q].link = sa[cur].link = clone;
            }
        }
        last = cur;
    }
    
    // 메시지를 앞에서부터 최대한 길게 매칭되는 구간으로 쪼개기 (그리디)
    int segments = 0;
    int pos = 0;
    int msize = message.size();
    while (pos < msize) {
        int curState = 0;
        // 만약 현재 글자가 신문에 나타나지 않으면 (문제에서는 주어진 조건상 발생하지 않음)
        if (sa[curState].next[message[pos] - 'A'] == -1) {
            cout << -1 << "\n";
            return 0;
        }
        segments++; // 새로운 컷 (부분 문자열) 시작
        // 가능한 한 많이 매칭
        while (pos < msize && sa[curState].next[message[pos] - 'A'] != -1) {
            curState = sa[curState].next[message[pos] - 'A'];
            pos++;
        }
    }
    cout << segments << "\n";
    return 0;
}
