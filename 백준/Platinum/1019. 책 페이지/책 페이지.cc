#include <bits/stdc++.h>
using namespace std;

long long cnt[10];

void calc(long long n) {
    long long start = 1;
    long long left, right, digit;
    while (start <= n) {
        left = n / (start * 10);
        digit = (n / start) % 10;
        right = n % start;
        
        for (int i = 0; i < 10; i++) {
            cnt[i] += left * start; 
        }
        
        // digit보다 작은 숫자들은 한 번의 주기에서 start만큼 추가로 등장
        for (int i = 0; i < digit; i++) {
            cnt[i] += start;
        }
        
        // digit는 right + 1번 더 등장
        cnt[digit] += right + 1;
        
        // 0은 leading zero 문제로 인해 조정이 필요하다.
        // 가장 왼쪽 블록에서 0이 등장하는 횟수를 빼줘야 한다.
        cnt[0] -= start; 
        
        start *= 10;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long N;
    cin >> N;
    calc(N);
    
    for (int i = 0; i < 10; i++) {
        cout << cnt[i] << (i == 9 ? '\n' : ' ');
    }
    return 0;
}
