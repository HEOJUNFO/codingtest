def solution(arr):
    # 숫자와 연산자 분리
    nums = [int(arr[i]) for i in range(0, len(arr), 2)]
    ops = [arr[i] for i in range(1, len(arr), 2)]
    
    n = len(nums)
    
    # DP 테이블 초기화
    # maxDP[i][j]: nums[i] ~ nums[j] 구간의 최댓값
    # minDP[i][j]: nums[i] ~ nums[j] 구간의 최솟값
    maxDP = [[float('-inf')] * n for _ in range(n)]
    minDP = [[float('inf')] * n for _ in range(n)]
    
    # 길이 1인 구간 초기화 (단일 숫자에 대해서)
    for i in range(n):
        maxDP[i][i] = nums[i]
        minDP[i][i] = nums[i]
    
    # 구간 길이를 늘려가며 DP 계산
    for length in range(2, n+1):  # length: 구간 길이
        for i in range(n-length+1):
            j = i + length - 1
            # i부터 j까지 구간을 나누는 모든 위치 k에 대해 계산
            for k in range(i, j):
                op = ops[k]  # k번째 연산자(ops[k]): nums[k]와 nums[k+1] 사이 연산
                
                # 왼쪽, 오른쪽 구간의 최솟값/최댓값 가져오기
                left_max, left_min = maxDP[i][k], minDP[i][k]
                right_max, right_min = maxDP[k+1][j], minDP[k+1][j]
                
                if op == '+':
                    # 가능한 조합들을 고려
                    new_max = left_max + right_max
                    new_min = left_min + right_min
                else:  # op == '-'
                    new_max = left_max - right_min
                    new_min = left_min - right_max
                
                # maxDP, minDP 갱신
                if new_max > maxDP[i][j]:
                    maxDP[i][j] = new_max
                if new_min < minDP[i][j]:
                    minDP[i][j] = new_min
    
    # 전체 식(0~n-1)을 계산했을 때의 최댓값 반환
    return maxDP[0][n-1]

# 예시 테스트
print(solution(["1", "-", "3", "+", "5", "-", "8"]))  # 기대값: 1
print(solution(["5", "-", "3", "+", "1", "+", "2", "-", "4"]))  # 기대값: 3
