import sys

def sieve(n):
    """ Sieve of Eratosthenes to generate primes up to n """
    sieve = [True]*(n+1)
    sieve[0], sieve[1] = False, False
    for i in range(2, int(n**0.5)+1):
        if sieve[i]:
            for j in range(i*i, n+1, i):
                sieve[j] = False
    return [x for x in range(2, n+1) if sieve[x]]

def modInverse(a, p):
    """ Compute modular inverse of a mod p using Fermat's little theorem.
        p is prime, so inverse = a^(p-2) mod p """
    return pow(a, p-2, p)

def solve_case(D, K, seq, primes):
    # If K=1, cannot determine next number uniquely
    if K == 1:
        return "I don't know."

    # Check if all numbers are the same
    all_same = all(x == seq[0] for x in seq)
    if all_same:
        # Next number is always seq[0]
        return seq[0]

    # Find first pair of consecutive elements that differ
    idx = None
    for i in range(K-1):
        if seq[i] != seq[i+1]:
            idx = i
            break

    # If no differing pair found (but we already checked all_same = False),
    # This means the sequence must have at least one difference.
    # If not found, it's an edge case, but logically it won't happen since not all same and we must have a difference.
    if idx is None:
        # Just a safety fallback (should not reach here)
        return "I don't know."

    s0 = seq[idx]
    s1 = seq[idx+1]

    # We'll try to determine A and B and P.
    # If we have at least one more element s2 = seq[idx+2], we can use it.
    # But even if not, we can still guess from what we have, by trying all primes.
    # The formula for A (if s1 != s0):
    # s1 = A*s0 + B (mod P)
    # s2 = A*s1 + B (mod P)
    # => s2 - s1 = A*(s1 - s0) (mod P)
    # If we have s2:
    if idx+2 < K:
        s2 = seq[idx+2]
        need_s2 = True
    else:
        s2 = None
        need_s2 = False

    max_elem = max(seq)
    limit = 10**D

    possible_next = set()
    # If s1 == s0, handle separately:
    # If s1 == s0, from s1 = A*s0 + B => B = s1 - A*s0
    # If we have s2, then s2 = A*s1 + B. If s2 != s1 and s1=s0, that gives constraints.
    # But let's handle general case first:
    if s0 == s1:
        # Then from s1 = A*s0 + B => B = s0 - A*s0 = s0*(1 - A) mod P
        # For any chosen P and A, we can find B. We must check entire sequence consistency.
        # If need_s2 and s2 != s1: 
        # s2 = A*s1 + B = A*s0 + B must = s2
        # This would restrict solutions heavily.
        # We'll just brute force over primes and attempt a consistent solution.
        pass
    else:
        # General case: (s2 - s1) = A*(s1 - s0) mod P
        # If we don't have s2, we only have one equation per prime. We must rely on checking the entire seq for uniqueness.
        pass

    # We'll try each prime candidate P > max_elem and ≤ 10^D
    # For large D, this might be slow, but let's trust that this is acceptable here.

    for P in primes:
        if P <= max_elem:  # P must be greater than any sequence element
            continue
        if P > limit:
            break

        if s0 == s1:
            # Then for the entire sequence to hold, if we never got a difference,
            # we would have handled this in all_same. Since we found a difference in the sequence:
            # Let j be the first difference index we found:
            # Suppose seq[j] = s0 and seq[j+1] != s0
            # Then from eq: s_{j+1} = A*seq[j] + B = A*s0 + B
            # If s0 = s1 = ... = seq[j], but seq[j+1] differ, we can try:
            # We know s_{j+1} ≠ s0, so from s_{j+1} = A*s0 + B:
            # That gives one linear equation in terms of A and B.
            # Without a second equation, we must rely on the rest of sequence to pin down A and B.
            
            # Let's build linear equations using consecutive pairs for the whole sequence and see if we can solve for A,B:
            # We'll pick two pairs where the difference is visible:
            found_second_pair = False
            A_candidates = []
            # We'll try to solve from any two distinct consecutive pairs that differ:
            # We have s_i+1 = A*s_i + B
            # We'll form system from two distinct pairs where s_i != s_{i+1} or that could give us a unique solution.
            # Actually if s0=s1 but we found a difference at idx: s0... s0 ... s_diff
            # We'll find another pair (maybe idx+1) if possible.
            
            # Strategy:
            # Collect all pairs (x,y) from seq as y = A*x + B mod P.
            # If we can find at least two pairs with distinct x to solve for A and B:
            pairs = []
            for i in range(K-1):
                pairs.append((seq[i], seq[i+1]))

            # Try to solve from any two pairs with different x:
            # (x1, y1), (x2, y2):
            # y1 = A*x1 + B
            # y2 = A*x2 + B
            # Subtract: (y2 - y1) = A*(x2 - x1)
            # If x1 != x2 and gcd(x2-x1, P)=1, we can solve A:
            
            # We'll try all pairs to find a suitable solution:
            found_any = False
            A_set = set()
            B_set = set()
            for i in range(len(pairs)):
                for j in range(i+1, len(pairs)):
                    x1, y1 = pairs[i]
                    x2, y2 = pairs[j]
                    if x1 != x2:
                        diff_x = (x2 - x1) % P
                        diff_y = (y2 - y1) % P
                        if diff_x % P != 0:
                            # Inverse exists since P is prime and diff_x != 0
                            inv_x = modInverse(diff_x, P)
                            A_candidate = (diff_y * inv_x) % P
                            B_candidate = (y1 - A_candidate*x1) % P
                            # Check entire sequence:
                            consistent = True
                            for k in range(K-1):
                                if (A_candidate*seq[k] + B_candidate) % P != seq[k+1]:
                                    consistent = False
                                    break
                            if consistent:
                                A_set.add(A_candidate)
                                B_set.add(B_candidate)
                                found_any = True
            if not found_any:
                # Maybe the sequence doesn't provide enough info:
                # If all pairs have same x (?), or no difference found that gives a solution:
                # It's safer to say "I don't know."
                continue
            
            # If we found multiple (A,B) pairs for this P, next is always computed by one of them:
            next_numbers = set()
            for A_candidate in A_set:
                for B_candidate in B_set:
                    nxt = (A_candidate * seq[-1] + B_candidate) % P
                    next_numbers.add(nxt)
            # If multiple different next numbers for this P, we still keep them, 
            # because we must combine with other P results.
            for nxt in next_numbers:
                possible_next.add(nxt)

        else:
            # s1 != s0
            diff = (s1 - s0) % P
            if diff == 0:
                # This can't happen since s1 != s0, modulo P might cause equality if P divides (s1-s0).
                # If P divides (s1-s0), we cannot find A due to lack of inverse.
                continue
            inv_diff = modInverse(diff, P)
            # If we have s2:
            if need_s2:
                # A = (s2 - s1)*inv_diff mod P
                A = ((s2 - s1) % P) * inv_diff % P
                B = (s1 - A*s0) % P
                # Check entire sequence consistency:
                good = True
                for i in range(K-1):
                    if (A*seq[i] + B) % P != seq[i+1]:
                        good = False
                        break
                if good:
                    nxt = (A*seq[-1] + B) % P
                    possible_next.add(nxt)
            else:
                # We have only two points: s0,s1
                # We have s1 = A*s0 + B mod P
                # This is a single equation with two unknowns (A,B).
                # For any chosen A, B = s1 - A*s0.
                # Without s2, we must rely on the entire (just two elements) doesn't give us constraints.
                # Actually, with only two elements, many solutions will fit.
                # We must try to see if there's any additional pairs we can use.
                # If K=2 and we have no s2, we cannot uniquely determine A,B:
                # We'll try all A in [0,P-1], B = s1 - A*s0 mod P and check the rest? 
                # The rest is only these two elements. Any A,B works for these two numbers alone.
                # That means infinite solutions.
                # So we must say "I don't know." for this P.
                # But wait, we must be consistent. For each prime P, any A,B works that produce s1 from s0.
                # The next would differ depending on A,B chosen. So no unique next number from this prime.
                # Let's skip this P.
                continue

    if len(possible_next) == 0:
        return "I don't know."
    if len(possible_next) == 1:
        return possible_next.pop()
    # If multiple possibilities differ:
    return "I don't know."

def main():
    input = sys.stdin.read().strip().split()
    T = int(input[0])
    # Precompute primes up to 10^6
    primes = sieve(10**6)
    pos = 1
    for t in range(1, T+1):
        D, K = int(input[pos]), int(input[pos+1])
        pos += 2
        seq = list(map(int, input[pos:pos+K]))
        pos += K
        ans = solve_case(D, K, seq, primes)
        print(f"Case #{t}: {ans}")

if __name__ == "__main__":
    main()
