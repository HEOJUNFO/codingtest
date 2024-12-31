import sys
import math
sys.setrecursionlimit(10**7)
input = sys.stdin.readline

N = int(input().strip())
adj = [[] for _ in range(N+1)]
for _ in range(N-1):
    x, y = map(int, input().split())
    adj[x].append(y)
    adj[y].append(x)

# 1. 깊이(depth) 계산 및 부모(par) 정보(바이너리 리프팅)를 위한 DFS
LOG = math.ceil(math.log2(N)) if N > 1 else 1
depth = [0]*(N+1)
parent = [[0]*(N+1) for _ in range(LOG+1)]  # parent[k][v] : v의 2^k번째 조상

def dfs(u, p):
    for v in adj[u]:
        if v == p:
            continue
        depth[v] = depth[u] + 1
        parent[0][v] = u  # v의 바로 위(1번째 조상)는 u
        dfs(v, u)

# 임의로 1번을 루트로 잡고 DFS
dfs(1, -1)

# 2. parent 테이블 완성 (Binary Lifting)
for k in range(1, LOG+1):
    for v in range(1, N+1):
        parent[k][v] = parent[k-1][ parent[k-1][v] ]

# 3. LCA 함수 (두 정점 u, v의 공통조상 중 깊이가 가장 큰 정점)
def lca(u, v):
    if depth[u] < depth[v]:
        u, v = v, u  # depth[u] >= depth[v]로 맞춤
    
    # 1) u를 v와 같은 depth로 올리기
    diff = depth[u] - depth[v]
    for k in range(LOG+1):
        if diff & (1 << k):
            u = parent[k][u]
    
    if u == v:
        return u
    
    # 2) 위에서부터 하나씩 내려가며 LCA 찾기
    for k in reversed(range(LOG+1)):
        if parent[k][u] != parent[k][v]:
            u = parent[k][u]
            v = parent[k][v]
    
    # 이제 바로 위가 LCA
    return parent[0][u]

# 4. 트리에서 거리 함수
def dist(u, v):
    return depth[u] + depth[v] - 2*depth[lca(u,v)]

# 5. (u->v) 경로상에서 u로부터 step만큼 떨어진 정점을 찾는 함수
#    총 거리는 dist(u,v)라 할 때, step <= dist(u,v) 라고 가정
def get_node_on_path(u, v, step):
    """
    경로: u -> ... -> L -> ... -> v   (L은 LCA(u,v))
    1) LCA를 구한다
    2) u에서 LCA까지의 거리: duL = depth[u] - depth[L]
       만약 step <= duL 이라면, u에서 위로 step만큼 올린 정점
       그렇지 않다면, v에서 위로 (dist(u,v) - step)만큼 올린 정점
    """
    L = lca(u, v)
    d_uv = depth[u] + depth[v] - 2*depth[L]
    duL = depth[u] - depth[L]
    
    if step <= duL:
        # u에서 위로 step만큼 올리기
        return get_kth_ancestor(u, step)
    else:
        # v에서 위로 (d_uv - step)만큼 올리기
        step2 = d_uv - step
        return get_kth_ancestor(v, step2)

# 6. node의 k번째 조상 찾기
def get_kth_ancestor(node, k):
    for bit in range(LOG+1):
        if k & (1 << bit):
            node = parent[bit][node]
    return node

# 7. 쿼리 처리
Q = int(input().strip())
for _ in range(Q):
    A, B, C = map(int, input().split())
    # 세 쌍의 거리
    dAB = dist(A, B)
    dBC = dist(B, C)
    dCA = dist(C, A)
    
    # 가장 큰 거리(= 트리플의 지름) 찾기
    # (D1, D2)는 지름을 이루는 두 점
    diam = dAB
    D1, D2 = A, B
    if dBC > diam:
        diam = dBC
        D1, D2 = B, C
    if dCA > diam:
        diam = dCA
        D1, D2 = C, A
    
    # 지름이 홀수면 외심은 존재하지 않음
    if diam % 2 == 1:
        print(-1)
        continue
    
    # 지름이 짝수이면 (D1->D2) 경로의 가운데 정점 M 찾기
    mid_step = diam // 2
    M = get_node_on_path(D1, D2, mid_step)
    
    # M에서 A,B,C까지 거리가 같은지 확인
    dMA = dist(M, A)
    dMB = dist(M, B)
    dMC = dist(M, C)
    if dMA == dMB == dMC:
        print(M)
    else:
        print(-1)
