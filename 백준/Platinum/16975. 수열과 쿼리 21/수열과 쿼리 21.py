import sys
input = sys.stdin.readline

# 펜윅 트리(Fenwick Tree, BIT)를 이용한 풀이
# 
# 접근 방법:
# 1. 구간에 값을 더하는 쿼리를 효율적으로 처리하기 위해,
#    A 배열을 차분 배열 B로 변환한다.
#    B[1] = A[1], B[i] = A[i] - A[i-1] (i >= 2)
# 
# 2. B 배열을 Fenwick Tree에 저장한다. Fenwick Tree는 단일 원소 업데이트,
#    prefix sum 쿼리를 O(log N)에 처리할 수 있다.
# 
# 3. 구간 [i, j]에 k를 더하는 쿼리 (1 i j k)를 B에 반영하는 방법:
#    B[i]에 k를 더하고, 만약 j+1 <= N이면 B[j+1]에 -k를 더한다.
#    이를 Fenwick Tree를 통해 update하면, 나중에 특정 원소 A[x]를 구할 때
#    Fenwick Tree의 prefix sum을 구하면 A[x]를 얻을 수 있다.
#
# 4. A[x]를 얻는 쿼리(2 x)는 Fenwick Tree에서 1부터 x까지의 합을 구하면 된다.
#
# 이 방법으로 모든 쿼리를 효율적으로 처리할 수 있다.

N = int(input().strip())
A = list(map(int, input().split()))
M = int(input().strip())

# Fenwick Tree 기본 함수 정의
def fenwicks_update(i, v):
    while i <= N:
        fenwicks[i] += v
        i += (i & -i)

def fenwicks_sum(i):
    s = 0
    while i > 0:
        s += fenwicks[i]
        i -= (i & -i)
    return s

# B 배열 생성: B[1] = A[1], B[i] = A[i] - A[i-1] (for i >= 2)
B = [0]*(N+1)
B[1] = A[0]
for i in range(2, N+1):
    B[i] = A[i-1] - A[i-2]

# Fenwick Tree 초기화 및 B값 반영
fenwicks = [0]*(N+1)
for i in range(1, N+1):
    fenwicks_update(i, B[i])

# 쿼리 처리
for _ in range(M):
    query = list(map(int, input().split()))
    if query[0] == 1:
        _, i, j, k = query
        fenwicks_update(i, k)
        if j+1 <= N:
            fenwicks_update(j+1, -k)
    else:
        # query[0] == 2
        _, x = query
        # Fenwicks sum(x)는 A[x]를 구하는 것과 동일
        print(fenwicks_sum(x))
