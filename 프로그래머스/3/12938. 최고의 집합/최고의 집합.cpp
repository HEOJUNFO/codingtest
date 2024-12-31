#include <string>
#include <vector>

using namespace std;

vector<int> solution(int n, int s) {
    // s가 n보다 작으면 각 원소가 1이상이 될 수 없으므로 -1 반환
    if (s < n) {
        return {-1};
    }
    
    // n개로 나누었을 때의 몫과 나머지
    int q = s / n;  // 몫
    int r = s % n;  // 나머지
    
    vector<int> answer(n, q);
    
    // 나머지만큼 뒤에서부터 1씩 추가 (오름차순 유지)
    for (int i = 0; i < r; i++) {
        answer[n - 1 - i] += 1;
    }
    
    return answer;
}
