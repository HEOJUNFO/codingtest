import sys
from collections import deque
input = sys.stdin.readline

N, M = map(int, input().split())
possible_tasks = [[] for _ in range(N+1)]
for i in range(1, N+1):
    data = list(map(int, input().split()))
    count = data[0]
    tasks = data[1:]
    for t in tasks:
        possible_tasks[i].append(t)

def hopcroft_karp():
    pairU = [0]*(N+1)  # U 집합 직원 매칭 상태
    pairV = [0]*(M+1)  # V 집합 일 매칭 상태
    dist = [0]*(N+1)   # BFS 레벨

    def bfs():
        queue = deque()
        for u in range(1, N+1):
            if pairU[u] == 0:
                dist[u] = 0
                queue.append(u)
            else:
                dist[u] = float('inf')
        dist[0] = float('inf')

        while queue:
            u = queue.popleft()
            if dist[u] < dist[0]:
                for v in possible_tasks[u]:
                    w = pairV[v]
                    if w == 0:
                        dist[0] = dist[u] + 1
                    elif dist[w] == float('inf'):
                        dist[w] = dist[u] + 1
                        queue.append(w)
        return dist[0] != float('inf')

    def dfs(start):
        # 비재귀 DFS 구현
        stack = [start]
        path_stack = []  # 경로 추적용
        visited_set = set([start])
        iter_map = {start: 0}
        
        while stack:
            u = stack[-1]
            if u == 0:
                # 0(NIL)에 도달했다는 것은 증가 경로를 찾았다는 것
                # 뒤로가면서 매칭 갱신
                # 경로 복원
                for (curr_u, curr_v) in reversed(path_stack):
                    pairV[curr_v] = curr_u
                    pairU[curr_u], curr_u = curr_v, pairU[curr_u]
                return True
            
            if iter_map[u] == len(possible_tasks[u]):
                stack.pop()
                if stack and path_stack and path_stack[-1][0] == u:
                    path_stack.pop()
                continue
            
            v = possible_tasks[u][iter_map[u]]
            iter_map[u] += 1
            w = pairV[v]
            if w == 0 or (dist[w] == dist[u]+1 and w not in visited_set):
                # 경로 확장 가능
                path_stack.append((u,v))
                if w != 0:
                    visited_set.add(w)
                    iter_map[w] = 0
                stack.append(w)
        
        return False

    matching = 0
    while bfs():
        for u in range(1, N+1):
            if pairU[u] == 0:
                if dfs(u):
                    matching += 1
    return matching

print(hopcroft_karp())
