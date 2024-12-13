import sys
input = sys.stdin.readline

N = int(input().strip())
heights = [int(input().strip()) for _ in range(N)]

stack = []
result = 0

for h in heights:
    count = 1  # 현재 키 h인 사람이 1명 나왔음을 의미
    # 스택 top에 있는 사람들의 키가 현재 사람의 키보다 작으면 pop
    while stack and stack[-1][0] < h:
        height, cnt = stack.pop()
        result += cnt  # pop되는 모든 사람들과 볼 수 있는 쌍 형성
    # 현재 사람과 같은 키가 스택 top에 있으면
    if stack and stack[-1][0] == h:
        # 같은 키의 연속 구간을 꺼낸다.
        height, cnt = stack.pop()
        result += cnt  # 같은 키끼리 서로 볼 수 있는 쌍
        
        # 스택이 아직 남아 있으면 (앞쪽 사람과 현재 사람 쌍 추가)
        if stack:
            result += 1
        
        # 현재 사람을 다시 넣되, 같은 키 개수를 갱신
        stack.append((h, cnt + 1))
    else:
        # 현재 키가 더 크거나 같지 않은 경우,
        # 스택이 비어있지 않다면 이전 사람과 현재 사람 쌍 1개 형성
        if stack:
            result += 1
        stack.append((h, 1))

print(result)
