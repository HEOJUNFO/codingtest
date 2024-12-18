def solution(strs, t):
    # 단어 조각들을 빠르게 확인하기 위해 set으로 변환
    word_set = set(strs)
    n = len(t)
    
    # dp[i]: t[:i]를 만드는 데 필요한 최소 단어 조각 수
    # 인덱스 편의를 위해 dp 길이를 n+1로 잡고,
    # dp[0] = 0(빈 문자열), dp[i]는 t[:i]를 만드는 최소 개수
    dp = [float('inf')] * (n+1)
    dp[0] = 0
    
    for i in range(1, n+1):
        # 최대 5글자 길이의 조각만 확인
        for length in range(1, 6):
            if i - length < 0:
                break
            # t[i-length : i]가 단어 조각으로 가능한지 확인
            if t[i-length:i] in word_set:
                dp[i] = min(dp[i], dp[i-length] + 1)
                
    return dp[n] if dp[n] != float('inf') else -1


# 동작 예시
print(solution(["ba","na","n","a"], "banana")) # 3
print(solution(["app","ap","p","l","e","ple","pp"], "apple")) # 2
print(solution(["ba","an","nan","ban","n"], "banana")) # -1

