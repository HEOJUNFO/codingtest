def solve():
    import sys
    input_data = sys.stdin.read().strip().split('\n')
    n = int(input_data[0])
    idx = 1
    for _ in range(n):
        cards = input_data[idx].split()
        idx += 1
        
        # Count the frequency of each rank
        rank_counts = {}
        for card in cards:
            rank = card[0]
            rank_counts[rank] = rank_counts.get(rank, 0) + 1
        
        # The strength is the maximum count
        strength = max(rank_counts.values())
        print(strength)


if __name__ == '__main__':
    solve()
