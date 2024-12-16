import math

def solve():
    import sys
    input=sys.stdin.readline
    N,M=map(int,input().split())
    
    # We want to calculate the probability that in a random permutation of N,
    # the maximum cycle length is at most M.
    #
    # Known approach:
    # Count the number of permutations of {1,...,N} that have no cycle longer than M.
    # Let dp[k] = number of permutations of k elements with all cycle lengths ≤ M.
    # We have dp[0] = 1 (the empty permutation).
    #
    # To build permutations of k elements with all cycles ≤ M:
    # Consider adding the k-th element. It must form a cycle with some subset of the previously arranged k-1 elements.
    # If we pick r elements out of the k (including the k-th one itself) to form a cycle of length r,
    # the number of ways is:
    #   Choose (r-1) elements out of the previous (k-1) to form a cycle with the k-th element.
    #   There are C(k-1, r-1) ways to choose these r-1 elements.
    #   The number of distinct r-cycles formed from these r chosen elements is (r-1)!.
    #
    # Thus, the number of ways to form a cycle of length r that includes element k is:
    #   C(k-1, r-1) * (r-1)!
    #
    # Then the rest (k-r) elements must form a permutation counted by dp[k-r].
    #
    # So dp[k] = sum_{r=1}^{min(M, k)} C(k-1, r-1)*(r-1)! * dp[k-r].
    #
    # Finally, the probability = dp[N] / N!
    #
    # We'll compute factorials once to handle combinations and factorial parts efficiently.
    # We'll do exact integer arithmetic and then convert to float at the end.
    
    fact = [1]*(N+1)
    for i in range(2,N+1):
        fact[i] = fact[i-1]*i
    
    # nCr function
    def nCr(n,r):
        return fact[n]//(fact[r]*fact[n-r]) if 0<=r<=n else 0
    
    dp = [0]*(N+1)
    dp[0] = 1
    
    # Precompute factorial for cycles: (r-1)!
    # Already in fact array, just use fact[r-1].
    for k in range(1, N+1):
        # dp[k] = sum_{r=1}^{min(M,k)} C(k-1, r-1)*(r-1)! * dp[k-r]
        #       = sum_{r=1}^{R} nCr(k-1, r-1)*fact[r-1]*dp[k-r]
        R = min(M,k)
        val = 0
        for r in range(1, R+1):
            val += nCr(k-1, r-1)*fact[r-1]*dp[k-r]
        dp[k] = val
    
    # Probability = dp[N]/N!
    prob = dp[N]/fact[N]
    
    print(f"{prob:.8f}")

if __name__ == "__main__":
    solve()
