#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

bool isBetween(int val, int a, int b) {
    return (min(a,b) <= val && val <= max(a,b));
}

bool isOnLineSegment(int x1, int y1, int x2, int y2, int xq, int yq) {
    // 일직선상에 있는지 확인
    // 외적 == 0 조건: (yq - y1)*(x2 - x1) == (y2 - y1)*(xq - x1)
    if ((long long)(yq - y1) * (x2 - x1) != (long long)(y2 - y1) * (xq - x1)) {
        return false;
    }
    // 구간 내에 있는지 확인
    if (isBetween(xq, x1, x2) && isBetween(yq, y1, y2)) {
        return true;
    }
    return false;
}

vector<int> solution(int m, int n, int startX, int startY, vector<vector<int>> balls) {
    vector<int> answer;
    
    for (auto &ball : balls) {
        int a = ball[0];
        int b = ball[1];
        
        vector<int> xCandidates = {-a, a, 2*m - a};
        vector<int> yCandidates = {-b, b, 2*n - b};
        
        int minDistSquared = -1;
        
        for (int xC : xCandidates) {
            for (int yC : yCandidates) {
                // 원래 위치 (a, b)는 제외 (쿠션 없이 바로 맞추는 경우)
                if (xC == a && yC == b) continue;
                
                // 해당 경로 상에서 목표 공 (a,b)를 먼저 만나면 안됨
                // 즉, (startX,startY) -> (xC,yC) 직선 경로 상에 (a,b)가 
                // start와 (xC,yC) 사이에 있는지 확인
                if (isOnLineSegment(startX, startY, xC, yC, a, b)) {
                    // 이 경우는 쿠션 전에 공을 맞추는 것이므로 제외
                    continue;
                }
                
                int dx = xC - startX;
                int dy = yC - startY;
                int distSquared = dx*dx + dy*dy;
                
                if (minDistSquared == -1 || distSquared < minDistSquared) {
                    minDistSquared = distSquared;
                }
            }
        }
        answer.push_back(minDistSquared);
    }
    
    return answer;
}
