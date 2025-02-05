#include <bits/stdc++.h>
using namespace std;
 
// Hungarian (Kuhn-Munkres) 알고리즘 (square matrix용, 0-indexed)
double hungarian(const vector<vector<double>> &cost) {
    int n = cost.size();
    const double INF = 1e18;
    vector<double> u(n+1, 0), v(n+1, 0);
    vector<int> p(n+1, 0), way(n+1, 0);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        vector<double> minv(n+1, INF);
        vector<char> used(n+1, false);
        int j0 = 0;
        do{
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            double delta = INF;
            for (int j = 1; j <= n; j++){
                if (!used[j]){
                    double cur = cost[i0-1][j-1] - u[i0] - v[j];
                    if(cur < minv[j]){
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if(minv[j] < delta){
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= n; j++){
                if(used[j]){
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while(p[j0] != 0);
        do{
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while(j0);
    }
    return -v[0]; // 최소 총비용
}
 
struct Point {
    int x, y;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N; 
    cin >> N;
    vector<Point> points(N);
    for (int i = 0; i < N; i++){
        cin >> points[i].x >> points[i].y;
    }
 
    // x<0 인 점들과 x>0 인 점들을 분리하고, x==0 인 경우는 단독 배치(비용 0)로 처리한다.
    vector<Point> neg, pos;
    double baseCost = 0.0;
    for (int i = 0; i < N; i++){
        if(points[i].x < 0) {
            neg.push_back(points[i]);
            baseCost += -points[i].x; // x<0 → 이동비용 = |x| = -x
        } else if(points[i].x > 0) {
            pos.push_back(points[i]);
            baseCost += points[i].x;
        } else {
            // x==0: 이미 y축 위에 있으므로 비용 0.
        }
    }
 
    // negatives와 positives 사이에 가능한 매칭에 대해 "절감액"을 계산한다.
    // (절감액 = 단독 배치시 비용 - 쌍으로 배치할 때의 비용)
    // 단독: 비용 = |x_neg| + |x_pos|, 쌍: 비용 = sqrt((x_neg+x_pos)^2 + (y_neg - y_pos)^2)
    int nL = neg.size(), nR = pos.size();
    vector<vector<double>> weight(nL, vector<double>(nR, 0.0));
    for (int i = 0; i < nL; i++){
        for (int j = 0; j < nR; j++){
            double sumX = (double)neg[i].x + pos[j].x; // (음수 + 양수) → pos[j].x - |neg[i].x|
            double diffY = (double)neg[i].y - pos[j].y;
            double pairCost = sqrt(sumX * sumX + diffY * diffY);
            double saving = ((-neg[i].x) + pos[j].x) - pairCost;
            if(saving < 0)
                saving = 0;
            weight[i][j] = saving;
        }
    }
 
    // 이제 negatives와 positives 사이의 매칭에서 총 절감액을 최대화하고자 한다.
    // 최대 weighted matching을 구하는데, Hungarian 알고리즘은 최소비용 매칭을 구하므로
    // 각 간선의 cost를 -saving으로 두면 된다.
    int n = max(nL, nR);
    vector<vector<double>> costMat(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            double val = 0.0;
            if(i < nL && j < nR)
                val = - weight[i][j]; // 최대화를 최소화 문제로 변환
            costMat[i][j] = val;
        }
    }
 
    double minCostMatching = hungarian(costMat); // 최소 총 "cost" (즉, -총절감액)
    double maxSaving = -minCostMatching; // 최대 총 절감액
 
    double ans = baseCost - maxSaving;
    // 소수점 셋째 자리까지 반올림하여 출력
    cout << fixed << setprecision(3) << ans << "\n";
    return 0;
}
