def solution(a):
    MOD = 10**7 + 19
    n = len(a)
    m = len(a[0])

    # 각 열별 1의 개수 r_i 계산
    r = [sum(a[row][col] for row in range(n)) for col in range(m)]

    # 조합수 계산
    # C(n,k) 테이블 미리 계산
    maxN = n
    C = [[0]*(maxN+1) for _ in range(maxN+1)]
    for i in range(maxN+1):
        C[i][0] = 1
    for i in range(1,maxN+1):
        for j in range(1,i+1):
            C[i][j] = (C[i-1][j-1] + C[i-1][j]) % MOD

    # dp[i][k]: i개 열 처리 후 홀수인 행의 수 = k 인 경우의 수
    # 마지막 열은 parity fix용이므로 m-1개 열까지만 dp
    dp = [[0]*(n+1) for _ in range(m)]
    dp[0][0] = 1  # 아직 열을 하나도 안 골랐을 때, 홀수인 행은 0개

    # 열을 1부터 m-1까지 처리
    for i in range(1, m):  # i번째 열(1-based), dp는 i열까지 처리후 상태
        # i=1일 때는 첫 번째 열 처리 결과를 dp[1]에 저장
        rj = r[i-1]  # 실제로는 i-1 인덱스열을 처리
        # 이유: dp[0]은 0개 열 처리, dp[1]은 1번째 열 처리완료 상태
        # 따라서 dp[i]는 i번째 열 처리 후 상태이므로 r 인덱스는 i-1

        prev = dp[i-1]
        curr = dp[i]
        for k_old in range(n+1):
            if prev[k_old] == 0:
                continue
            val = prev[k_old]
            # r_j개 선택 시
            # x개는 홀수 행에서 선택(뒤집어 홀수->짝수), r_j - x개는 짝수 행에서 선택(짝수->홀수)
            # k_new = k_old + r_j - 2x
            # 범위: x는 0 <= x <= r_j
            # 또한 x ≤ k_old (홀수행 중에서 x개 골라야 하므로), (r_j - x) ≤ (n - k_old)
            start_x = max(0, rj-(n-k_old))   # r_j-x ≤ n-k_old => x≥r_j-(n-k_old)
            end_x = min(rj, k_old)           # x ≤ k_old
            for x in range(start_x, end_x+1):
                k_new = k_old + rj - 2*x
                if 0 <= k_new <= n:
                    ways = (C[k_old][x]*C[n-k_old][rj - x]) % MOD
                    curr[k_new] = (curr[k_new] + val*ways) % MOD

    # 이제 m-1개 열 처리 후 dp[m-1][..] 상태에서 마지막 열(r_m)을 맞춰야 함
    # 마지막 열로 parity를 fix하려면 dp[m-1][r_m] 값이 최종 답이 된다.
    # 이유: m-1개 열 처리 후 홀수 행의 수가 r_m개라면, 마지막 열은 그 r_m개 홀수 행에 1을 주어
    # 모든 행을 짝수 패리티로 맞출 수 있고, 그 방법은 유일하므로 dp[m-1][r_m]이 곧 정답.

    return dp[m-1][r[m-1]] % MOD
