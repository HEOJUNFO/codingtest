import sys
input = sys.stdin.readline

MAX_TASTE = 1000000

# Fenwick Tree (Binary Indexed Tree) 구현
fenwicks = [0] * (MAX_TASTE + 1)

def update(idx, val):
    while idx <= MAX_TASTE:
        fenwicks[idx] += val
        idx += idx & (-idx)

def query(idx):
    # 1 ~ idx까지의 합
    s = 0
    while idx > 0:
        s += fenwicks[idx]
        idx -= idx & (-idx)
    return s

def find_kth(k):
    # fenwicks를 이용해 k번째 사탕의 맛을 찾는다.
    # 이진 탐색을 통해 구현
    pos = 0
    # 2^20 = 1,048,576 > 1,000,000
    # Fenwick Tree 상에서 교차점을 찾기 위해 가장 큰 2의 거듭제곱을 찾는다.
    bit_mask = 1 << 20
    while bit_mask > 0:
        next_pos = pos + bit_mask
        if next_pos <= MAX_TASTE and fenwicks[next_pos] < k:
            k -= fenwicks[next_pos]
            pos = next_pos
        bit_mask >>= 1
    return pos + 1  # 실제 맛 번호

n = int(input())
for _ in range(n):
    line = list(map(int, input().split()))
    if line[0] == 1:
        # B번째로 맛있는 사탕 꺼내기
        B = line[1]
        taste = find_kth(B)
        print(taste)
        update(taste, -1)
    else:
        # B맛 사탕 C개 추가 또는 제거
        B, C = line[1], line[2]
        update(B, C)
