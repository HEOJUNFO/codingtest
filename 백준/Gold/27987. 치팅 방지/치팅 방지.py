import sys
import heapq

input = sys.stdin.readline

N = int(input().strip())
K = list(map(int, input().split()))
M = sum(K)

# 만약 모든 반의 학생 수가 1명 이하라면(즉, 모두 1명이거나 일부 0명일 경우),
# 동일한 반 학생 둘 이상이 없으므로 원하는 대로 출력 가능
if max(K) <= 1:
    # 모든 반의 순서: 그냥 1반부터 순서대로 출력
    # 사실 어떤 순서로 출력해도 같은 반 학생 간 거리가 무한대처럼 취급 가능
    result = []
    for i, cnt in enumerate(K, start=1):
        result.extend([i]*cnt)
    print(" ".join(map(str, result)))
    sys.exit(0)

# check(d): 최소 거리가 d 이상 되는 배치 가능 여부 및 실제 배치 결과 반환
def check(d):
    # 힙에는 (-남은수, 반번호)를 넣어 남은 수가 많은 순으로 뽑도록 한다.
    pq = []
    for i, cnt in enumerate(K, start=1):
        if cnt > 0:
            heapq.heappush(pq, (-cnt, i))
    
    # waitQ: (재삽입 시점, 남은수, 반번호)
    # 배치 후 d만큼 지나야 다시 삽입 가능하므로, 현재 step+i+d에서 다시 삽입
    waitQ = []
    
    result = []
    for i in range(1, M+1):
        # i 시점에 재삽입 가능한 반 있으면 힙에 복귀
        while waitQ and waitQ[0][0] == i:
            _, c_cnt, c_id = heapq.heappop(waitQ)
            heapq.heappush(pq, (-c_cnt, c_id))
        
        if not pq:
            # 배치 불가능
            return False, []
        
        c_cnt, c_id = heapq.heappop(pq)
        c_cnt = -c_cnt
        
        # 현재 반 학생 하나 배치
        result.append(c_id)
        c_cnt -= 1
        if c_cnt > 0:
            # 다음에 이 반을 다시 사용할 수 있는 시점은 i+d
            # i+d 위치에서 사용 가능하도록 대기열에 넣는다.
            heapq.heappush(waitQ, (i+d, c_cnt, c_id))
    
    return True, result

# 이분 탐색으로 최대 d 찾기
low, high = 1, M
best_d = 1
best_arr = []
while low <= high:
    mid = (low + high) // 2
    feasible, arr = check(mid)
    if feasible:
        best_d = mid
        best_arr = arr
        low = mid + 1
    else:
        high = mid - 1

# best_d로 만든 결과를 출력
print(" ".join(map(str, best_arr)))
