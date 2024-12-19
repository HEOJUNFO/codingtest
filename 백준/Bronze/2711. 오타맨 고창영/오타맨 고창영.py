import sys

input = sys.stdin.readline

T = int(input().strip())
for _ in range(T):
    data = input().strip().split()
    pos = int(data[0])
    word = data[1]
    # 오타를 낸 위치의 문자를 제거
    # 문자열의 첫 문자가 1번째이므로, Python 인덱스로는 pos-1
    corrected = word[:pos-1] + word[pos:]
    print(corrected)
