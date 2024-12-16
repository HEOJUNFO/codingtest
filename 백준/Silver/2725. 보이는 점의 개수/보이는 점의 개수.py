import sys

input = sys.stdin.readline

# 최대 범위
MAXN = 1000

# 뫼비우스 함수 계산
# μ(d) 계산 방법:
# 1) mu(1) = 1
# 2) 나머지는 소인수 분해를 통해 결정하나, 에라토스테네스 방식으로도 가능
# prime 리스트를 이용해 매 d에 대해:
# - 처음에 mu배열을 모두 1로 초기화
# - 에라토스테네스 식으로 각 소수 p에 대해 p가 나누어 떨어지는 배수에 대해 mu를 -1로 곱해준다.
# - 만약 p^2가 나누어 떨어지면 그 배수는 mu=0
def compute_mu(n):
    mu = [1]*(n+1)
    is_prime = [True]*(n+1)
    is_prime[0] = False
    is_prime[1] = False

    for i in range(2, n+1):
        if is_prime[i]:
            # i는 소수
            # i의 배수에 대해 mu값 업데이트
            for j in range(i, n+1, i):
                mu[j] = -mu[j]
                is_prime[j] = False
            is_prime[i] = False  # i에 대해서는 다시 True로 설정하지 않아도 됨

            # i^2의 배수에 대해서는 mu=0
            i2 = i*i
            for j in range(i2, n+1, i2):
                mu[j] = 0
    return mu

mu = compute_mu(MAXN)

C = int(input().strip())
for _ in range(C):
    N = int(input().strip())
    # sum over d: mu(d)*(floor(N/d))^2
    ans = 2  # 축 위의 점 (1,0), (0,1)
    for d in range(1, N+1):
        val = N//d
        ans += mu[d]*(val*val)
    print(ans)
