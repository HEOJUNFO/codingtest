import sys
input = sys.stdin.readline

N, M = map(int, input().split())
arr = list(map(int, input().split()))

# prefix sum: prefix[i] = sum of arr[0:i]
prefix = [0] * (N+1)
for i in range(1, N+1):
    prefix[i] = prefix[i-1] + arr[i-1]

def interval_sum(l, r):
    # sum of arr[l:r+1]
    return prefix[r+1] - prefix[l]

# dp[i][j]: using first i elements, forming exactly j groups, max sum
# Initialize dp with very small number
dp = [[float('-inf')] * (M+1) for _ in range(N+1)]
dp[0][0] = 0

for i in range(1, N+1):
    # j groups using i elements
    for j in range(0, M+1):
        # case 1: not using the i-th element in any new group
        dp[i][j] = max(dp[i][j], dp[i-1][j])
        
        # case 2: form a group ending at i-th element
        if j > 0:
            # find k where the new group starts: arr[k:i]
            for k in range(1, i+1):
                # dp[k-1][j-1] + sum(arr[k-1:i-1])
                # since arr is 0-indexed, group k..i in dp means arr[k-1:i], 
                # interval_sum(k-1, i-1)
                dp[i][j] = max(dp[i][j], dp[k-1][j-1] + interval_sum(k-1, i-1))

print(dp[N][M])
