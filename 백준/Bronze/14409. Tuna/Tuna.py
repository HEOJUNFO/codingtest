N = int(input().strip())
X = int(input().strip())

total_value = 0

for _ in range(N):
    # Read the first line with P1 and P2
    P1, P2 = map(int, input().strip().split())
    # Check the difference
    if abs(P1 - P2) <= X:
        # If difference <= X, take the higher of P1, P2
        total_value += max(P1, P2)
    else:
        # If difference > X, read the next line for P3
        P3 = int(input().strip())
        total_value += P3

print(total_value)