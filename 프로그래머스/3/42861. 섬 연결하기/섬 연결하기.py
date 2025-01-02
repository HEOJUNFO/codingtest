def solution(n, costs):
    # 1. 비용 기준으로 오름차순 정렬
    costs.sort(key=lambda x: x[2])
    
    # 2. Union-Find 자료구조 초기화
    # parent[i] = i로 초기화: 각 원소가 자기 자신을 부모로 갖도록 설정
    parent = [i for i in range(n)]
    
    # find 함수(경로 압축 기법 적용)
    def find(x):
        if parent[x] != x:
            parent[x] = find(parent[x])
        return parent[x]
    
    # union 함수
    def union(a, b):
        rootA = find(a)
        rootB = find(b)
        if rootA < rootB:
            parent[rootB] = rootA
        else:
            parent[rootA] = rootB
    
    total_cost = 0  # 최소 스패닝 트리에 포함된 간선 비용 합
    edge_count = 0  # 선택한 간선 수

    # 3. 간선들을 하나씩 확인하며 MST를 구성
    for a, b, cost in costs:
        # a, b의 부모(루트)를 확인
        if find(a) != find(b):
            # 부모가 다르면 사이클이 형성되지 않으므로 간선 포함
            union(a, b)
            total_cost += cost
            edge_count += 1
            # 모든 섬이 연결되었다면 break
            if edge_count == n - 1:
                break

    return total_cost
