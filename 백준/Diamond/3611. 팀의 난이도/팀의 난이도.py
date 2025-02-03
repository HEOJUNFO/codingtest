import sys,collections
sys.setrecursionlimit(10000)
input = sys.stdin.readline

INF = 10**9  # 충분히 큰 수

# 입력 읽기
n, m = map(int, input().split())
edges = []
for _ in range(m):
    u, v = map(int, input().split())
    edges.append((u, v))

# Dinic 알고리즘을 위한 max flow 구현
def max_flow(graph, s, t):
    flow = 0.0
    N = len(graph)
    while True:
        level = [-1]*N
        queue = collections.deque()
        level[s] = 0
        queue.append(s)
        while queue:
            u = queue.popleft()
            for v, cap, rev in graph[u]:
                if cap > 1e-9 and level[v] < 0:
                    level[v] = level[u] + 1
                    queue.append(v)
        if level[t] < 0:
            break
        it = [0]*N
        def dfs(u, f):
            if u == t:
                return f
            for i in range(it[u], len(graph[u])):
                it[u] = i
                v, cap, rev = graph[u][i]
                if cap > 1e-9 and level[u] + 1 == level[v]:
                    ret = dfs(v, min(f, cap))
                    if ret > 1e-9:
                        graph[u][i][1] -= ret
                        graph[v][graph[u][i][2]][1] += ret
                        return ret
            return 0.0
        while True:
            pushed = dfs(s, INF)
            if pushed <= 1e-9:
                break
            flow += pushed
    return flow

# 주어진 x에 대해 네트워크 구성
# 노드 번호 할당:
# s: 0
# 간선 노드: 1 ~ m (각 입력 간선에 대해 하나씩)
# 직원(정점) 노드: m+1 ~ m+n   (직원 i는 m+i, i는 1-indexed)
# t: m+n+1
def build_network(x):
    N = m + n + 2
    graph = [[] for _ in range(N)]
    def add_edge(u, v, cap):
        graph[u].append([v, cap, len(graph[v])])
        graph[v].append([u, 0.0, len(graph[u]) - 1])
    s = 0
    t = m + n + 1
    # s -> 간선 노드, 각 간선 노드 -> 해당 직원 노드들
    for i, (u, v) in enumerate(edges):
        edge_node = 1 + i
        add_edge(s, edge_node, 1.0)
        # 직원 노드 번호: m + u, m + v
        add_edge(edge_node, m + u, INF)
        add_edge(edge_node, m + v, INF)
    # 각 직원 노드 -> t (용량 = x)
    for v in range(1, n+1):
        add_edge(m + v, t, x)
    return graph

# s, t의 번호
s = 0
t = m + n + 1
N_total = m + n + 2

# 이분 탐색: 최적의 x (즉, 최대 난이도)를 구함.
low = 0.0
high = m * 1.0  # 최대 난이도는 m (직원 1명이 될 때)
best_S = None
for _ in range(50):
    mid = (low + high) / 2.0
    graph = build_network(mid)
    flow = max_flow(graph, s, t)
    # DFS로 s에서 도달 가능한 노드 찾기 (잔여 네트워크에서)
    visited = [False] * N_total
    def dfs(u):
        visited[u] = True
        for v, cap, _ in graph[u]:
            if cap > 1e-9 and not visited[v]:
                dfs(v)
    dfs(s)
    # 직원 노드: m+1 ~ m+n. 도달 가능한 노드들 중에서
    S_candidate = [v - m for v in range(m+1, m+n+1) if visited[v]]
    # 후보 S가 비어있지 않으면(즉, 적어도 한 명의 직원이 포함되면)
    if S_candidate:
        # 위의 네트워크에서 해당 컷의 용량는 m - (|E(S)| - mid * |S|)가 됨.
        # S가 조건을 만족하려면 이 값이 m 이하여야 함.
        if flow <= m + 1e-9:
            low = mid
            best_S = S_candidate[:]  # 복사
        else:
            high = mid
    else:
        high = mid

# 혹시 best_S가 None이면 (예: m==0 인 경우) 임의로 1명 선택
if best_S is None or len(best_S) == 0:
    best_S = [1]

best_S.sort()
print(len(best_S))
for v in best_S:
    print(v)
