import sys,math
input = sys.stdin.readline

# 입력 읽기
N = int(input())
points = []
for _ in range(N):
    x, y, w = map(int, input().split())
    points.append((x, y, w))

# 빈 영역(아무 점도 포함하지 않는 경우)의 profit = 0 를 고려
best_profit = 0

# 모든 점에 대해, profit는 단순히 w (gold면 양, rock이면 음)
# 후보 각도의 집합: 
#   최적의 스트립은 두 평행선 중 적어도 한 선이 한 점을 지나도록 할 수 있음.
#   따라서 theta=0 (즉, x축 기준)와, 두 점 i,j에 대해
#       (x_i - x_j) cosθ + (y_i - y_j) sinθ = 0  ⇨  θ = (atan2(y_i-y_j, x_i-x_j) + π/2) mod π 
# 인 θ들이 후보가 된다.
cand = set()
cand.add(0.0)
for i in range(N):
    x1, y1, _ = points[i]
    for j in range(i+1, N):
        x2, y2, _ = points[j]
        # 두 점으로부터 후보 각도 구하기
        theta = math.atan2(y1 - y2, x1 - x2) + math.pi/2
        theta %= math.pi  # [0, π) 범위로 정규화
        cand.add(theta)

# (실제 N이 큰 경우 후보 개수가 매우 많아지므로, 아래 방식은 시간초과 위험이 있음)
# 필요에 따라 아래와 같이 후보 각도를 클러스터링(유사한 각도끼리 대표 1개만 사용)할 수 있음.
eps = 1e-12
cand_list = sorted(cand)
filtered = []
prev = -10.0
for theta in cand_list:
    if theta - prev > eps:
        filtered.append(theta)
        prev = theta
# 이제 filtered에 후보 각도가 들어있다.
# (작은 N의 경우에는 filtered의 크기가 크게 문제되지 않음)
    
# 각 후보 theta에 대해,
#  1. 모든 점의 투영값 = x*cosθ + y*sinθ 를 구하고 정렬
#  2. 정렬된 순서에 대해, 연속 구간 (스트립에 포함되는 점 집합)의 profit 합의 최대값(=Kadane 알고리즘) 계산
for theta in filtered:
    c = math.cos(theta)
    s = math.sin(theta)
    proj = []
    for (x, y, w) in points:
        proj.append((x * c + y * s, w))
    # 투영값에 따라 정렬 (동일 투영값일 경우 w 순)
    proj.sort(key=lambda t: (t[0], t[1]))
    
    # Kadane 알고리즘으로 최대 연속 부분합(빈 구간 포함 → 음수면 0 선택) 계산
    curr = 0
    local_best = 0
    for _, w in proj:
        curr = max(0, curr + w)
        if curr > local_best:
            local_best = curr
    if local_best > best_profit:
        best_profit = local_best

print(best_profit)
