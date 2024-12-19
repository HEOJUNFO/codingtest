def solution(stones, k):
    left, right = 1, max(stones)
    answer = 0
    
    while left <= right:
        mid = (left + right) // 2
        # mid 명이 건널 수 있는지 확인
        cnt = 0
        possible = True
        for stone in stones:
            if stone < mid:
                cnt += 1
            else:
                cnt = 0
            if cnt >= k:
                possible = False
                break
        
        if possible:
            answer = mid
            left = mid + 1
        else:
            right = mid - 1

    return answer
