x, y = map(int, input().split())

if y < x:
    # 현재 역에서 관으로 가는 중
    print(y + x)
else:
    # 현재 관에서 역으로 가는 중
    print(y - x)
