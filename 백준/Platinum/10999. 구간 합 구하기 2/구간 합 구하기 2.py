import sys
input = sys.stdin.readline

# 구간 합 구하기 2 (백준 10999)
# 세그먼트 트리 + 레이지 프로파게이션을 이용한 구간 업데이트, 구간 합 쿼리
#
# 접근:
# - N개의 수에 대해 구간합 쿼리와 구간 업데이트(더하기) 쿼리를 처리하는 문제.
# - 세그먼트 트리를 사용하며, 구간 업데이트 시 현재 노드 범위 전체에 동일한 값을 더해야 하므로
#   lazy propagation(지연 전파) 기법을 사용한다.
#
# 구현 상세:
# - segment 배열: 구간 합을 저장하는 세그먼트 트리 배열
# - lazy 배열: 해당 구간에 아직 반영되지 않은 업데이트 값을 저장.
# - build: 초기 배열을 바탕으로 세그먼트 트리를 구축
# - update_range: [start, end] 구간에 value를 더하는 함수 (lazy propagation)
# - query_range: [start, end] 구간 합을 구하는 함수 (lazy propagation 적용)
#
# 시간 복잡도:
# - N 최대 1,000,000
# - 세그먼트 트리 구축: O(N)
# - M번의 업데이트, K번의 쿼리: 각 O(log N)
# - 전체적으로 M+K <= 20,000 이므로 log N ~ 20 정도에서 충분히 2초 시간 내 가능.
#
# 주의:
# - 인덱스가 1-based로 주어지므로 이를 그대로 사용할 수도 있고,
#   세그먼트 트리를 1-based 인덱스로 구축하는 것이 일반적이다.
# - 파이썬 I/O 최적화를 위해 sys.stdin.readline 사용.
# - 정수 범위가 매우 크므로 (64비트 정수) Python의 int 사용에 문제 없음.
#
# 출력:
# - 쿼리(a=2)시에만 결과 출력

def build(node, start, end):
    if start == end:
        segment[node] = arr[start-1]
        return
    mid = (start + end) // 2
    build(node*2, start, mid)
    build(node*2+1, mid+1, end)
    segment[node] = segment[node*2] + segment[node*2+1]

def update_lazy(node, start, end):
    # lazy[node]가 0이 아니라면 현재 node에 lazy값 적용
    if lazy[node] != 0:
        segment[node] += (end - start + 1) * lazy[node]
        # 자식노드가 있을 경우 lazy값 전달
        if start != end:
            lazy[node*2] += lazy[node]
            lazy[node*2+1] += lazy[node]
        # 현재 노드의 lazy값 초기화
        lazy[node] = 0

def update_range(node, start, end, left, right, val):
    # 먼저 현재 노드에 반영되지 않은 lazy 값 처리
    update_lazy(node, start, end)

    if start > right or end < left:
        # 업데이트 구간과 전혀 겹치지 않음
        return
    if left <= start and end <= right:
        # 현재 구간이 업데이트 구간에 완전히 포함됨
        segment[node] += (end - start + 1) * val
        if start != end:
            lazy[node*2] += val
            lazy[node*2+1] += val
        return
    mid = (start + end) // 2
    update_range(node*2, start, mid, left, right, val)
    update_range(node*2+1, mid+1, end, left, right, val)
    segment[node] = segment[node*2] + segment[node*2+1]

def query_range(node, start, end, left, right):
    # lazy값 먼저 처리
    update_lazy(node, start, end)

    if start > right or end < left:
        return 0
    if left <= start and end <= right:
        return segment[node]
    mid = (start + end) // 2
    return query_range(node*2, start, mid, left, right) + query_range(node*2+1, mid+1, end, left, right)


N, M, K = map(int, input().split())
arr = [int(input()) for _ in range(N)]

segment = [0]*(4*N)
lazy = [0]*(4*N)

# 세그먼트 트리 구축
build(1, 1, N)

for _ in range(M+K):
    line = list(map(int, input().split()))
    if line[0] == 1:
        # 업데이트 쿼리: 1 b c d -> b부터 c까지 각 원소에 d를 더하기
        _, b, c, d = line
        update_range(1, 1, N, b, c, d)
    else:
        # 쿼리: 2 b c -> b부터 c까지의 합
        _, b, c = line
        print(query_range(1, 1, N, b, c))
