import sys
import heapq

input = sys.stdin.readline
INF = float('inf')

def dijkstra(start, graph, dist):
    pq = []
    heapq.heappush(pq, (0, start))
    dist[start] = 0
    while pq:
        cur_dist, u = heapq.heappop(pq)
        if dist[u] < cur_dist:
            continue
        for v, w in graph[u]:
            nd = cur_dist + w
            if dist[v] > nd:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))

def solve():
    while True:
        N, M = map(int, input().split())
        if N == 0 and M == 0:
            break
        S, D = map(int, input().split())
        
        graph = [[] for _ in range(N)]
        reverse_graph = [[] for _ in range(N)]  # 역방향 그래프(최단 경로 역추적용)
        
        for _m in range(M):
            U, V, P = map(int, input().split())
            graph[U].append((V, P))
            reverse_graph[V].append((U, P))
        
        # 1. 첫 번째 최단 경로 계산
        dist = [INF] * N
        dijkstra(S, graph, dist)
        
        if dist[D] == INF:
            # D로 가는 경로가 없는 경우
            print(-1)
            continue
        
        shortest_dist = dist[D]
        
        # 2. 최단 경로에 포함된 간선을 찾고 제거
        # D에서 시작해서 역방향 그래프로 S까지 최단 경로에 포함된 간선을 찾는다.
        queue = [D]
        check = [False] * N
        check[D] = True
        edges_to_remove = [[] for _ in range(N)]
        
        while queue:
            u = queue.pop()
            for v, w in reverse_graph[u]:
                if dist[v] + w == dist[u]:  # 최단 경로 조건
                    # v->u 간선은 최단 경로에 포함된다.
                    edges_to_remove[v].append(u)
                    if not check[v]:
                        check[v] = True
                        queue.append(v)
        
        # 실제 그래프에서 최단 경로 간선 제거
        for u in range(N):
            if edges_to_remove[u]:
                # u에서 v로 가는 간선을 제거
                new_list = []
                remove_set = set(edges_to_remove[u])
                for (v, w) in graph[u]:
                    if v not in remove_set:
                        new_list.append((v, w))
                graph[u] = new_list
        
        # 3. 거의 최단 경로 재계산
        dist2 = [INF]*N
        dijkstra(S, graph, dist2)
        
        if dist2[D] == INF:
            print(-1)
        else:
            print(dist2[D])

# 메인 함수 실행
if __name__ == "__main__":
    solve()
