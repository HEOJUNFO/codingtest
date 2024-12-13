import sys
input = sys.stdin.readline

# Persistent Segment Tree
# 각 노드는 (left_child, right_child, sum)로 구성
# 0번 노드는 아무것도 없는 null로 사용 (왼/오 없음, 합 0)
MAXN = 100000

# 전역 변수
left_child = [0]*(40*MAXN)
right_child = [0]*(40*MAXN)
node_sum = [0]*(40*MAXN)
ptr = 1  # 다음에 할당할 노드 인덱스 (1부터 시작)

def build(arr, start, end):
    """초기 세그먼트 트리 빌드 (버전0)"""
    global ptr
    if start == end:
        # 새로운 노드 할당
        idx = ptr
        ptr += 1
        node_sum[idx] = arr[start-1]
        return idx
    mid = (start + end) // 2
    idx = ptr
    ptr += 1
    left_child[idx] = build(arr, start, mid)
    right_child[idx] = build(arr, mid+1, end)
    node_sum[idx] = node_sum[left_child[idx]] + node_sum[right_child[idx]]
    return idx

def update(prev_root, start, end, pos, val):
    """prev_root를 기반으로 pos 위치를 val로 업데이트한 새로운 버전 트리의 루트 노드 반환"""
    global ptr
    idx = ptr
    ptr += 1
    if start == end:
        node_sum[idx] = val
        return idx
    mid = (start + end) // 2
    if pos <= mid:
        left_child[idx] = update(left_child[prev_root], start, mid, pos, val)
        right_child[idx] = right_child[prev_root]
    else:
        left_child[idx] = left_child[prev_root]
        right_child[idx] = update(right_child[prev_root], mid+1, end, pos, val)
    node_sum[idx] = node_sum[left_child[idx]] + node_sum[right_child[idx]]
    return idx

def query(root, start, end, left, right):
    """root 버전에서 [left, right] 구간합 반환"""
    if right < start or end < left:
        return 0
    if left <= start and end <= right:
        return node_sum[root]
    mid = (start + end) // 2
    return query(left_child[root], start, mid, left, right) + query(right_child[root], mid+1, end, left, right)


N = int(input())
arr = list(map(int, input().split()))
M = int(input())

# 버전별 루트 저장
roots = [0]*(M+1)  # 최대 M번 업데이트 가능
# version 0 : 초기 상태 트리
roots[0] = build(arr, 1, N)

update_count = 0
for _q in range(M):
    q = list(map(int, input().split()))
    if q[0] == 1:
        # 1 i v : Ai = v
        i, v = q[1], q[2]
        update_count += 1
        roots[update_count] = update(roots[update_count-1], 1, N, i, v)

    else:
        # 2 k i j : k번째 1번 쿼리까지 적용된 트리에서 i~j 합
        k, i, j = q[1], q[2], q[3]
        print(query(roots[k], 1, N, i, j))
