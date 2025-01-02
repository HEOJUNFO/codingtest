def solution(enroll, referral, seller, amount):
    import sys
    input = sys.stdin.readline
    
    # 판매원 이름 -> 인덱스
    name_to_idx = {}
    for i, name in enumerate(enroll):
        name_to_idx[name] = i
    
    # 추천인 정보를 인덱스로 저장 (부모 노드)
    parent = [-1] * len(enroll)
    for i, ref in enumerate(referral):
        # 추천인이 "-" 가 아니라면, 해당 추천인의 인덱스를 parent 에 저장
        if ref != "-":
            parent[i] = name_to_idx[ref]
    
    # 각 판매원의 이익금 초기화
    profits = [0] * len(enroll)
    
    # 칫솔 하나당 이익
    PRICE = 100
    
    # seller, amount 길이만큼 판매정보에 대해 순회
    for s, a in zip(seller, amount):
        # s 가 누구인지 인덱스 구하기
        idx = name_to_idx[s]
        # 현재 판매자가 얻은 총 이익 (개수 * 100원)
        profit = a * PRICE
        
        # 이익 배분을 위로 타고 올라가며 분배
        while True:
            # 자신이 가져갈 몫
            keep = profit - (profit // 10)
            # 내 이익 추가
            profits[idx] += keep
            
            # 위로 넘길 몫
            up = profit // 10
            
            # 위로 넘길 돈이 0원이거나, 부모가 없으면(=-1) 종료
            if up == 0 or parent[idx] == -1:
                break
            
            # 다음 부모에게 분배
            idx = parent[idx]
            profit = up
    
    return profits

