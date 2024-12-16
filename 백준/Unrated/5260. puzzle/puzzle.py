import sys
from itertools import permutations

input = sys.stdin.readline

n = int(input().strip())

pieces = []
for _ in range(4):
    mat = []
    for _ in range(n):
        row = list(map(int, input().strip().split()))
        mat.append(row)
    pieces.append(mat)
    blank = input().strip()  # 빈 줄 읽기 (마지막에서는 없을 수 있으므로 무시)

# 퍼즐 변환 함수들
def rotate90(mat):
    return list(zip(*mat[::-1]))

def rotate180(mat):
    return rotate90(rotate90(mat))

def rotate270(mat):
    return rotate90(rotate180(mat))

def flip_horizontal(mat):
    # 수평으로 뒤집기(좌우반전)
    return [row[::-1] for row in mat]

def to_list_of_lists(mat):
    return [list(row) for row in mat]

def all_transformations(mat):
    # 8가지 변환
    # 1. 그대로
    # 2. 90도 회전
    # 3. 180도 회전
    # 4. 270도 회전
    # 5. 좌우반전
    # 6. 좌우반전 후 90도 회전
    # 7. 좌우반전 후 180도 회전
    # 8. 좌우반전 후 270도 회전
    res = []
    # 원본
    m0 = mat
    # 회전들
    m90 = to_list_of_lists(rotate90(m0))
    m180 = to_list_of_lists(rotate180(m0))
    m270 = to_list_of_lists(rotate270(m0))
    # 플립
    mf = flip_horizontal(m0)
    mf90 = to_list_of_lists(rotate90(mf))
    mf180 = to_list_of_lists(rotate180(mf))
    mf270 = to_list_of_lists(rotate270(mf))
    res = [m0, m90, m180, m270, mf, mf90, mf180, mf270]
    return res


# 최종 퍼즐 크기
final_size = 2*n - 1

def can_assemble(p1, p2, p3, p4):
    # p1: top-left
    # p2: top-right
    # p3: bottom-left
    # p4: bottom-right

    # 최종 퍼즐 초기화
    final = [[0]*(final_size) for _ in range(final_size)]

    # top-left 배치
    for i in range(n):
        for j in range(n):
            if p1[i][j] != 0:
                final[i][j] = p1[i][j]
            # p1의 위치에 대해 0일 경우 그냥 냅둔다.

    # top-right 배치 (0..n-1, n-1..2n-2)
    for i in range(n):
        for j in range(n):
            fi, fj = i, j+(n-1)
            if p2[i][j] != 0:
                if final[fi][fj] == 0:
                    final[fi][fj] = p2[i][j]
                else:
                    # 겹치는 영역에서 충돌 발생하면 실패
                    return None

    # bottom-left 배치 (n-1..2n-2, 0..n-1)
    for i in range(n):
        for j in range(n):
            fi, fj = i+(n-1), j
            if p3[i][j] != 0:
                if final[fi][fj] == 0:
                    final[fi][fj] = p3[i][j]
                else:
                    # 충돌
                    return None

    # bottom-right 배치 (n-1..2n-2, n-1..2n-2)
    for i in range(n):
        for j in range(n):
            fi, fj = i+(n-1), j+(n-1)
            if p4[i][j] != 0:
                if final[fi][fj] == 0:
                    final[fi][fj] = p4[i][j]
                else:
                    # 충돌
                    return None

    # 모든 칸이 0이 아니어야 하는지 확인 (문제 설명상 모든 부분이 잘 채워진다고 언급)
    # 문제에서 'fit perfectly'라 했으므로 모든 빈칸 없이 맞아떨어져야 한다.
    for row in final:
        if any(x == 0 for x in row):
            return None

    return final


# 모든 조합 시도
from math import factorial

indices = [0,1,2,3]
piece_transformations = [all_transformations(p) for p in pieces]

for perm in permutations(indices):
    # perm: 어떤 순서로 조각을 배치할지
    # 순서: top-left, top-right, bottom-left, bottom-right
    # 각 조각에 대해 8가지 변환 모두 시도
    # 완전 탐색
    for t1 in piece_transformations[perm[0]]:
        for t2 in piece_transformations[perm[1]]:
            for t3 in piece_transformations[perm[2]]:
                for t4 in piece_transformations[perm[3]]:
                    res = can_assemble(t1, t2, t3, t4)
                    if res is not None:
                        # 정답 출력
                        for row in res:
                            print(' '.join(map(str, row)))
                        sys.exit(0)
