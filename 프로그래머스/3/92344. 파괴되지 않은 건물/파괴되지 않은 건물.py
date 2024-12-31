def solution(board, skill):
    N = len(board)
    M = len(board[0])
    
    # 1. diff 배열 준비 (N+1) x (M+1)
    diff = [[0] * (M+1) for _ in range(N+1)]
    
    # 2. skill 정보를 diff 배열에 기록
    for t, r1, c1, r2, c2, degree in skill:
        if t == 1:  # 적의 공격
            d = -degree
        else:       # 아군의 회복
            d = degree
        
        diff[r1][c1]     += d
        diff[r1][c2+1]   -= d
        diff[r2+1][c1]   -= d
        diff[r2+1][c2+1] += d
    
    # 3. diff 배열에 대해 2차원 누적합(가로 → 세로 순서로 예시)
    # 가로 방향 누적합
    for i in range(N+1):
        for j in range(1, M+1):
            diff[i][j] += diff[i][j-1]
    
    # 세로 방향 누적합
    for j in range(M+1):
        for i in range(1, N+1):
            diff[i][j] += diff[i-1][j]
    
    # 4. 최종 board에 반영 & 내구도 1 이상인 건물 세기
    answer = 0
    for i in range(N):
        for j in range(M):
            if board[i][j] + diff[i][j] > 0:
                answer += 1
    
    return answer
