import sys
input = sys.stdin.readline

n, k, a = map(int, input().split())
m = int(input())
missiles = list(map(int, input().split()))

# 이분 탐색을 위한 함수
# x번째 미사일까지 고려했을 때 전함 k개를 배치할 수 있는지 확인
def can_place(x):
    # x번째 미사일까지의 금지 칸을 추출
    forbidden = sorted(missiles[:x])
    
    # 금지 구간 사이의 안전 구간 길이를 계산
    # 시작점은 1부터
    prev = 0
    count = 0
    for f in forbidden:
        length = f - prev - 1  # 이전 금지칸 다음부터 현재 금지칸 이전까지
        if length > 0:
            # 이 구간에 배치 가능한 전함 수
            count += (length+1)//(a+1)
        prev = f
    # 마지막 구간(마지막 금지 칸 이후 ~ n)
    length = n - prev
    if length > 0:
        count += (length+1)//(a+1)
    
    return count >= k

left, right = 1, m+1
answer = -1
while left < right:
    mid = (left+right)//2
    if can_place(mid):
        # mid번째 미사일까지 피할 수 있다면, 더 많은 미사일 고려
        left = mid+1
    else:
        # mid번째 미사일에서 피할 수 없다면, 더 적게 고려
        answer = mid
        right = mid

# answer가 -1이면 모든 미사일을 피해 배치 가능
print(answer if answer != -1 else -1)
