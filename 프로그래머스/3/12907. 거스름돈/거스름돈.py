def solution(n, money):
    MOD = 1_000_000_007
    dp = [0] * (n + 1)
    dp[0] = 1  # 0원을 만드는 방법은 아무 동전도 사용하지 않는 한 가지

    # 각 동전 단위에 대해 dp를 업데이트
    for coin in money:
        for i in range(coin, n + 1):
            dp[i] += dp[i - coin]
            dp[i] %= MOD

    return dp[n]
