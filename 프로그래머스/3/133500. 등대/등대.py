import sys
sys.setrecursionlimit(10**9)

def solution(n, lighthouse):
    # 인접 리스트 생성
    adj = [[] for _ in range(n+1)]
    for a, b in lighthouse:
        adj[a].append(b)
        adj[b].append(a)
        
    # dp[node][0 or 1]
    # 0: 이 노드를 선택하지 않음
    # 1: 이 노드를 선택함
    dp = [[0, 0] for _ in range(n+1)]
    visited = [False] * (n+1)

    def dfs(u):
        visited[u] = True
        dp[u][0] = 0
        dp[u][1] = 1
        for v in adj[u]:
            if not visited[v]:
                dfs(v)
                # u를 선택하지 않았다면, v는 반드시 선택해야 함
                dp[u][0] += dp[v][1]
                # u를 선택했다면, v는 선택하거나 안하거나 그 중 최소값
                dp[u][1] += min(dp[v][0], dp[v][1])

    # 임의의 노드(1번)에서 시작
    dfs(1)
    return min(dp[1][0], dp[1][1])


