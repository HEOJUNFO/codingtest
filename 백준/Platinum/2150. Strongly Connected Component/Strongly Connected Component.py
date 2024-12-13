import sys
sys.setrecursionlimit(10**7)

input = sys.stdin.readline

V, E = map(int, input().split())
graph = [[] for _ in range(V+1)]

for _ in range(E):
    A, B = map(int, input().split())
    graph[A].append(B)

# 타잔 알고리즘을 위한 전역 변수
id_counter = 0
ids = [0]*(V+1)
low = [0]*(V+1)
on_stack = [False]*(V+1)
stack = []
SCCs = []

def tarjan(u):
    global id_counter
    id_counter += 1
    ids[u] = id_counter
    low[u] = id_counter
    stack.append(u)
    on_stack[u] = True

    for v in graph[u]:
        if ids[v] == 0:  # 아직 방문 안한 노드
            tarjan(v)
            low[u] = min(low[u], low[v])
        elif on_stack[v]:
            low[u] = min(low[u], ids[v])
    
    # 자신의 low-link 값이 자기 자신 id와 같다면 SCC 루트
    if low[u] == ids[u]:
        scc = []
        while True:
            node = stack.pop()
            on_stack[node] = False
            scc.append(node)
            if node == u:
                break
        scc.sort()
        SCCs.append(scc)

for i in range(1, V+1):
    if ids[i] == 0:
        tarjan(i)

# SCC를 각 SCC에 속하는 정점의 가장 작은 정점 번호 기준으로 정렬
SCCs.sort(key=lambda s: s[0])

print(len(SCCs))
for scc in SCCs:
    print(' '.join(map(str, scc)), -1)
