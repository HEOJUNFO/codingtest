import sys
sys.setrecursionlimit(10**7)

def solve():
    input = sys.stdin.readline
    
    H, W = map(int, input().split())
    board = [input().rstrip() for _ in range(H)]
    
    # dp[r1][r2][c1][c2] := 보드의 (r1~r2, c1~c2) 구역에 대한 그런디 값
    # 초기값 -1은 아직 계산 안 됨을 의미
    dp = [[[[ -1 for _ in range(W)] for _ in range(W)] 
                    for _ in range(H)] for _ in range(H)]
    
    # mex 함수를 구하는 보조 함수
    def mex(s):
        """ 
        s(정수 집합)에 포함되지 않은 가장 작은 비음이 아닌 정수를 찾는다.
        """
        g = 0
        while g in s:
            g += 1
        return g
    
    def grundy(r1, r2, c1, c2):
        # 범위가 말이 안 되면(빈 구역) 그런디 값 = 0
        if r1 > r2 or c1 > c2:
            return 0
        if dp[r1][r2][c1][c2] != -1:
            return dp[r1][r2][c1][c2]
        
        # 가능한 XOR 값들을 저장할 집합
        xor_values = set()
        
        # 현재 구역 내에 빈 칸('.')이 있는지, 그리고 선택했을 때 어떻게 분할되는지 계산
        found_empty = False
        
        for r in range(r1, r2+1):
            for c in range(c1, c2+1):
                if board[r][c] == '.':
                    found_empty = True
                    # (r, c)를 고르면, 십자가로 잘린 후 4개의 부분 구역이 생김
                    # 위쪽
                    g1 = grundy(r1, r-1, c1, c-1)
                    # 왼쪽 위, 오른쪽 위로 나눠 쓰는 대신
                    # 여기서는 (r1 ~ r-1, c+1 ~ c2)가 "위쪽-오른쪽" 부분
                    g2 = grundy(r1, r-1, c+1, c2)
                    # 아래쪽-왼쪽
                    g3 = grundy(r+1, r2, c1, c-1)
                    # 아래쪽-오른쪽
                    g4 = grundy(r+1, r2, c+1, c2)
                    
                    xor_values.add(g1 ^ g2 ^ g3 ^ g4)
        
        # 빈 칸이 하나도 없으면 (이미 전부 벽이거나 'X' 뿐이면) 그런디값 = 0
        if not found_empty:
            dp[r1][r2][c1][c2] = 0
            return 0
        
        # 그렇지 않으면 mex 로 계산
        g = mex(xor_values)
        dp[r1][r2][c1][c2] = g
        return g
    
    # 전체 보드 (0, H-1, 0, W-1)에 대한 그런디 값
    G = grundy(0, H-1, 0, W-1)
    
    # 그런디 값이 0이면 "Second", 아니면 "First"
    if G == 0:
        print("Second")
    else:
        print("First")


# 메인에서 바로 실행을 원하시면 아래처럼 두시면 됩니다.
# 백준 등에 제출 시에는 solve() 함수를 그대로 제출 형식에 맞추면 됩니다.
if __name__ == "__main__":
    solve()
