def kmp_table(pattern):
    # KMP 전처리 테이블 생성
    table = [0]*len(pattern)
    j = 0
    for i in range(1, len(pattern)):
        while j > 0 and pattern[i] != pattern[j]:
            j = table[j-1]
        if pattern[i] == pattern[j]:
            j += 1
            table[i] = j
    return table

def kmp_search(text, pattern):
    # KMP를 이용한 패턴 매칭
    table = kmp_table(pattern)
    j = 0
    for i in range(len(text)):
        while j > 0 and text[i] != pattern[j]:
            j = table[j-1]
        if text[i] == pattern[j]:
            j += 1
            if j == len(pattern):
                return True
    return False

def solve():
    import sys
    input = sys.stdin.readline

    n = int(input().strip())
    A = list(map(int, input().split()))
    B = list(map(int, input().split()))

    A.sort()
    B.sort()

    # 각 배열의 차이 배열 계산
    def get_diff(arr):
        diff = []
        for i in range(n-1):
            diff.append(arr[i+1]-arr[i])
        diff.append(360000 - arr[-1] + arr[0])
        return diff

    D_A = get_diff(A)
    D_B = get_diff(B)

    # KMP 이용: D_B가 D_A*2의 부분수열인지 확인
    D_AA = D_A + D_A  # 두 번 이어붙이기

    if kmp_search(D_AA, D_B):
        print("possible")
    else:
        print("impossible")

if __name__ == "__main__":
    solve()
