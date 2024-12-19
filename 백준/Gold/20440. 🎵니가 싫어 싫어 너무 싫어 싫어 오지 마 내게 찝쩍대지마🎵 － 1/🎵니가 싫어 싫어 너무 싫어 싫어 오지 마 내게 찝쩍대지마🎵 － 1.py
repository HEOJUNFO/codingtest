import sys
from collections import defaultdict

input = sys.stdin.readline

n = int(input().strip())
d = defaultdict(int)

for _ in range(n):
    s, e = map(int, input().split())
    d[s] += 1
    d[e] -= 1

m, cnt = 0, 0
tem, txm = 0, 0
flag = False  # 모기 최댓값 상태를 유지하는 동안 구간 종료 시점 확인용

# 시간 순으로 정렬
for t in sorted(d.keys()):
    prev_cnt = cnt
    cnt += d[t]
    
    # 모기 수 최댓값 갱신
    if cnt > m:
        m = cnt
        tem = t
        flag = True
    # 최대값 상태에서 모기 수가 감소하는 순간 구간 종료
    elif flag and cnt < m and prev_cnt == m:
        txm = t
        flag = False

print(m)
print(tem, txm)
