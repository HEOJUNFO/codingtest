import sys
input = sys.stdin.readline

N = int(input().strip())
connected = [[False]*101 for _ in range(101)]

for _ in range(N):
    a, b = map(int, input().split())
    if a > b:
        a, b = b, a
    connected[a][b] = True

dp = [[-1]*101 for _ in range(101)]

def solve(i, j):
    if i > j:
        return 0
    if dp[i][j] != -1:
        return dp[i][j]
    if i == j:
        dp[i][j] = 0
        return 0

    # 점 i를 사용하지 않는 경우
    res = solve(i+1, j)

    # 점 i를 사용한다면 i와 연결된 k를 찾아서
    for k in range(i+1, j+1):
        if connected[i][k]:
            # i-k를 현으로 사용한다면 i+1~k-1, k+1~j로 나뉨
            res = max(res, solve(i+1, k-1) + 1 + solve(k+1, j))

    dp[i][j] = res
    return res

print(solve(1, 100))
