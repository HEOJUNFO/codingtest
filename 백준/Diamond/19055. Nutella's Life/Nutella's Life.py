import sys
sys.setrecursionlimit(300000)

NEG_INF = -10**19

# LiChaoTree for maximum query on lines f(x) = m*x + b
class LiChaoTree:
    __slots__ = ('l', 'r', 'm', 'line', 'left', 'right')
    def __init__(self, l, r):
        self.l = l
        self.r = r
        self.m = (l + r) >> 1
        # 초기에는 모든 x에서 NEG_INF를 반환하는 선을 둡니다.
        self.line = (0, NEG_INF)  # line: (slope, intercept)
        self.left = None
        self.right = None

    def add_line(self, new_line):
        l, r, m = self.l, self.r, self.m
        cur = self.line
        # mid에서 새로운 선이 더 좋으면 swap
        if new_line[0] * m + new_line[1] > cur[0] * m + cur[1]:
            self.line, new_line = new_line, self.line
        # 구간이 끝나면 종료
        if l == r:
            return
        # 왼쪽 구간 검사
        if new_line[0] * l + new_line[1] > self.line[0] * l + self.line[1]:
            if self.left is None:
                self.left = LiChaoTree(l, m)
            self.left.add_line(new_line)
        # 오른쪽 구간 검사
        elif new_line[0] * r + new_line[1] > self.line[0] * r + self.line[1]:
            if self.right is None:
                self.right = LiChaoTree(m+1, r)
            self.right.add_line(new_line)

    def query(self, x):
        res = self.line[0] * x + self.line[1]
        if self.l == self.r:
            return res
        if x <= self.m and self.left is not None:
            res = max(res, self.left.query(x))
        elif x > self.m and self.right is not None:
            res = max(res, self.right.query(x))
        return res

# Fenwick tree (Binary Indexed Tree) where each node stores a LiChaoTree.
class FenwTree:
    __slots__ = ('n', 'data')
    def __init__(self, n, xL, xR):
        self.n = n
        self.data = [LiChaoTree(xL, xR) for _ in range(n+1)]
        
    def update(self, i, line):
        # i는 1-indexed
        while i <= self.n:
            self.data[i].add_line(line)
            i += i & -i
            
    def query(self, i, x):
        res = NEG_INF
        while i:
            res = max(res, self.data[i].query(x))
            i -= i & -i
        return res

def main():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    it = iter(input_data)
    n = int(next(it))
    a = [int(next(it)) for _ in range(n)]
    
    # a_i 값 압축: Fenwick 트리의 인덱스로 사용 (1-indexed)
    uniq = sorted(set(a))
    comp = {v: i+1 for i, v in enumerate(uniq)}
    m = len(uniq)
    
    # LiChao 트리의 x 도메인은 1부터 n까지 (i 값)
    fenw = FenwTree(m, 1, n)
    
    # DP: dp[i]의 두 배값을 D[i] = 2*dp[i]로 두면,
    # 점화식은 다음과 같이 유도됩니다.
    # D[i] = 2*a_i - i^2 + i + max(0, max_{j < i, a_j <= a_i} { (2*dp[j] - j^2 - j) + 2*i*j })
    # 단, 여기서 i는 1-indexed 입니다.
    D_list = [0] * n
    bestD = NEG_INF  # 최종 답 후보(두 배한 값)
    
    for i in range(n):
        i_val = i + 1
        pos = comp[a[i]]
        # fenw에서 a_j <= a[i]인 모든 j에 대해, f(i) = (2*j)*i + (2*dp[j]-j^2-j) 의 최댓값을 구합니다.
        Q = fenw.query(pos, i_val)
        candidate = Q if Q > 0 else 0
        D = 2 * a[i] - i_val * i_val + i_val + candidate
        D_list[i] = D
        # 마지막으로, i 이후의 모든 대회를 skip할 때의 패널티는 (n-i_val)*(n-i_val+1) (두 배한 값)입니다.
        curCandidate = D - (n - i_val) * (n - i_val + 1)
        if curCandidate > bestD:
            bestD = curCandidate
        # 이번 대회 i를 선택했을 때의 보조값:
        # H(i) = 2*dp[i] - i^2 - i = D - i^2 - i   (i는 1-indexed)
        H = D - i_val * i_val - i_val
        # 새 선: f(x) = (2*i)*x + H, 여기서 x는 이후 대회 번호(i 값)
        new_line = (2 * i_val, H)
        fenw.update(pos, new_line)
    
    # 아무 대회도 참여하지 않는 경우의 fun: - (1 + 2 + ... + n) = - n*(n+1)/2,
    # 두 배한 값은 -n*(n+1)
    bestD = max(bestD, -n * (n + 1))
    # 최종 답: dp = D/2
    ans = bestD // 2
    sys.stdout.write(str(ans))
    
if __name__ == '__main__':
    main()
