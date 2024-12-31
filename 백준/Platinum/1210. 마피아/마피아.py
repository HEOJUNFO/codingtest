import sys
from collections import deque

input = sys.stdin.readline
INF = 10**15  # 충분히 큰 값(무한대 대신 사용)

class Dinic:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]

    def add_edge(self, u, v, cap):
        """유량 그래프에 (u -> v, 용량=cap) 간선을 추가"""
        self.adj[u].append([v, cap, len(self.adj[v])])
        # 역방향 간선은 용량을 0으로(잔여 용량 관리용)
        self.adj[v].append([u, 0, len(self.adj[u]) - 1])

    def bfs(self, s, t, level):
        """레벨 그래프(Level Graph) 구성"""
        for i in range(len(level)):
            level[i] = -1
        level[s] = 0
        queue = deque([s])
        while queue:
            u = queue.popleft()
            for v, cap, rev in self.adj[u]:
                # 잔여 용량이 남아 있고, 아직 방문하지 않았다면
                if cap > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    queue.append(v)
        return level[t] >= 0

    def send_flow(self, u, flow, t, level, it):
        """DFS를 사용하여 유량 보내기"""
        if u == t:
            return flow
        while it[u] < len(self.adj[u]):
            v, cap, rev = self.adj[u][it[u]]
            if cap > 0 and level[v] == level[u] + 1:
                curr_flow = min(flow, cap)
                temp_flow = self.send_flow(v, curr_flow, t, level, it)
                if temp_flow > 0:
                    # 잔여 용량 업데이트
                    self.adj[u][it[u]][1] -= temp_flow
                    self.adj[v][rev][1] += temp_flow
                    return temp_flow
            it[u] += 1
        return 0

    def max_flow(self, s, t):
        """Dinic 알고리즘으로 최대 유량 계산"""
        if s == t:
            return -1  # 보통 s != t 가정
        total = 0
        level = [-1]*self.n
        # 가능한 동안 BFS + DFS로 유량을 보냄
        while self.bfs(s, t, level):
            it = [0]*self.n
            while True:
                flow = self.send_flow(s, INF, t, level, it)
                if flow <= 0:
                    break
                total += flow
        return total

def solve():
    n, m = map(int, input().split())
    S, T = map(int, input().split())
    # 인덱스를 0-based 로 맞추기 위해 조정
    S -= 1
    T -= 1
    
    cost = [int(input()) for _ in range(n)]

    # 그래프를 구성할 때 각 노드를 in, out 으로 분할
    # node_in = i, node_out = i + n (i는 0-based)
    # 따라서 총 2*n 개의 정점이 필요
    graph = Dinic(2*n)

    # 각 노드 i에 대해 (i_in -> i_out) 간선 용량 = cost[i]
    for i in range(n):
        i_in = i
        i_out = i + n
        graph.add_edge(i_in, i_out, cost[i])

    # 주어진 m개의 간선(u, v)에 대해
    # (u_out -> v_in) 과 (v_out -> u_in) 에 무한대 용량 간선을 추가
    for _ in range(m):
        u, v = map(int, input().split())
        u -= 1
        v -= 1
        u_in, u_out = u, u + n
        v_in, v_out = v, v + n
        # 무한대로 설정(실제로는 매우 큰 수 사용)
        graph.add_edge(u_out, v_in, INF)
        graph.add_edge(v_out, u_in, INF)

    # Dinic 알고리즘으로 최대 유량 계산
    # 소스: S_in = S, 싱크: T_out = T + n
    source = S
    sink = T + n
    _ = graph.max_flow(source, sink)

    # 이제 잔여 그래프에서 source(S_in)로부터 도달 가능한 정점을 찾는다.
    visited = [False]*(2*n)
    queue = deque([source])
    visited[source] = True

    while queue:
        u = queue.popleft()
        for v, cap, rev in graph.adj[u]:
            # 잔여 용량이 남아있고 아직 방문하지 않았다면 방문
            if cap > 0 and not visited[v]:
                visited[v] = True
                queue.append(v)

    # 최소 컷에 포함되는 톨게이트 찾기
    # 조건: i_in 은 방문했지만(i_in에 True) i_out은 방문 못했다면(i_out에 False)
    answer = []
    for i in range(n):
        i_in, i_out = i, i + n
        if visited[i_in] and not visited[i_out]:
            answer.append(i+1)  # 1-based 로 복구

    # 오름차순 출력
    print(*sorted(answer))

# 입력을 받아서 solve 함수 실행
if __name__ == "__main__":
    solve()
