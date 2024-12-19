import sys
input = sys.stdin.readline

def main():
    N = int(input().strip())
    G = [int(input().strip()) for _ in range(N)]

    # Check feasibility using a known theorem for degree sequences of simple graphs
    # But since we also need to construct one, let's use a constructive approach
    # We'll use a greedy approach (Havel-Hakimi like) to construct the matches if possible.

    # Quick checks:
    # 1. Sum of all degrees must be even (because each edge contributes 2 to the sum).
    # 2. No player wants more games than N-1 (given by problem constraint).
    # 3. If sum of degrees is even and no player wants more than N-1 matches, we still need to try constructing.

    total_matches = sum(G)
    if total_matches % 2 != 0: 
        print("NO SCHEDULE")
        return

    # We'll try a greedy approach:
    # Sort players by their desired matches in descending order and try to assign matches.
    # Use a priority queue (max-heap) to always match the player with the highest remaining demand first.
    import heapq
    pq = []
    for i in range(N):
        if G[i] > 0:
            # Push (-G[i], i) because heapq in Python is a min-heap
            heapq.heappush(pq, (-G[i], i))

    # We will store the opponents for each player
    opponents = [[] for _ in range(N)]

    # Greedy construction:
    while pq:
        # Pop the player with the largest degree needed
        deg, player = heapq.heappop(pq)
        deg = -deg

        if deg == 0:
            # No more matches needed for this player
            continue

        # Now we need to find deg other players from the heap to match with
        if len(pq) < deg:
            # Not enough players to satisfy this player's demand
            print("NO SCHEDULE")
            return

        temp = []
        for _ in range(deg):
            # Pop the next largest available player to match
            nd, np = heapq.heappop(pq)
            nd = -nd
            if np == player:
                # Can't match with oneself (shouldn't happen if constraints are followed)
                print("NO SCHEDULE")
                return
            # Record the match
            opponents[player].append(np+1)
            opponents[np].append(player+1)
            # Decrease their needed match count
            nd -= 1
            if nd < 0:
                # That means they needed fewer matches than we tried to assign
                print("NO SCHEDULE")
                return
            if nd > 0:
                temp.append((-nd, np))

        # We have assigned all matches for 'player', no need to push 'player' back since deg is now 0.
        # Push back the updated remaining demands
        for item in temp:
            heapq.heappush(pq, item)

    # If we reach here, we have a valid schedule
    print("SCHEDULE")
    for opp_list in opponents:
        opp_list.sort()
        print(' '.join(map(str, opp_list)) if opp_list else '')

if __name__ == '__main__':
    main()
