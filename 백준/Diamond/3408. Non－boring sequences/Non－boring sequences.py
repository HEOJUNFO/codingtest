import sys
from bisect import bisect_left, bisect_right

def solve():
    input_data = sys.stdin.buffer.read().split()
    t = int(input_data[0])
    pos = 1
    output_lines = []
    for _ in range(t):
        n = int(input_data[pos])
        pos += 1
        arr = list(map(int, input_data[pos:pos+n]))
        pos += n
        
        # 각 값의 등장 인덱스를 저장 (이미 순서대로 추가되므로 정렬할 필요 없음)
        positions = {}
        for idx, value in enumerate(arr):
            if value not in positions:
                positions[value] = []
            positions[value].append(idx)
        
        # 스택을 이용한 분할 정복 (각 구간에 대해 유일한 원소가 있는지 확인)
        stack = [(0, n-1)]
        is_non_boring = True
        
        while stack and is_non_boring:
            l, r = stack.pop()
            # 구간 길이가 0 또는 1이면 항상 non-boring
            if l >= r:
                continue
            
            # 양쪽 끝에서부터 후보를 찾는다.
            i, j = l, r
            found_candidate = None
            while i <= j:
                # 왼쪽 끝 후보 검사
                val_left = arr[i]
                lst = positions[val_left]
                cnt_left = bisect_right(lst, r) - bisect_left(lst, l)
                if cnt_left == 1:
                    found_candidate = i
                    break
                
                # 오른쪽 끝 후보 검사
                val_right = arr[j]
                lst = positions[val_right]
                cnt_right = bisect_right(lst, r) - bisect_left(lst, l)
                if cnt_right == 1:
                    found_candidate = j
                    break
                
                i += 1
                j -= 1
            
            if found_candidate is None:
                is_non_boring = False
                break
            else:
                # 후보 원소를 기준으로 좌우 구간을 검사 대상으로 추가
                stack.append((l, found_candidate - 1))
                stack.append((found_candidate + 1, r))
        
        output_lines.append("non-boring" if is_non_boring else "boring")
    
    sys.stdout.write("\n".join(output_lines))

if __name__ == '__main__':
    solve()
