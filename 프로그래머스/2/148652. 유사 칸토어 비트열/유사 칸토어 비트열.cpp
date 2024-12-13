#include <string>
#include <vector>
using namespace std;

bool hasTwoInBase5(long long x) {
    while (x > 0) {
        if (x % 5 == 2) return true;
        x /= 5;
    }
    return false;
}

int solution(int n, long long l, long long r) {
    int answer = 0;
    for (long long i = l; i <= r; i++) {
        if (!hasTwoInBase5(i-1)) answer++;
    }
    return answer;
}
