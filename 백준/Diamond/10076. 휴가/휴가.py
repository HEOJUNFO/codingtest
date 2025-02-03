import sys
from itertools import combinations

def main():
    input = sys.stdin.readline
    # 입력: 도시의 개수 n, 시작 도시 start, 휴가일수 d
    n, start, d = map(int, input().split())
    attractions = list(map(int, input().split()))
    
    best = 0
    # 도시 번호는 0부터 n-1
    # 도시들을 집합 S로 고른다고 할 때, S에 대해 s(시작도시)를 방문 경로에 포함하지 않아도
    # s에서 출발하여 S의 최소, 최대를 포함하는 구간 [L, R]를 방문하는 경로로 처리할 수 있다.
    # (S가 연속하지 않더라도, 도시들은 직선 상에 있으므로 S의 "볼록 껍질"인 [L,R]를 방문하면 S의
    #  도시들을 필요시에 관광할 수 있다.)
    #
    # 도시 [L, R]를 방문하는 최소 이동일수는 상황에 따라 달라지는데,
    # - 만약 s가 [L,R] 밖에 있다면, s에서 가까운 쪽 끝까지 이동한 후 [L,R] 전체를 지나면 된다.
    # - s가 [L,R] 내부에 있다면, 양쪽 끝 중 가까운 쪽부터 방문한 후 반대쪽 끝까지 가면 된다.
    #
    # 구체적으로, L = min(S), R = max(S)라 할 때,
    # if s < L: travel = (R - L) + (L - s)
    # if s > R: travel = (R - L) + (s - R)
    # if s in [L,R]: travel = (R - L) + min(s - L, R - s)
    #
    # S에 포함된 도시들을 관광하는 데는 도시 하나당 1일씩 필요하므로 collection cost = |S|.
    # 따라서 전체 필요한 일수 total_days = travel + |S| 가 d 이하여야 한다.
    #
    # S로 얻을 수 있는 이익은 sum( attractions[i] for i in S ) 이다.
    #
    # 모든 S (공집합 제외, 2^n - 1) 에 대해 위 조건을 확인하여 최대 이익을 구한다.
    
    # n이 최대 20이므로 combinations를 사용하여 모든 부분집합(크기 1부터 n까지)을 열거한다.
    for k in range(1, n+1):
        # k개 도시를 고르는 모든 조합 (도시번호는 오름차순으로 정렬되어 나온다)
        for subset in combinations(range(n), k):
            # S의 구간: L, R
            L = subset[0]
            R = subset[-1]
            # 총 관광지 개수 합
            sum_val = sum(attractions[i] for i in subset)
            # s와 구간 [L,R]의 관계에 따른 이동일수 계산
            if start < L:
                travel = (R - L) + (L - start)
            elif start > R:
                travel = (R - L) + (start - R)
            else:
                travel = (R - L) + min(start - L, R - start)
            total_days = travel + k  # 이동일수 + 관광(방문)일수
            if total_days <= d:
                best = max(best, sum_val)
    print(best)
    
if __name__ == '__main__':
    main()
