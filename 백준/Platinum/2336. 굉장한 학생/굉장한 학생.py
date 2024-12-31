import sys
input = sys.stdin.readline

def solve():
    N = int(input().strip())
    
    # 각 시험 결과(등수 순서로 학생 번호가 주어짐)
    # 예) 첫 시험에서 i등한 학생 -> test1[i-1]
    test1 = list(map(int, input().split()))
    test2 = list(map(int, input().split()))
    test3 = list(map(int, input().split()))
    
    # 각 학생의 (시험별) 등수를 저장할 배열
    # 예) r1[x] = x번 학생의 첫 번째 시험 등수
    r1 = [0] * (N+1)
    r2 = [0] * (N+1)
    r3 = [0] * (N+1)
    
    # 등수 정보를 저장 (i+1등에 해당하는 학생 test1[i])
    # 즉, test1[i] 학생의 첫 시험 등수는 i+1
    for i in range(N):
        r1[test1[i]] = i+1
        r2[test2[i]] = i+1
        r3[test3[i]] = i+1
    
    # 학생들을 첫 시험 등수 기준 오름차순으로 나열
    # st[k] = (r1, r2, r3, 학생번호) 형태여도 되지만 학생번호는 문제 해결에는 직접 쓰지 않으므로 생략 가능
    st = []
    for student_id in range(1, N+1):
        st.append((r1[student_id], r2[student_id], r3[student_id]))
    st.sort(key=lambda x: x[0])  # r1 오름차순
    
    # Fenwick(BIT) 트리로, 인덱스 i까지의 r3 최소값을 저장/질의
    # 여기서는 '최소값 Fenwick 트리'가 필요하므로, 초깃값을 충분히 큰 값으로 둔다.
    INF = 10**9 + 1
    fenwicksum = [INF] * (N+1)
    
    # Fenwick 트리에서 [1..pos] 구간의 최소값 가져오기
    def get_min(pos):
        ret = INF
        while pos > 0:
            if fenwicksum[pos] < ret:
                ret = fenwicksum[pos]
            pos -= pos & -pos
        return ret
    
    # Fenwick 트리에 pos 위치 업데이트 (새 r3 값이 기존보다 더 작으면 갱신)
    def update(pos, val):
        while pos <= N:
            if val < fenwicksum[pos]:
                fenwicksum[pos] = val
            pos += pos & -pos
    
    # '굉장하다'고 판단되는 학생 수
    amazing_count = 0
    
    # r1 오름차순으로 순회하며,
    # 이미 확인한 학생들(r1이 더 좋은 학생들 중)에서
    # 내 (r2, r3)보다 모두 더 좋은 (r2 < 내 r2, r3 < 내 r3) 학생이 있는지 확인
    for rx1, rx2, rx3 in st:
        # 이미 본 학생들 중 (r2 < rx2)의 구간에서 최소 r3 값
        min_r3_in_range = get_min(rx2 - 1) if rx2 > 1 else INF
        
        # 만약 min_r3_in_range 가 rx3 보다 작다면 => 지배당함
        if min_r3_in_range > rx3:
            # 지배당하지 않으므로 '굉장한' 후보가 됨
            amazing_count += 1
        
        # 현재 학생을 Fenwicksum 구조에 반영
        update(rx2, rx3)
    
    print(amazing_count)

# 메인 실행부 (백준에 제출 시에는 아래 부분을 제거하거나 solve()만 호출하도록 하면 됨)
if __name__ == "__main__":
    solve()
