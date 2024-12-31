import sys
sys.setrecursionlimit(10**7)
input = sys.stdin.readline

MAX_LOG = 17  # 대략 log2(100000) ~ 16.6 정도이므로 17 정도 잡는다.

def dfs(u, p, d, graph, parent, depth, sub_size):
    """ 루트를 1번 노드로 잡고 DFS 하면서 
        - parent[0][u] = p
        - depth[u] = d
        - sub_size[u] (서브트리 크기) 계산
    """
    parent[0][u] = p
    depth[u] = d
    sub_size[u] = 1
    for v in graph[u]:
        if v == p:
            continue
        dfs(v, u, d+1, graph, parent, depth, sub_size)
        sub_size[u] += sub_size[v]


def build_lca(N, parent, depth):
    """ Binary Lifting 테이블 parent[k][v] 구성
        parent[k][v]: v에서 2^k 번째 조상
    """
    for k in range(1, MAX_LOG):
        for v in range(1, N+1):
            if parent[k-1][v] != -1:
                parent[k][v] = parent[k-1][ parent[k-1][v] ]


def lca(u, v, parent, depth):
    """ u, v 의 LCA를 binary lifting 으로 구한다. """
    # depth[u] >= depth[v] 가 되도록 조정
    if depth[u] < depth[v]:
        u, v = v, u
    
    # 1) u를 v의 깊이까지 끌어올린다
    diff = depth[u] - depth[v]
    for k in range(MAX_LOG):
        if diff & (1 << k):
            u = parent[k][u]
    
    # 같아졌다면 그것이 LCA
    if u == v:
        return u
    
    # 2) u, v를 동시에 위로 올려서 부모가 같아지기 직전까지 올린다
    for k in reversed(range(MAX_LOG)):
        if parent[k][u] != parent[k][v]:
            u = parent[k][u]
            v = parent[k][v]
    
    # 이제 바로 위 부모가 LCA
    return parent[0][u]


def get_ancestor(u, dist, parent):
    """ u에서 위로 dist만큼 올린 노드를 구한다. """
    for k in range(MAX_LOG):
        if dist & (1 << k):
            u = parent[k][u]
    return u


def solve():
    T = int(input())
    for tcase in range(1, T+1):
        N, Q, R = map(int, input().split())
        
        # 그래프(트리) 입력
        graph = [[] for _ in range(N+1)]
        for _ in range(N-1):
            A, B = map(int, input().split())
            graph[A].append(B)
            graph[B].append(A)
        
        # LCA를 위한 자료구조
        parent = [[-1]*(N+1) for _ in range(MAX_LOG)]  # parent[k][v] = v의 2^k번째 조상
        depth = [0]*(N+1)       # 각 노드의 깊이(루트=1에서의)
        sub_size = [0]*(N+1)    # 서브트리 크기
        
        # 1번 노드를 루트로 DFS
        dfs(1, -1, 0, graph, parent, depth, sub_size)
        
        # Binary Lifting 전처리
        build_lca(N, parent, depth)
        
        # 현재 수도
        capital = R
        
        print(f"Case #{tcase}:")
        for _ in range(Q):
            S, U = map(int, input().split())
            if S == 0:
                # 수도 변경
                capital = U
            else:
                # S == 1: U가 세금 처리해야 할 도시 개수
                X = U
                if X == capital:
                    # 수도 == X 인 경우, 전체 도시 수
                    print(N)
                    continue
                
                # LCA를 구한다
                L = lca(X, capital, parent, depth)
                if L != X:
                    # X가 capital의 조상이 아니면
                    # 그대로 X의 서브트리 크기
                    print(sub_size[X])
                else:
                    # L == X, 즉 X가 capital의 조상 (단 X != capital)
                    # capital에서 X까지의 경로에서 X 아래(직계 자식) 노드를 구해야 함
                    dist = depth[capital] - depth[X] - 1
                    if dist < 0:
                        # dist < 0 가 나온다는 것은 X == capital 인 경우를 
                        # 이미 위에서 처리했으므로 실제로 여기선 안 나올 것.
                        print(N)
                    else:
                        c = get_ancestor(capital, dist, parent)
                        # X의 새로운 서브트리 크기는 전체 N에서 'c의 서브트리'를 뺀 것
                        print(N - sub_size[c])


# 메인 실행(백준/외부 플랫폼 제출 시에는 아래 부분 생략 혹은 조건 처리)
if __name__ == "__main__":
    solve()
