#include <bits/stdc++.h>
using namespace std;
 
using ll = long long;
 
// --- 1. 1차원(한 방향)에서의 기름 면적 함수 ---
// 주어진 침대 시트의 한 차원 좌표 구간 [L, R] (L <= R)
// 에 대해, t초에 [-t, t]와의 교집합의 “단위 정사각형 수”를 구하는 함수 f(t)를 
// “조각별 선형함수”로 나타냅니다.
// (참고: 좌표가 단위 정사각형을 나타내므로 교집합의 길이는 (upper - lower + 1)입니다.)
struct LinearSegment {
    int L, R; // 유효 구간: t in [L, R)
    ll a, b;  // f(t) = a * t + b (구간 내에서 선형)
};
 
vector<LinearSegment> getLinearSegments(int L, int R) {
    vector<LinearSegment> seg;
    // Case 1: [L,R]가 모두 음수가 아닌 경우 (전부 오른쪽에)
    if (L >= 0) {
        // t < L이면 아직 [-t,t]가 [L,R]에 닿지 않으므로 0.
        if(0 < L)
            seg.push_back({0, L, 0, 0});
        // t in [L, R): 교집합 = [L, t] ⇒ 길이 = t - L + 1
        if(L < R)
            seg.push_back({L, R, 1, - (ll)L + 1});
        // t >= R: 교집합 전체 [L,R]가 포함되어, 길이 = R - L + 1
        seg.push_back({R, INT_MAX, 0, (ll)R - L + 1});
    }
    // Case 2: [L,R]가 모두 음수인 경우 (전부 왼쪽에)
    else if (R < 0) {
        int t0 = -R; // t < -R이면 교집합 없음.
        if(0 < t0)
            seg.push_back({0, t0, 0, 0});
        int t1 = -L; // t가 이 구간 이상이면 전체 [L,R]가 포함됨.
        // t in [t0, t1): 교집합 = [ -t, R ] ⇒ 길이 = R - (-t) + 1 = t + R + 1.
        if(t0 < t1)
            seg.push_back({t0, t1, 1, (ll)R + 1});
        seg.push_back({t1, INT_MAX, 0, (ll)R - L + 1});
    }
    // Case 3: [L,R]가 0를 걸치는 경우 (L < 0, R > 0)
    else {
        int A = -L, B = R; // A, B는 양수
        if(A <= B) { // 즉, |L| <= R 인 경우
            // t in [0, A): 교집합 = [-t, t] ⇒ 길이 = 2*t + 1.
            seg.push_back({0, A, 2, 1});
            // t in [A, B): 이제 낮은 쪽은 L, 높은 쪽은 t: 길이 = t - L + 1.
            seg.push_back({A, B, 1, - (ll)L + 1});
            // t >= B: 전체 [L,R]가 포함, 길이 = R - L + 1.
            seg.push_back({B, INT_MAX, 0, (ll)R - L + 1});
        } else { // A > B, 즉 |L| > R
            seg.push_back({0, B, 2, 1}); // t in [0, B): 길이 = 2*t + 1.
            seg.push_back({B, A, 1, (ll)R + 1}); // t in [B, A): 길이 = R - (-t) + 1 = t + R + 1.
            seg.push_back({A, INT_MAX, 0, (ll)R - L + 1});
        }
    }
    return seg;
}
 
// --- 2. 2차원에서의 면적 기여 ---
// 침대 시트의 기름에 젖는 면적은 x-방향 교집합 길이 * y-방향 교집합 길이입니다.
// 각각은 위와 같이 선형 함수로 분할되어 있으므로, 두 선형함수의 곱은 구간마다 2차 다항식이 됩니다.
struct PolySegment {
    int L, R; // t in [L, R)
    ll A, B, C; // f(t) = A*t^2 + B*t + C
};
 
