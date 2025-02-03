import sys

def query(freq, k):
    # freq: frequency array for attraction values (0~100)
    # k: 선택해야 할 도시의 개수
    total = 0
    # 높은 관광지 개수부터 k개를 선택
    for val in range(100, -1, -1):
        if k <= 0:
            break
        if freq[val] <= k:
            total += freq[val] * val
            k -= freq[val]
        else:
            total += k * val
            k = 0
    return total

def main():
    input = sys.stdin.readline
    n, start, d = map(int, input().split())
    # 서브태스크 2에서는 시작 도시가 항상 0임
    attractions = list(map(int, input().split()))
    
    # R: furthest 방문 도시 (최소 비용을 위해 실제로 방문한 도시여야 함)
    R_max = min(n - 1, d)  # R일의 이동비용이 d를 초과하면 안 됨
    
    # prefix[i] = attractions[0] + attractions[1] + ... + attractions[i]
    prefix = [0] * (R_max + 1)
    prefix[0] = attractions[0]
    for i in range(1, R_max + 1):
        prefix[i] = prefix[i - 1] + attractions[i]
    
    ans = 0
    # 0~R-1 도시들에 대한 frequency distribution (관광지 개수: 0~100)
    freq = [0] * 101

    # R = 0일 때 처리:
    # 방문 가능 일수 = d - 0 = d. d>=1이므로 전체 도시(단 0번 도시) 방문
    if d >= 1:
        ans = attractions[0]
    
    # R=0 이미 처리했으므로 freq 업데이트
    freq[attractions[0]] += 1

    # R=1부터 R_max까지 반복
    for R in range(1, R_max + 1):
        visiting_days = d - R  # 이동비용 R일 + 방문일수 = d
        if visiting_days <= 0:
            # 방문할 날이 없으면 건너뜀
            pass
        else:
            if visiting_days >= R + 1:
                # 0번부터 R번 도시 모두 방문 가능
                candidate = prefix[R]
            else:
                # 반드시 R번 도시는 방문해야 하므로 남은 (visiting_days - 1)일에 대해
                # 0~R-1 도시들 중 관광지 합이 최대가 되도록 선택
                k = visiting_days - 1
                candidate = attractions[R] + query(freq, k)
            if candidate > ans:
                ans = candidate
        # 다음 R에 대비하여 현재 도시 R를 frequency에 추가
        freq[attractions[R]] += 1

    print(ans)

if __name__ == '__main__':
    main()
