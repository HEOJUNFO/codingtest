N = int(input().strip())
count = 0

for _ in range(N):
    S = input().strip()
    # Check if '01' or 'OI' appears in the string
    if '01' in S or 'OI' in S:
        count += 1

print(count)
