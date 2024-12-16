import sys
input = sys.stdin.readline

R, C = map(int, input().split())
N = int(input())
heights = list(map(int, input().split()))

freq = {}
for h in heights:
    freq[h] = freq.get(h, 0) + 1

# 각 높이에 대해 min(freq(h), C) 구하고 합산
total = 0
for h in freq:
    total += min(freq[h], C)

# 최대 R*C까지만 가능
answer = min(total, R*C)

print(answer)
