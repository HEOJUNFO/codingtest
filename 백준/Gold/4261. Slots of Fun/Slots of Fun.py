import sys
import math

def is_equilateral(p1, p2, p3):
    # p1, p2, p3 are tuples of (r, c)
    # We have a triangular lattice arranged as follows:
    # Coordinates:
    #   x = c - r/2
    #   y = (sqrt(3)/2) * r
    # We want to check if the triangle formed by these three points is equilateral.
    #
    # To avoid floating point issues, we will compare squared distances using a rational representation:
    # Let dx = (2c1 - r1) - (2c2 - r2), and dy component as (r1 - r2).
    # Distance squared * 4 = dx^2 + 3*(r1 - r2)^2
    #
    # If all three distances squared are equal, the triangle is equilateral.
    
    def dist_squared_times_4(a, b):
        r1, c1 = a
        r2, c2 = b
        dx = (2*c1 - r1) - (2*c2 - r2)
        dy = (r1 - r2)
        return dx*dx + 3*dy*dy

    d12 = dist_squared_times_4(p1, p2)
    d23 = dist_squared_times_4(p2, p3)
    d31 = dist_squared_times_4(p3, p1)

    return d12 == d23 == d31 and d12 != 0

def solve():
    input_lines = sys.stdin.read().strip().split('\n')
    idx = 0
    while True:
        if idx >= len(input_lines):
            break
        n = input_lines[idx].strip()
        idx += 1
        if n == '0':
            break
        n = int(n)
        line = input_lines[idx].strip()
        idx += 1

        # Read the display configuration
        # There are n(n+1)/2 letters
        letters = line
        # Store positions of each letter
        # Positions in triangular form: row r: elements from sum of i=1..(r-1) to that plus r
        # Let's store them in a 2D manner for convenience:
        
        # We'll have at most 3 occurrences of each letter.
        # So we can store positions in a dictionary: letter -> list of (r, c) positions
        letter_positions = {}
        pos = 0
        for r in range(n):
            for c in range(r+1):
                ch = letters[pos]
                pos += 1
                if ch not in letter_positions:
                    letter_positions[ch] = []
                letter_positions[ch].append((r, c))

        # For each letter, if it occurs exactly 3 times, check if they form an equilateral triangle.
        # If it occurs less than 3 times, ignore. More than 3 times is not possible by problem statement.
        winning_letters = []
        for ch, pts in letter_positions.items():
            if len(pts) == 3:
                if is_equilateral(pts[0], pts[1], pts[2]):
                    winning_letters.append(ch)

        if len(winning_letters) == 0:
            print("LOOOOOOOOSER!")
        else:
            winning_letters = sorted(set(winning_letters))
            print("".join(winning_letters))

if __name__ == '__main__':
    solve()
