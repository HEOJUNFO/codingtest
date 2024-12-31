import sys
sys.setrecursionlimit(10**7)
input = sys.stdin.readline

MOD = 10**9 + 7

class Dinic:
    def __init__(self, n):
        self.n = n
        # 그래프 인접 리스트: 각 노드마다 (연결된노드 to, 역방향인덱스 rev, 현재 잔여용량 cap) 형태로 저장
        self.adj = [[] for _ in range(n)]
        
    def add_edge(self, u, v, cap):
        # u -> v 용량 cap
        self.adj[u].append([v, len(self.adj[v]), cap])
        # v -> u 용량 0(초기에 역방향은 0으로), 하지만 문제는 '무방향'이므로 원래 간선도 v->u cap이어야 한다.
        # 따라서 여기서는 "무방향"을 만족하려면 추가로 v->u도 cap이어야 한다.
        # 다만 Dinic의 일반적 틀에서 '무방향' 처리를 위해선 정방향/역방향 두 쌍을 모두 cap으로 넣어준다.
        self.adj[v].append([u, len(self.adj[u]) - 1, cap])
        
    def bfs(self, start, end):
        """ 레벨 그래프 구성 """
        self.level = [-1] * self.n
        self.level[start] = 0
        queue = [start]
        q_idx = 0
        while q_idx < len(queue):
            u = queue[q_idx]
            q_idx += 1
            for v, rev, cap in self.adj[u]:
                # 아직 방문 안했고, 잔여용량이 남아있으면
                if cap > 0 and self.level[v] == -1:
                    self.level[v] = self.level[u] + 1
                    queue.append(v)
        return self.level[end] != -1  # end까지의 레벨이 구성되었으면 True
    
    def send_flow(self, u, flow, end, it):
        """ DFS로 blocking flow를 전진 탐색 """
        if u == end:
            return flow
        
        while it[u] < len(self.adj[u]):
            v, rev, cap = self.adj[u][it[u]]
            
            # 레벨 그래프 순서 맞고, 용량 남으면
            if cap > 0 and self.level[v] == self.level[u] + 1:
                # 보낼 수 있는 최대 유량
                curr_flow = min(flow, cap)
                temp_flow = self.send_flow(v, curr_flow, end, it)
                
                # 실제로 보낼 수 있었다면
                if temp_flow > 0:
                    # 잔여용량 갱신
                    self.adj[u][it[u]][2] -= temp_flow
                    # 역방향 용량 증가
                    self.adj[v][rev][2] += temp_flow
                    return temp_flow
            it[u] += 1
        return 0
    
    def max_flow(self, s, t):
        """ Dinic 최대 유량 계산 """
        total_flow = 0
        
        # 레벨 그래프가 더 이상 구성되지 않을 때까지 반복
        while self.bfs(s, t):
            it = [0]*self.n
            while True:
                flow = self.send_flow(s, float('inf'), t, it)
                if flow <= 0:
                    break
                total_flow += flow
        return total_flow


def solve():
    N, M = map(int, input().split())
    
    # 간선이 없으면 최대 유량 0
    if M == 0:
        print(0)
        return
    
    # Dinic 준비
    dinic = Dinic(N)
    
    # 도로 i의 용량: 3^i
    # 미리 pow(3, i) 를 계산하여 저장해두자. (파이썬은 큰 정수도 지원)
    # 다만 i가 최대 2000까지이므로, 한 번에 계산해두는 것이 낫다.
    capacities = [1] * M
    # capacities[0] = 3^0 = 1
    for i in range(1, M):
        capacities[i] = capacities[i-1] * 3  # 직전 값에 3을 곱해나가면 됨
    
    edges = []
    for i in range(M):
        a, b = map(int, input().split())
        edges.append((a, b))
    
    # 그래프에 간선 추가 (무방향이므로 Dinic에선 (u->v, v->u) 둘 다 cap으로)
    for i, (a, b) in enumerate(edges):
        cap = capacities[i]
        dinic.add_edge(a, b, cap)
    
    # 0번에서 N-1번까지 최대 유량
    flow_val = dinic.max_flow(0, N-1)
    
    # 출력: flow_val mod (10^9 + 7)
    print(flow_val % MOD)


if __name__ == "__main__":
    solve()
