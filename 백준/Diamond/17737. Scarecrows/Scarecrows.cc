#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

const ll INF = 1LL << 60; // 충분히 큰 값

struct Scarecrow {
    ll x, y;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    vector<Scarecrow> sc(N);
    for (int i = 0; i < N; i++){
        cin >> sc[i].x >> sc[i].y;
    }
    // x 좌표 기준 오름차순 정렬 (x, y 모두 서로 다르므로)
    sort(sc.begin(), sc.end(), [](const Scarecrow &a, const Scarecrow &b){
        return a.x < b.x;
    });
    
    // 스택: 각 원소는 {y값 (즉, 원래의 값), 현재까지의 "레코드" (즉, valid한 후보 j의 최소 y)}
    vector<pair<ll,ll>> st;
    ll ans = 0;
    
    // x좌표 순으로 (즉, left에서 right로) 순회하면서
    // 각 점을 'northeast' 후보로 보면서, 이전 점들을 southwest 후보로 할 때 valid한 경우(내부에 점이 없는 경우)를 체크합니다.
    for (int i = 0; i < N; i++){
        ll v = sc[i].y;
        // 스택을 뒤에서부터 순회합니다.
        // 각 후보 (a, rec) 에 대해, 새 점 v 가 후보 a 보다 크면서 (즉, y가 더 높아야 함)
        // 또한 아직 기록된 rec보다 작으면 (즉, 지금까지의 valid 후보보다 낮은 값이면)
        // 이 쌍은 조건을 만족하는 직사각형 (southwest: a, northeast: v)가 됩니다.
        for (int j = st.size()-1; j >= 0; j--){
            if(v > st[j].first && v < st[j].second){
                ans++;
                st[j].second = v; // 해당 후보의 기록을 갱신 (더 낮은 값으로)
            }
        }
        // 새 점을 후보로 스택에 넣습니다.
        st.push_back({v, INF});
    }
    
    cout << ans << "\n";
    return 0;
}
