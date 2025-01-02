from collections import deque

def solution(board):
    N = len(board)
    
    # 방문 체크 배열: visited[r][c][d]
    # d = 0 (가로), d = 1 (세로)
    visited = [[[False]*2 for _ in range(N)] for _ in range(N)]
    
    # 0-based로 시작 상태: (0,0) 가로 상태(d=0), 비용(시간)=0
    queue = deque()
    queue.append((0, 0, 0, 0))  # (r, c, direction, cost)
    visited[0][0][0] = True
    
    # 범위 및 빈칸 확인용 함수
    def is_free(r, c):
        if 0 <= r < N and 0 <= c < N and board[r][c] == 0:
            return True
        return False
    
    while queue:
        r, c, d, cost = queue.popleft()
        
        # 로봇이 차지하는 2칸 중 하나라도 (N-1, N-1)에 닿는지 확인
        if d == 0:  # 가로
            # (r,c)와 (r,c+1)
            if (r == N-1 and c == N-1) or (r == N-1 and c+1 == N-1):
                return cost
        else:       # 세로
            # (r,c)와 (r+1,c)
            if (r == N-1 and c == N-1) or (r+1 == N-1 and c == N-1):
                return cost
        
        # (A) 상하좌우 이동
        if d == 0:  # 가로로 누워있을 때
            # 위
            if r-1 >= 0 and is_free(r-1, c) and is_free(r-1, c+1):
                if not visited[r-1][c][0]:
                    visited[r-1][c][0] = True
                    queue.append((r-1, c, 0, cost+1))
            # 아래
            if r+1 < N and is_free(r+1, c) and is_free(r+1, c+1):
                if not visited[r+1][c][0]:
                    visited[r+1][c][0] = True
                    queue.append((r+1, c, 0, cost+1))
            # 왼
            if c-1 >= 0 and is_free(r, c-1):
                # 새 위치는 (r, c-1)에서 가로 방향
                if not visited[r][c-1][0]:
                    visited[r][c-1][0] = True
                    queue.append((r, c-1, 0, cost+1))
            # 오른쪽
            if c+2 < N and is_free(r, c+2):
                # 새 위치는 (r, c+1)에서 가로 방향
                if not visited[r][c+1][0]:
                    visited[r][c+1][0] = True
                    queue.append((r, c+1, 0, cost+1))
            
            # (B) 회전
            # 가로->세로 (위/아래)
            
            # 아래쪽 회전 (로봇 아래 공간 필요)
            # 1) 왼쪽 칸 (r,c)를 축
            #    회전 후: (r,c) & (r+1,c)
            #    대각: (r+1,c+1)도 비어있어야 한다고 문제에서 명시
            if r+1 < N and is_free(r+1, c) and is_free(r+1, c+1):
                # 이제 세로 방향이므로 d=1
                if not visited[r][c][1]:
                    visited[r][c][1] = True
                    queue.append((r, c, 1, cost+1))
            
            # 2) 오른쪽 칸 (r,c+1)을 축
            #    회전 후: (r,c+1) & (r+1,c+1)
            #    대각: (r+1,c)도 비어있어야 함
            if r+1 < N and is_free(r+1, c+1) and is_free(r+1, c):
                if not visited[r][c+1][1]:
                    visited[r][c+1][1] = True
                    queue.append((r, c+1, 1, cost+1))
            
            # 위쪽 회전 (로봇 위 공간 필요)
            # 1) 왼쪽 칸 (r,c)를 축
            #    회전 후: (r-1,c) & (r,c)
            #    대각: (r-1,c+1)
            if r-1 >= 0 and is_free(r-1, c) and is_free(r-1, c+1):
                if not visited[r-1][c][1]:
                    visited[r-1][c][1] = True
                    queue.append((r-1, c, 1, cost+1))
            
            # 2) 오른쪽 칸 (r,c+1)을 축
            #    회전 후: (r-1,c+1) & (r,c+1)
            #    대각: (r-1,c)
            if r-1 >= 0 and is_free(r-1, c+1) and is_free(r-1, c):
                if not visited[r-1][c+1][1]:
                    visited[r-1][c+1][1] = True
                    queue.append((r-1, c+1, 1, cost+1))
        
        else:  # d == 1, 세로로 서 있을 때
            # 위
            if r-1 >= 0 and is_free(r-1, c):
                # 새 위치는 (r-1, c)에서 세로 방향
                if not visited[r-1][c][1]:
                    visited[r-1][c][1] = True
                    queue.append((r-1, c, 1, cost+1))
            # 아래
            if r+2 < N and is_free(r+2, c):
                # 새 위치는 (r+1, c)에서 세로 방향
                if not visited[r+1][c][1]:
                    visited[r+1][c][1] = True
                    queue.append((r+1, c, 1, cost+1))
            # 왼
            if c-1 >= 0 and is_free(r, c-1) and is_free(r+1, c-1):
                if not visited[r][c-1][1]:
                    visited[r][c-1][1] = True
                    queue.append((r, c-1, 1, cost+1))
            # 오른쪽
            if c+1 < N and is_free(r, c+1) and is_free(r+1, c+1):
                if not visited[r][c+1][1]:
                    visited[r][c+1][1] = True
                    queue.append((r, c+1, 1, cost+1))
            
            # (B) 회전 (세로->가로)
            # 왼쪽/오른쪽 회전
            
            # 왼쪽 회전
            # 1) 위 칸 (r,c)를 축
            #    회전 후: (r,c) & (r,c-1)
            #    대각: (r+1,c-1)
            if c-1 >= 0 and is_free(r, c-1) and is_free(r+1, c-1):
                if not visited[r][c-1][0]:
                    visited[r][c-1][0] = True
                    queue.append((r, c-1, 0, cost+1))
            # 2) 아래 칸 (r+1,c)를 축
            #    회전 후: (r+1,c) & (r+1,c-1)
            #    대각: (r,c-1)
            if c-1 >= 0 and is_free(r+1, c-1) and is_free(r, c-1):
                if not visited[r+1][c-1][0]:
                    visited[r+1][c-1][0] = True
                    queue.append((r+1, c-1, 0, cost+1))
            
            # 오른쪽 회전
            # 1) 위 칸 (r,c)를 축
            #    회전 후: (r,c) & (r,c+1)
            #    대각: (r+1,c+1)
            if c+1 < N and is_free(r, c+1) and is_free(r+1, c+1):
                if not visited[r][c][0]:
                    visited[r][c][0] = True
                    queue.append((r, c, 0, cost+1))
            # 2) 아래 칸 (r+1,c)를 축
            #    회전 후: (r+1,c) & (r+1,c+1)
            #    대각: (r,c+1)
            if c+1 < N and is_free(r+1, c+1) and is_free(r, c+1):
                if not visited[r+1][c][0]:
                    visited[r+1][c][0] = True
                    queue.append((r+1, c, 0, cost+1))
    
    # 문제 조건 상 "항상 도달 가능"이라고 했으므로 여기 오기 전 리턴되겠지만,
    # 혹시 모르니 기본값을 두자.
    return -1
