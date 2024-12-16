from collections import deque

def solution(game_board, table):
    n = len(game_board)
    
    # 방향 벡터 (상, 하, 좌, 우)
    directions = [(-1,0),(1,0),(0,-1),(0,1)]
    
    def get_clusters(board, target):
        """ 
        board에서 target 값(0 또는 1)을 갖는 칸들로 이루어진 연결 요소(클러스터)들을 추출.
        각 클러스터는 상대 좌표 형태의 집합으로 반환.
        """
        visited = [[False]*n for _ in range(n)]
        clusters = []
        
        for i in range(n):
            for j in range(n):
                if board[i][j] == target and not visited[i][j]:
                    # BFS/DFS로 클러스터 추출
                    queue = deque([(i,j)])
                    visited[i][j] = True
                    cluster = []
                    
                    while queue:
                        x,y = queue.popleft()
                        cluster.append((x,y))
                        for dx,dy in directions:
                            nx, ny = x+dx, y+dy
                            if 0 <= nx < n and 0 <= ny < n and not visited[nx][ny] and board[nx][ny] == target:
                                visited[nx][ny] = True
                                queue.append((nx,ny))
                    
                    # 클러스터 정규화: 좌표를 (min_x,min_y)를 기준으로 0,0 시작 형태로 만들기
                    cluster.sort()
                    min_x = cluster[0][0]
                    min_y = min([c[1] for c in cluster])
                    normalized = [(c[0]-min_x, c[1]-min_y) for c in cluster]
                    normalized.sort()
                    clusters.append(normalized)
        return clusters
    
    # 구멍(빈칸, 0으로 이루어진 연결 요소) 추출
    holes = get_clusters(game_board, 0)
    
    # 테이블에서 퍼즐 조각(1으로 이루어진 연결 요소) 추출
    pieces = get_clusters(table, 1)
    
    # 퍼즐 조각 회전 함수
    def rotate(piece):
        # piece는 (x,y) 좌표 리스트
        # 90도 회전: (x,y) -> (y, -x)
        # 다만 회전 후에도 정규화 필요
        rotated = [(y, -x) for (x,y) in piece]
        rotated.sort()
        min_x = rotated[0][0]
        min_y = min([p[1] for p in rotated])
        normalized = [(p[0]-min_x, p[1]-min_y) for p in rotated]
        normalized.sort()
        return normalized
    
    # 각 퍼즐 조각에 대해 4가지 회전 형태를 미리 구해둔다.
    rotated_pieces = []
    for piece in pieces:
        r90 = rotate(piece)
        r180 = rotate(r90)
        r270 = rotate(r180)
        all_forms = [piece, r90, r180, r270]
        # 중복 제거(혹시 회전했는데 같은 형태가 나올 수 있으므로 set으로 처리)
        # set에 넣기 위해 tuple 변환
        unique_forms = set(tuple(form) for form in all_forms)
        # 다시 list로 변환
        unique_forms = [list(t) for t in unique_forms]
        rotated_pieces.append(unique_forms)
    
    used = [False]*len(rotated_pieces)
    answer = 0
    
    # 구멍 하나당 사용 가능한 퍼즐 조각 탐색
    for hole in holes:
        for i in range(len(rotated_pieces)):
            if used[i]:
                continue
            # 현재 조각의 모든 회전 형태 중 하나라도 hole과 같으면 매칭
            match = False
            for form in rotated_pieces[i]:
                if form == hole:
                    match = True
                    break
            if match:
                used[i] = True
                answer += len(hole)  # 채운 칸 수만큼 추가
                break
    
    return answer
