def longest_increasing_subsequence_length(heights):
    n = len(heights)
    dp = [1] * n  # dp[i]는 i번째 원소를 마지막 원소로 하는 LIS의 길이
    
    for i in range(n):
        for j in range(i):
            if heights[j] < heights[i]:
                dp[i] = max(dp[i], dp[j] + 1)
    
    return max(dp)

def solve():
    import sys
    input_data = sys.stdin.read().strip().split()
    T = int(input_data[0])
    index = 1
    for t in range(1, T + 1):
        N = int(input_data[index]); index += 1
        heights = list(map(int, input_data[index:index+N]))
        index += N
        
        lis_length = longest_increasing_subsequence_length(heights)
        # 최소 파괴 집 수 = 총 집 수 - LIS 길이
        result = N - lis_length
        
        print(f"Case #{t}: {result}")

if __name__ == "__main__":
    solve()
