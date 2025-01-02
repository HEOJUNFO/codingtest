import sys
import random

# Miller-R test(밀러-라빈 소수 판별)을 사용하여 64비트 범위 내에서 정밀하게 소수 판별
# 2^64 미만 정수에 대해서는 특정한 몇 개의 밑(base)만 테스트해도 결정론적이다는 것이 알려져 있음
def is_prime(n):
    # 2 이하, 짝수에 대한 빠른 예외 처리
    if n < 2:
        return False
    if n in (2, 3):
        return True
    if n % 2 == 0:
        return False

    # 밀러-라빈을 위한 d, s 구하기(n-1 = 2^s * d 꼴)
    d = n - 1
    s = 0
    while d % 2 == 0:
        d >>= 1
        s += 1

    # 2^64 미만 정수에서 결정론적으로 사용되는 테스트 베이스
    # ( https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test 참고 )
    test_bases = [2, 325, 9375, 28178, 450775, 9780504, 1795265022]
    for a in test_bases:
        if a % n == 0:  # a와 n이 같다면 스킵
            return True
        if not miller_r_check(a, d, n, s):
            return False
    return True

def miller_r_check(a, d, n, s):
    # (a^d) % n 을 구함
    x = pow(a, d, n)
    if x == 1 or x == n - 1:
        return True
    # r을 1에서 s-1까지 변화시키며 (a^(2^r * d)) % n 이 n-1 인지 확인
    for _ in range(s - 1):
        x = (x * x) % n
        if x == n - 1:
            return True
    return False

# 최대한 빠르게 small prime으로 미리 나누어 보는 함수
def small_prime_factor(n, prime_list):
    factors = []
    for p in prime_list:
        if p * p > n:
            break
        while n % p == 0:
            factors.append(p)
            n //= p
    return factors, n

# Pollard's Rho 알고리즘
def pollard_rho(n):
    if n % 2 == 0:
        return 2
    # 알고리즘에 사용할 랜덤한 c값 선택
    c = random.randint(1, n-1)
    # 시작값 x, y는 랜덤하게 두어도 되지만 간단히 2로 통일
    x = 2
    y = 2
    d = 1

    # gcd가 1이 아닐 때까지 반복
    while d == 1:
        x = (x*x + c) % n
        y = (y*y + c) % n
        y = (y*y + c) % n
        d = gcd(abs(x - y), n)
        # 혹시나 주기가 잘못 맞아서 n으로 떨어지면 c를 바꿔서 다시 시도
        if d == n:
            return pollard_rho(n)
    return d

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

# 재귀적으로 인수분해 (Pollard's Rho + Miller-R)
def factor(n, result):
    # 만약 n이 1 이하라면 종료
    if n < 2:
        return
    # n이 소수이면 리스트에 추가 후 종료
    if is_prime(n):
        result.append(n)
        return

    # Rho 알고리즘으로 인수를 찾는다
    divisor = pollard_rho(n)
    # 찾은 인수를 재귀적으로 분해
    factor(divisor, result)
    factor(n // divisor, result)

def main():
    # 입력 받기
    input_data = sys.stdin.readline().strip()
    n = int(input_data)

    # 미리 자주 나오는 작은 소수들을 준비(에라토스테네스 등으로 미리 구해둬도 됨)
    # 여기서는 10만 이하 소수 중 일부 사용 가능
    # 하지만 범용 데모용으로 1,000 정도만 구성
    # (시간과 메모리를 고려해 적절히 세팅하세요)
    small_primes = sieve_small_primes(100000)

    # 1) 작은 소수로 나누기
    small_factors, remainder = small_prime_factor(n, small_primes)

    # 2) remainder가 1이 아니면 Pollard's Rho로 나머지 인수를 분해
    result = []
    if remainder > 1:
        factor(remainder, result)

    # 전체 factor는 small_factors + result
    # 오름차순 정렬
    final_factors = small_factors + result
    final_factors.sort()

    # 결과 출력
    for f in final_factors:
        print(f)

def sieve_small_primes(limit):
    """ limit 이하의 소수를 리스트로 반환하는 간단한 에라토스테네스 체 """
    sieve = [True]*(limit+1)
    sieve[0], sieve[1] = False, False

    for i in range(2, int(limit**0.5)+1):
        if sieve[i]:
            for j in range(i*i, limit+1, i):
                sieve[j] = False
    return [p for p in range(2, limit+1) if sieve[p]]

if __name__ == "__main__":
    main()