// --- 3. 이벤트 (범위 업데이트) ---
// 각 PolySegment에 대해 구간 [L, R) 동안 다항식 값이 더해지도록 이벤트를 기록합니다.
struct Event {
    int t;
    ll dA, dB, dC;
};
 
// 메인 – 입력 읽고, 각 시트별로 조각 분할한 후 이벤트로 기록한 뒤, t=0..maxT까지 스위프
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<array<int,4>> sheets(N);
    for (int i=0; i<N; i++){
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        sheets[i] = {x1, y1, x2, y2};
    }
    int M;
    cin >> M;
    vector<int> query(M);
    int maxT = 0;
    for (int i=0; i<M; i++){
        cin >> query[i];
        maxT = max(maxT, query[i]);
    }
    // t 범위는 0부터 maxT까지; 이벤트 업데이트 시 [L, R) 구간으로 진행하므로, T_limit = maxT+1.
    int T_limit = maxT + 1;
 
    vector<Event> events;
    // 한 시트당 최대 3*3=9 조각, 각 조각 2 이벤트 → 약 18*N 이벤트
    events.reserve(N * 18);
 
    // 각 시트에 대해
    for (int i=0; i<N; i++){
        int x1 = sheets[i][0], y1 = sheets[i][1], x2 = sheets[i][2], y2 = sheets[i][3];
        // x-방향, y-방향 함수 구간 분할
        vector<LinearSegment> segX = getLinearSegments(x1, x2);
        vector<LinearSegment> segY = getLinearSegments(y1, y2);
        // 각 x-구간과 y-구간의 교집합 구간에서 두 선형함수의 곱으로 2차 다항식을 만듭니다.
        for (auto &sx : segX) {
            for (auto &sy : segY) {
                int L_int = max(sx.L, sy.L);
                int R_int = min(sx.R, sy.R);
                if(L_int < R_int) {
                    // 두 선형 함수: f_x(t)=sx.a*t+sx.b, f_y(t)=sy.a*t+sy.b.
                    // 곱하면: (sx.a*sy.a)*t^2 + (sx.a*sy.b + sx.b*sy.a)*t + (sx.b*sy.b).
                    ll A_poly = sx.a * sy.a;
                    ll B_poly = sx.a * sy.b + sx.b * sy.a;
                    ll C_poly = sx.b * sy.b;
                    // t 범위를 [L_int, R_int)로 제한하고, 우리 관심 범위 [0, T_limit) 내로 클립.
                    int L_clip = max(L_int, 0);
                    int R_clip = min(R_int, T_limit);
                    if(L_clip < R_clip) {
                        events.push_back({L_clip, A_poly, B_poly, C_poly});
                        events.push_back({R_clip, -A_poly, -B_poly, -C_poly});
                    }
                }
            }
        }
    }
 
    // 이벤트를 t 순으로 정렬합니다.
    sort(events.begin(), events.end(), [](const Event &e1, const Event &e2){
        return e1.t < e2.t;
    });
 
    // 이제 t = 0부터 T_limit-1 (즉, maxT)까지 스위프하면서, 
    // "현재 활성 이벤트"의 누적 다항식 계수를 유지하고, F[t] = cumA*t^2 + cumB*t + cumC를 구합니다.
    vector<ll> F(T_limit, 0);
    ll cumA = 0, cumB = 0, cumC = 0;
    int evIndex = 0;
    int evSize = events.size();
    for (int t = 0; t < T_limit; t++){
        while(evIndex < evSize && events[evIndex].t == t){
            cumA += events[evIndex].dA;
            cumB += events[evIndex].dB;
            cumC += events[evIndex].dC;
            evIndex++;
        }
        ll t_ll = t;
        F[t] = cumA * t_ll * t_ll + cumB * t_ll + cumC;
    }
 
    // 질의 시간에 대해 미리 구한 F[t] 값을 출력합니다.
    for (int i = 0; i < M; i++){
        cout << F[ query[i] ] << "\n";
    }
 
    return 0;
}
