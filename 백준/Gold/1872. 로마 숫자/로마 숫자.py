import sys
input = sys.stdin.readline

roman_chars = ['I','V','X','L','C','D','M']
ridx = {c:i for i,c in enumerate(roman_chars)}

# 각 블록별 가능한 패턴과 해당 값
# 값 계산: 로마 숫자 규칙에 따른 기본값
C_patterns = [
    ("",0),
    ("C",100),("CC",200),("CCC",300),
    ("CD",400),
    ("D",500),("DC",600),("DCC",700),("DCCC",800),
    ("CM",900)
]

X_patterns = [
    ("",0),
    ("X",10),("XX",20),("XXX",30),
    ("XL",40),
    ("L",50),("LX",60),("LXX",70),("LXXX",80),
    ("XC",90)
]

I_patterns = [
    ("",0),
    ("I",1),("II",2),("III",3),
    ("IV",4),
    ("V",5),("VI",6),("VII",7),("VIII",8),
    ("IX",9)
]

def solve_line(line):
    # 1. 문장에서 로마 숫자 문자만 추출
    chars = [ch.upper() for ch in line if ch in 'ivxlcdm ']
    # 공백 제거
    chars = [c for c in chars if c in roman_chars]

    if not chars:
        return 0

    n = len(chars)

    # 2. next_pos 테이블 생성
    # next_pos[i][r] : i번 인덱스부터 오른쪽으로 문자 roman_chars[r]가 나오는 최소 인덱스
    # 인덱스 i 위치에서 시작할 때 다음문자 찾기 위해 사용
    # 편의를 위해 n 크기+1로 만들어 끝에서 -1 초기화
    next_pos = [[-1]*7 for _ in range(n+1)]
    # 마지막 위치(n)에서는 모든 다음 등장 -1
    # 뒤에서부터 채우기
    last_occ = [-1]*7
    for i in range(n-1,-1,-1):
        ch = chars[i]
        last_occ[ridx[ch]] = i
        for r in range(7):
            next_pos[i][r] = last_occ[r]
    for r in range(7):
        next_pos[n][r] = -1

    # 패턴 매칭 함수
    # start 위치에서 pattern을 subsequence로 매칭한 후 마지막 위치+1 리턴
    # 매칭 실패 시 -1 리턴
    def match_pattern(start, pattern):
        pos = start
        for ch in pattern:
            ni = next_pos[pos][ridx[ch]]
            if ni == -1:
                return -1
            pos = ni+1
        return pos

    # dp 메모이제이션
    dpC = [-1]*(n+1)
    dpX = [-1]*(n+1)
    dpI = [-1]*(n+1)

    # dpI(pos): pos부터 I-블록 패턴 중 하나를 골라 매칭한 뒤 얻는 최대값
    def solveI(pos):
        if dpI[pos] != -1:
            return dpI[pos]
        res = -1
        # 모든 I 패턴 시도
        # I 패턴은 마지막 블록이므로 여기서 끝난다.
        for pat,val in I_patterns:
            if pat == "":
                # 빈 패턴 매칭 -> 아무것도 안 매칭하고 끝
                # 최소 0점
                res = max(res, 0)
            else:
                p = match_pattern(pos, pat)
                if p != -1:
                    res = max(res, val)
        if res == -1:
            res = 0  # 사실 "" 패턴 있으니 -1 될 일 없음
        dpI[pos] = res
        return res

    # dpX(pos): pos부터 X-블록 패턴 하나 선택 후 I-블록 처리
    def solveX(pos):
        if dpX[pos] != -1:
            return dpX[pos]
        res = -1
        for pat,val in X_patterns:
            p = match_pattern(pos, pat)
            if p != -1:
                # X-블록 패턴 매칭 후 I-블록으로
                valI = solveI(p)
                if valI >= 0:
                    res = max(res, val+valI)
        # "" 패턴 가능하므로 항상 최소 0 이상
        if res == -1:
            res = 0
        dpX[pos] = res
        return res

    # dpC(pos): pos부터 C-블록 패턴 선택 후 X-블록 처리
    def solveC(pos):
        if dpC[pos] != -1:
            return dpC[pos]
        res = -1
        for pat,val in C_patterns:
            p = match_pattern(pos, pat)
            if p != -1:
                valX = solveX(p)
                if valX >= 0:
                    res = max(res, val+valX)
        if res == -1:
            res = 0
        dpC[pos] = res
        return res

    # M-블록 처리
    # 가능한 M을 최대한 많이 매칭한 뒤 dpC로 넘어간다.
    # 가능한 많은 M을 매칭하기 위해 M 연속 subsequence를 최대한 찾는다.
    # 하지만 M이 반드시 연속일 필요는 없으나, M-블록은 단순히 M을 반복하는 형태이므로,
    # subsequence 상에서 M을 가능한 많이 추출해야 한다.
    # 이 때, 가장 큰 수를 원하므로 가능한 많은 M을 먼저 시도한다.
    # 하지만 M이 중간에 흩어져 있을 수 있으니, M 매칭 시마다 다음 M 위치를 찾아가며 개수를 센다.
    # 최적화를 위해 전체 M 개수를 한 번에 다 세고, k개의 M을 사용할 때 포지션을 추적하자.

    # M을 k개 선택하는 과정을 효율적으로 하기 위해 미리 M을 고르는 방식을 준비
    # pos=0에서 시작해서 M을 하나 찾으면 pos를 그 다음으로 옮기고 계속 찾는다.
    # 이 때, M 개수별로 끝나는 pos를 리스트로 저장
    M_positions = [0]
    cur_pos = 0
    while True:
        ni = next_pos[cur_pos][ridx['M']]
        if ni == -1:
            break
        cur_pos = ni+1
        M_positions.append(cur_pos)
    # M_positions[k] = k개 M을 사용했을 때 끝나는 위치

    # 이제 k개 M 사용 후 dpC(M_positions[k]) 값을 구해 최대값을 찾는다.
    result = 0
    for k in range(len(M_positions)-1, -1, -1):
        valM = k*1000
        valC = solveC(M_positions[k])
        candidate = valM + valC
        if candidate > result:
            result = candidate

    # result가 0이면 실제로 어떤 로마 숫자도 형성 못한 것일 수 있다.
    # 왜냐하면 모두 "" 패턴만 선택하면 0이 나오는데 그것은 숫자가 없다.
    # 최소 한 글자라도 써야 로마 숫자가 형성됨.
    # 그런데 M을 0개 선택, C,X,I 블록 모두 "" 패턴이면 실제로 아무 문자를 사용 안함 -> 0
    # 이 경우 로마 숫자가 아니다.
    # 로마 숫자를 형성하려면 최소 하나의 문자가 사용되어야 함.
    # result가 0인데 문자를 전혀 안쓴 경우가 문제. 그러나 문자를 쓴 경우 result는 >=1.
    # 문자를 전혀 안 쓴 경우는 result=0일 것이고, 그게 우리가 반환해야 하는 값이기도 하다.
    # 즉, result=0이면 유효한 로마 숫자 없음.

    return result if result > 0 else 0


def main():
    N = int(input())
    for _ in range(N):
        line = input().strip()
        ans = solve_line(line)
        print(ans)

if __name__ == "__main__":
    main()
