import sys
input = sys.stdin.readline

def manacher(s):
    # 문자열 s를 변환: 예) ABCBA -> #A#B#C#B#A#
    # 양 끝에도 #를 붙여주는 이유: 모든 팰린드롬을 홀수 길이 형태로統一
    T = "#" + "#".join(s) + "#"
    n = len(T)
    P = [0]*n
    C = R = 0  # C: 팰린드롬의 중심, R: 팰린드롬의 오른쪽 끝 인덱스
    
    for i in range(n):
        # i' = i_mirror: i' = 2*C - i
        mirror = 2*C - i
        if i < R:
            P[i] = min(R - i, P[mirror])
        
        # 중심으로부터 양쪽 확장
        while i - P[i] - 1 >= 0 and i + P[i] + 1 < n and T[i - P[i] - 1] == T[i + P[i] + 1]:
            P[i] += 1
        
        # 새로운 팰린드롬이 R을 넘어가면 중심 C와 R 업데이트
        if i + P[i] > R:
            C, R = i, i + P[i]
    
    return P

s = input().strip()
P = manacher(s)

# P[i]는 변환 문자열에서 '#'까지 포함된 반경 크기
# 원래 문자열에서의 팰린드롬 개수 합: sum((P[i]+1)//2)
result = sum((v+1)//2 for v in P)
print(result)
