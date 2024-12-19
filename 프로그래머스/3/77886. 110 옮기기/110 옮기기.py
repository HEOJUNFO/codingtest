def solution(s):
    answer = []
    for x in s:
        oneCount = 0
        ooz = 0  # "110" 패턴의 개수
        ones = []  # 결과 문자열을 담을 리스트 (나중에 ''.join() 할 예정)

        for ch in x:
            if ch == '1':
                oneCount += 1
            else:
                # '0'을 만난 경우
                if oneCount >= 2:
                    # 앞에 1이 2개 이상이면 "110" 패턴 하나 확보
                    ooz += 1
                    oneCount -= 2
                else:
                    # 앞에 1이 2개 미만이면 현재까지 모아둔 1들을 ones에 추가하고 0 추가
                    if oneCount > 0:
                        ones.extend(['1'] * oneCount)
                    ones.append('0')
                    oneCount = 0

        # 남은 "110" 패턴들 삽입
        for _ in range(ooz):
            ones.extend(['1', '1', '0'])

        # 남은 1들 삽입
        if oneCount > 0:
            ones.extend(['1'] * oneCount)

        answer.append(''.join(ones))

    return answer

