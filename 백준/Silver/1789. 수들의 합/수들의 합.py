import math

S = int(input().strip())
# N(N+1)/2 <= S
# N^2 + N - 2S <= 0
# 근의 공식을 이용해 N 구하기
N = int((math.sqrt(1 + 8*S) - 1) // 2)
print(N)
