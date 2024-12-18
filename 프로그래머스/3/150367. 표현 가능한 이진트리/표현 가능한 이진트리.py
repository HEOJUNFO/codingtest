def can_form_tree(bits: str, is_top_level: bool) -> bool:
    # 모든 노드가 '0'인지 확인
    if all(b == '0' for b in bits):
        # 최상위 트리인데 모두 0이라면 실제 노드가 없으므로 불가능
        # 서브트리(자식 트리)라면 빈 서브트리를 의미하므로 가능
        return not is_top_level

    length = len(bits)
    if length == 1:
        # 길이가 1이면 루트 노드 하나만 존재
        # '1'이면 실제 노드이므로 유효, '0'이면 실제 노드가 없으므로 불가능
        return bits[0] == '1'

    # 중간 인덱스 (포화 이진트리 중위순회의 루트 위치)
    mid = length // 2
    root = bits[mid]
    if root == '0':
        # 루트 노드가 실제 노드가 아니면 불가능
        return False

    # 루트가 '1'이면 왼/오른 서브트리 검사
    left = bits[:mid]
    right = bits[mid+1:]

    # 왼쪽 서브트리 검사
    if all(b == '0' for b in left):
        left_ok = True  # 빈 서브트리
    else:
        left_ok = can_form_tree(left, False)

    # 오른쪽 서브트리 검사
    if all(b == '0' for b in right):
        right_ok = True  # 빈 서브트리
    else:
        right_ok = can_form_tree(right, False)

    return left_ok and right_ok


def solution(numbers):
    answer = []
    for num in numbers:
        # num을 이진수로 변환
        bits = bin(num)[2:]  # '0b' 제거

        # 포화 이진트리 형태의 길이(2^h - 1)로 맞추기 위한 패딩
        length = len(bits)
        # 2^h -1 >= length 인 최소 h 찾기
        h = 1
        while (2**h - 1) < length:
            h += 1

        # 필요한 길이
        full_length = (2**h - 1)
        # 왼쪽 패딩
        bits = bits.rjust(full_length, '0')

        # 트리로 만들 수 있는지 확인
        if can_form_tree(bits, True):
            answer.append(1)
        else:
            answer.append(0)

    return answer
