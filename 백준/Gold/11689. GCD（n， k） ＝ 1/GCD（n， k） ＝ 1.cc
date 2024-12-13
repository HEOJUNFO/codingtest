#include <bits/stdc++.h>
using namespace std;

/*
 * We want to find the count of integers k in the range [1, n]
 * such that GCD(n, k) = 1. This count is given by Euler's Tot function φ(n).
 *
 * Euler's Tot function φ(n) can be computed as:
 *   φ(n) = n * Π (1 - 1/p) over all prime factors p of n.
 *
 * For n ≤ 10^12, we can factor n by trial division up to sqrt(n) which will be at most 10^6.
 * This will be efficient enough given the problem constraints.
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    if (n == 1) {
        cout << 1 << "\n";
        return 0;
    }

    // Factorization using trial division
    long long phi = n;
    long long temp = n;

    // Check divisibility by small primes (2 first)
    if (temp % 2 == 0) {
        phi = phi - phi / 2;
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }

    // Check odd factors
    for (long long i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            phi = phi - phi / i;
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }

    // If temp > 1 here, then temp is a prime factor
    if (temp > 1) {
        phi = phi - phi / temp;
    }

    cout << phi << "\n";
    return 0;
}
