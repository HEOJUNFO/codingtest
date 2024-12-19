def total_blocks(n):
    # The total number of blocks needed for a pyramid with base n
    # is the sum of the first n positive integers: n(n+1)/2.
    return n * (n + 1) // 2

def main():
    while True:
        n = int(input().strip())
        if n == 0:
            break
        print(total_blocks(n))

if __name__ == "__main__":
    main()
