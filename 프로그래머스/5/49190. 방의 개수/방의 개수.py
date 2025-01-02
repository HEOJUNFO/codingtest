def solution(arrows):
    # 8방향 정의 (수직·수평 ±2, 대각 ±2씩 이동하는 식으로 "반 정수" 처리를 쉽게 하기 위해 스케일 2배)
    # 0: 위, 1: 오른위, 2: 오른쪽, 3: 오른아래, 4: 아래, 5: 왼아래, 6: 왼쪽, 7: 왼위
    direction = [
        (0, 2),   # 0
        (2, 2),   # 1
        (2, 0),   # 2
        (2, -2),  # 3
        (0, -2),  # 4
        (-2, -2), # 5
        (-2, 0),  # 6
        (-2, 2)   # 7
    ]
    
    # Union-Find(Disjoint Set) 구현부
    parent = dict()  # 각 노드 -> 부모 노드를 저장
    rank = dict()    # Union by rank 최적화용

    def find(u):
        # 경로 압축(Path Compression)
        if parent[u] != u:
            parent[u] = find(parent[u])
        return parent[u]

    def union(u, v):
        rootU = find(u)
        rootV = find(v)
        if rootU == rootV:
            return False  # 이미 같은 집합
        # Union by rank
        if rank[rootU] < rank[rootV]:
            parent[rootU] = rootV
        elif rank[rootU] > rank[rootV]:
            parent[rootV] = rootU
        else:
            parent[rootV] = rootU
            rank[rootU] += 1
        return True

    def add_node(node):
        # 아직 parent에 없다면 초기화
        if node not in parent:
            parent[node] = node
            rank[node] = 0

    # 간선 중복 체크용 (set에 frozenset({u,v}) 형태로 저장)
    visited_edges = set()

    # 시작 노드(0, 0) - 여기서는 (x, y)를 (0, 0)에 해당하는 "2배 스케일" 좌표계로 둠
    cur_x, cur_y = 0, 0
    add_node((cur_x, cur_y))

    rooms = 0

    for arr in arrows:
        dx, dy = direction[arr]
        next_x = cur_x + dx
        next_y = cur_y + dy
        
        # 대각선 이동인지 확인
        # (dx, dy) 둘 다 0이 아닌 경우 == 대각선
        if dx != 0 and dy != 0:
            # 1) 중간 지점(half point)
            mid_x = cur_x + dx // 2
            mid_y = cur_y + dy // 2
            
            # 노드 등록
            add_node((mid_x, mid_y))
            add_node((next_x, next_y))
            
            # 간선 1: cur -> mid
            edge1 = frozenset({(cur_x, cur_y), (mid_x, mid_y)})
            if edge1 not in visited_edges:
                visited_edges.add(edge1)
                # 같은 집합이면 방 1개 추가
                if not union((cur_x, cur_y), (mid_x, mid_y)):
                    rooms += 1
            
            # 간선 2: mid -> next
            edge2 = frozenset({(mid_x, mid_y), (next_x, next_y)})
            if edge2 not in visited_edges:
                visited_edges.add(edge2)
                if not union((mid_x, mid_y), (next_x, next_y)):
                    rooms += 1
        else:
            # 수평·수직 이동
            add_node((next_x, next_y))
            edge = frozenset({(cur_x, cur_y), (next_x, next_y)})
            if edge not in visited_edges:
                visited_edges.add(edge)
                if not union((cur_x, cur_y), (next_x, next_y)):
                    rooms += 1

        # 현재 위치 갱신
        cur_x, cur_y = next_x, next_y

    return rooms


# 예제 테스트
if __name__ == "__main__":
    arrows = [6, 6, 6, 4, 4, 4, 2, 2, 2, 0, 0, 0, 1, 6, 5, 5, 3, 6, 0]
    print(solution(arrows))  # 기댓값 = 3
