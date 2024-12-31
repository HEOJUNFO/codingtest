import sys
from collections import deque

input = sys.stdin.readline

class AhoCorasick:
    def __init__(self):
        # 각 노드는 다음 알파벳(자식) 정보, 실패 링크, 출력(매칭 여부) 정보를 가진다.
        self.next = []     # next[node] = {'a':다음노드, 'b':..., ...}
        self.fail = []     # fail[node] = 현재 노드에서 매칭이 실패했을 때 이동해야 할 노드
        self.output = []   # output[node] = 이 노드에서 끝나는 패턴들의 인덱스 (또는 True/False 등으로 활용)
        # 루트 노드를 하나 만든다.
        self._createNode()

    def _createNode(self):
        """새로운 노드를 하나 생성하고 해당 노드의 인덱스를 반환한다."""
        self.next.append({})
        self.fail.append(0)
        self.output.append([])
        return len(self.next) - 1

    def insert(self, pattern, pattern_index):
        """트라이에 패턴을 삽입한다. pattern_index는 패턴 식별용(또는 매칭 여부 확인)에 사용 가능"""
        node = 0  # 루트에서 시작
        for ch in pattern:
            if ch not in self.next[node]:
                self.next[node][ch] = self._createNode()
            node = self.next[node][ch]
        self.output[node].append(pattern_index)

    def buildFailLinks(self):
        """실패 함수를 구성한다 (BFS 이용)"""
        queue = deque()
        # 루트(0) 노드의 자식들에 대해 실패 링크와 큐 초기화
        for ch, nxt_node in self.next[0].items():
            self.fail[nxt_node] = 0
            queue.append(nxt_node)

        # BFS로 트라이를 돌면서 실패 링크를 설정
        while queue:
            current = queue.popleft()

            for ch, nxt_node in self.next[current].items():
                # 실패 링크를 타고 부모의 부모(... root)에서 갈 수 있는 최대한 멀리 이동
                f = self.fail[current]
                while f > 0 and ch not in self.next[f]:
                    f = self.fail[f]
                if ch in self.next[f]:
                    f = self.next[f][ch]
                self.fail[nxt_node] = f
                # 실패 링크를 통해서도 매칭되는 패턴이 있을 수 있으므로 output을 병합
                self.output[nxt_node].extend(self.output[f])
                queue.append(nxt_node)

    def search(self, text):
        """text를 한 글자씩 탐색하며 패턴이 등장하는지 검사한다.
           하나라도 매칭되면 True를 반환, 없으면 False를 반환."""
        node = 0
        for ch in text:
            # 현재 노드에서 갈 수 없는 경우, 실패 링크를 타고 이동
            while node > 0 and ch not in self.next[node]:
                node = self.fail[node]
            # 만약 갈 수 있는 간선이 있다면 그쪽으로 이동
            if ch in self.next[node]:
                node = self.next[node][ch]
            # 매칭되는 패턴이 하나라도 있으면 True
            if self.output[node]:
                return True
        return False


def solve():
    N = int(input().strip())
    aho = AhoCorasick()

    # 집합 S의 원소(패턴)들을 트라이에 삽입
    for i in range(N):
        pattern = input().strip()
        aho.insert(pattern, i)

    # 실패 링크(실패 함수) 구성
    aho.buildFailLinks()

    Q = int(input().strip())
    for _ in range(Q):
        query = input().strip()
        # query 문자열을 검사해서 하나라도 매칭되는 패턴이 있으면 YES, 아니면 NO
        if aho.search(query):
            print("YES")
        else:
            print("NO")


if __name__ == "__main__":
    solve()
