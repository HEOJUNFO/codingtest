from collections import deque

def solution(n, results):
    # 1. 그래프 초기화
    wins = [[] for _ in range(n+1)]   # wins[i] = i번 선수가 이긴 선수 리스트
    loses = [[] for _ in range(n+1)]  # loses[i] = i번 선수를 이긴 선수 리스트

    for winner, loser in results:
        # winner → loser
        wins[winner].append(loser)
        loses[loser].append(winner)

    def bfs(start, graph):
        """ 
        start 선수에서 시작해서 graph를 따라 이동할 수 있는 모든 노드(선수)를 찾는 함수
        graph가 wins이면 '이길 수 있는 선수들', loses이면 '이길 수 있는 선수들(역방향)'이 됨 
        """
        visited = [False] * (n+1)
        visited[start] = True
        queue = deque([start])
        count = 0

        while queue:
            current = queue.popleft()
            for nxt in graph[current]:
                if not visited[nxt]:
                    visited[nxt] = True
                    queue.append(nxt)
                    count += 1

        return count

    answer = 0

    # 2. 각 선수별로 (이길 수 있는 선수 수 + 이길 수 있는 선수 수(역방향)) 를 구해서
    #    (n - 1)이 되면 순위가 확실함.
    for i in range(1, n+1):
        win_count = bfs(i, wins)    # i가 이길 수 있는 선수 수
        lose_count = bfs(i, loses) # i를 이길 수 있는 선수 수
        if win_count + lose_count == n - 1:
            answer += 1

    return answer


# 테스트
if __name__ == "__main__":
    n = 5
    results = [[4,3], [4,2], [3,2], [1,2], [2,5]]
    print(solution(n, results))  # 예상 결과: 2
