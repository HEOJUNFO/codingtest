import sys
input = sys.stdin.readline

N, K, D = map(int, input().split())

students = []
for _ in range(N):
    M, d = map(int, input().split())
    algos = list(map(int, input().split()))
    students.append((d, algos))

# 실력에 따라 정렬
students.sort(key=lambda x: x[0])

count = [0]*K  # 각 알고리즘을 몇 명이 알고 있는지 카운트
union_count = 0  # 알고리즘 합집합의 크기 (알고 있는 학생이 최소 1명 있는 알고리즘 수)
max_E = 0

l = 0
for r in range(N):
    # r번째 학생을 윈도우에 추가
    for a in students[r][1]:
        a_idx = a-1
        if count[a_idx] == 0:
            union_count += 1
        count[a_idx] += 1

    # 윈도우 내 실력 차이가 D를 초과하면 l을 이동
    while students[r][0] - students[l][0] > D:
        # l번째 학생을 윈도우에서 제거
        for a in students[l][1]:
            a_idx = a-1
            count[a_idx] -= 1
            if count[a_idx] == 0:
                union_count -= 1
        l += 1

    window_size = r - l + 1
    # intersection_count 계산: count[a] == window_size인 알고리즘 개수
    intersection_count = 0
    for c in count:
        if c == window_size:
            intersection_count += 1

    E = (union_count - intersection_count) * window_size
    if E > max_E:
        max_E = E

print(max_E)
