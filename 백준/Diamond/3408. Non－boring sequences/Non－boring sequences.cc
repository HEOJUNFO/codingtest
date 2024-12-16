#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

static const int MAXN = 200000 + 10;

int n;
int arr[MAXN];
// l[i], r[i]: i번 인덱스 원소와 같은 값을 가진 원소 중 i 왼쪽, 오른쪽에 있는 인덱스
int lpos[MAXN], rpos[MAXN];

// 정답 플래그
bool isBoring;

// [s, e] 구간을 확인하는 재귀 함수
void solveSubarray(int s, int e) {
    if (s >= e || isBoring) return;

    // 구간 [s, e]에서 유니크한 원소를 찾는다.
    // 유니크한 원소의 정의: 구간 내에 그 값이 단 하나만 존재하는 원소.
    // 이를 효율적으로 찾기 위해, 양 끝에서부터 차례로 확인한다.
    // 유니크한 원소인지 확인: lpos[i] < s && rpos[i] > e 이면
    // i번 인덱스의 원소는 [s, e] 내에서 유니크한 원소이다.
    int length = e - s + 1;
    for (int offset = 0; offset <= length / 2; ++offset) {
        int leftIdx = s + offset;
        if (lpos[leftIdx] < s && rpos[leftIdx] > e) {
            // [s, leftIdx-1], [leftIdx+1, e]로 분할
            solveSubarray(s, leftIdx - 1);
            solveSubarray(leftIdx + 1, e);
            return;
        }

        int rightIdx = e - offset;
        if (lpos[rightIdx] < s && rpos[rightIdx] > e) {
            // [s, rightIdx-1], [rightIdx+1, e]로 분할
            solveSubarray(s, rightIdx - 1);
            solveSubarray(rightIdx + 1, e);
            return;
        }
    }

    // 유니크한 원소를 찾지 못했다면 boring
    isBoring = true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> arr[i];

        // arr의 값들을 압축 (좌표압축)
        vector<int> comp;
        comp.reserve(n);
        for (int i = 1; i <= n; i++) comp.push_back(arr[i]);
        sort(comp.begin(), comp.end());
        comp.erase(unique(comp.begin(), comp.end()), comp.end());

        for (int i = 1; i <= n; i++) {
            arr[i] = (int)(lower_bound(comp.begin(), comp.end(), arr[i]) - comp.begin()) + 1;
        }

        // 각 값에 대해 인덱스를 저장
        static vector<int> pos[MAXN];
        int maxVal = (int)comp.size();
        for (int i = 0; i <= maxVal; i++) {
            pos[i].clear();
        }

        for (int i = 1; i <= n; i++)
            pos[arr[i]].push_back(i);

        // 각 값에 대해 0과 n+1도 sentinel로 둔다.
        for (int i = 0; i <= maxVal; i++) {
            pos[i].insert(pos[i].begin(), 0);
            pos[i].push_back(n + 1);
        }

        // lpos[i], rpos[i] 구하기
        for (int i = 1; i <= n; i++) {
            lpos[i] = 0;
            rpos[i] = n + 1;
        }

        for (int val = 0; val <= maxVal; val++) {
            for (size_t j = 1; j < pos[val].size(); j++) {
                int idx = pos[val][j];
                lpos[idx] = pos[val][j - 1];
            }
            for (size_t j = 0; j + 1 < pos[val].size(); j++) {
                int idx = pos[val][j];
                rpos[idx] = pos[val][j + 1];
            }
        }

        isBoring = false;
        solveSubarray(1, n);

        cout << (isBoring ? "boring\n" : "non-boring\n");
    }

    return 0;
}
