import sys
input = sys.stdin.readline

MOD = 10**9 + 7

class FenwickTree:
    def __init__(self, n):
        self.n = n
        self.tree = [0]*(n+1)
    
    def update(self, i, v):
        while i <= self.n:
            self.tree[i] = (self.tree[i] + v) % MOD
            i += i & (-i)
    
    def query(self, i):
        s = 0
        while i > 0:
            s = (s + self.tree[i]) % MOD
            i -= i & (-i)
        return s

    def range_query(self, l, r):
        if l > r:
            return 0
        return (self.query(r) - self.query(l-1)) % MOD

N = int(input())
A = list(map(int, input().split()))

# dp[k]를 Fenwick Tree로 구현. k = 1부터 11까지
fenwicks = [FenwickTree(N) for _ in range(11)]

for x in A:
    # 길이 1인 subsequence: 각 원소 하나만으로 이루는 subsequence
    fenwicks[0].update(x, 1)
    # 길이가 2 이상인 subsequence: 이전 길이 dp값을 참조
    for length in range(2, 12):
        # x-1까지의 dp[length-1] 값을 모두 합산
        val = fenwicks[length-2].query(x-1)
        if val != 0:
            fenwicks[length-1].update(x, val)

# 길이 11인 subsequence의 총 개수
answer = fenwicks[10].query(N) % MOD
print(answer)
