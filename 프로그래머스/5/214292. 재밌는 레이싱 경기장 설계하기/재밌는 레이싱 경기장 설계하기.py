def solution(heights):
    heights.sort()
    n = len(heights)

    if n % 2 == 0:
        # 짝수 길이일 경우
        m = n // 2
        # D_i = a[m+i] - a[i]
        diffs = [heights[m+i] - heights[i] for i in range(m)]
        return min(diffs)
    else:
        # 홀수 길이일 경우
        m = (n - 1) // 2
        # D_i = a[m+i] - a[i]
        diffs = [heights[m+i] - heights[i] for i in range(m+1)]
        diffs.sort()
        # 두 번째로 작은 값 반환
        return diffs[1]